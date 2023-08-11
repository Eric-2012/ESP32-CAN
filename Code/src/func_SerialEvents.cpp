/******************************************************************************
 * func_SerialEvents.cpp
 * In this file is the functions that take care of the serial communication
 * with a computer
 *****************************************************************************/

#include "func_SerialEvents.h"

// Variables used in all functions regarding to the serial communication with a computer
char serialReading[200];
uint8_t serialBytesReceived;

// There is a basic encryption used in the serial communication
// When the communication starts, ESP32 sent 1 numbers with 16 bits. The nibbles will be sum in a byte variable (possible overflow). The result will be used in a XOR encryption
// The valid packet detection is done by the EVK in the beggining
uint16_t serialEncryptNumber;
uint8_t serialEncryptPassword;

// Makes possible to use text in the switch case that handles the serial communication
enum SerialStates
{
    StartCommunication,
    RecvParameterCode,
    RecvParameterContent
};

// Handle the serial events
void serialEvent()
{
    // Variables to be used in this fucntion
    static uint8_t serialState = 0;
    static uint16_t paramCode = 0;

    // Temp variables to receive data from the serial, depending on the parameter type
    bool tempBool;
    int8_t tempInt8;
    int16_t tempInt16;
    int32_t tempInt32;
    uint8_t tempUint8;
    uint16_t tempUint16;
    uint32_t tempUint32;
    double tempDouble;
    char tempChar[60];

    // Clean the reading array
    memset(serialReading, 0, sizeof(serialReading));

    // Read the content coming from the serial
    serialBytesReceived = Serial.readBytesUntil('\n', serialReading, sizeof(serialReading));
    /*for (uint8_t x = 0; x < serialBytesReceived; x++)
    {
        Serial.print(serialReading[x]);
        Serial.print("|");
    }
    Serial.println("");*/

    // Coordinates the serial communication
    switch (serialState)
    {
    // The serial communication haven't started yet
    case StartCommunication:
        // If a parameters configuration is starting
        if (strcmp(serialReading, "pc") == 0)
        {
            // Choose a random number with 16 bits that will be used to calculate the encryption pass in both sides
            serialEncryptNumber = random(0x0000, 0xFFFF);

            // Send the number
            Serial.print("n");
            Serial.println(serialEncryptNumber);

            // Calculates the password to be used in the ESP32
            serialEncryptPassword = (serialEncryptNumber >> 8) + (serialEncryptNumber & 0xFF); // Sum both bytes in one

            // Go to the another state, to receive the parameter number
            serialState = RecvParameterCode;
        }
        // If the message is pr, it means parameter report
        else if (strcmp(serialReading, "pr") == 0)
        {
            sendParametersReport();
        }
        // If the message is up, it means to read from the FRAM memory and update the variables
        else if (strcmp(serialReading, "up") == 0)
        {
            // The task BusI2C knows that it needs to read all data from the FRAM
            flagToReadFromFRAM = true;
        }
        break;

    case RecvParameterCode:
        // In this part, for sure, the number of bytes received is 5 (3 for signature and 2 for code), so if it is more, just ignore and come back to the first state
        if (serialBytesReceived == 5)
        {
            // Decrypt the serial array and verify if the data is valid
            if (decryptNVerifySerialArray())
            {
                // Get the paramCode, that is the first and second elements from the array, because the signature was already removed
                paramCode = serialReading[0];
                paramCode <<= 8;
                paramCode += serialReading[1];

                // Checks if the paramemeter is being used
                if (paramFRAM[paramCode].isConfigured())
                {
                    serialState = RecvParameterContent;
                }
                else
                {
                    // If not restart the serial process
                    serialState = StartCommunication;
                }
            }
            else
            {
                // If not valid, come back to the first state
                serialState = StartCommunication;
            }
        }
        else
        {
            serialState = StartCommunication;
        }

        break;

    case RecvParameterContent:
        // Decrypt the serial array and verify if the data is valid
        if (decryptNVerifySerialArray())
        {
            /*for (uint8_t x = 0; x < serialBytesReceived; x++)
            {
                Serial.print(serialReading[x]);
                Serial.print("|");
            }
            Serial.println("");*/

            Serial.print("Param: ");
            Serial.print(paramCode);
            Serial.print(" | ");

            // Depending on the parameter type, put the value on different variables
            switch (paramFRAM[paramCode].getType())
            {
            case 1: // bool
                tempBool = (bool)atol(serialReading);
                paramFRAM[paramCode].requestWrite(tempBool);
                Serial.print("bool: ");
                Serial.println(tempBool);
                break;

            case 2: // int8_t
                tempInt8 = (int8_t)atol(serialReading);
                paramFRAM[paramCode].requestWrite(tempInt8);
                Serial.print("int8_t: ");
                Serial.println(tempInt8);
                break;

            case 3: // int16_t
                tempInt16 = (int16_t)atol(serialReading);
                paramFRAM[paramCode].requestWrite(tempInt16);
                Serial.print("int16_t: ");
                Serial.println(tempInt16);
                break;

            case 4: // int32_t
                tempInt32 = (int32_t)atol(serialReading);
                paramFRAM[paramCode].requestWrite(tempInt32);
                Serial.print("int32_t: ");
                Serial.println(tempInt32);
                break;

            case 5: // uint8_t
                tempUint8 = (uint8_t)atol(serialReading);
                paramFRAM[paramCode].requestWrite(tempUint8);
                Serial.print("uint8_t: ");
                Serial.println(tempUint8);
                break;

            case 6: // uint16_t
                tempUint16 = (uint16_t)atol(serialReading);
                paramFRAM[paramCode].requestWrite(tempUint16);
                Serial.print("uint16_t: ");
                Serial.println(tempUint16);
                break;

            case 7: // uint32_t
                tempUint32 = (uint32_t)atol(serialReading);
                paramFRAM[paramCode].requestWrite(tempUint32);
                Serial.print("uint32_t: ");
                Serial.println(tempUint32);
                break;

            case 8: // double
                tempDouble = (double)atof(serialReading);
                paramFRAM[paramCode].requestWrite(tempDouble);
                Serial.print("double: ");
                Serial.println(tempDouble);
                break;

            case 9: // char array - In this case, it's just necessary to copy the array
                // Clean the buffer array
                memset(tempChar, 0, sizeof(tempChar));

                // Get the data
                memcpy(tempChar, serialReading, serialBytesReceived);
                paramFRAM[paramCode].requestWrite(tempChar);

                Serial.print("char: ");
                Serial.println(tempChar);
                break;

            default:
                break;
            }
        }
        else
        {
            // If not valid, come back to the first state
            serialState = StartCommunication;
        }
        break;
    }
}

// Decrypt the reading array and verify if is valid
bool decryptNVerifySerialArray()
{
    // Go to each array item, doing the XOR with the password
    for (uint8_t x = 0; x < serialBytesReceived; x++)
    {
        serialReading[x] ^= serialEncryptPassword;
    }

    // Check if the first elements are EVK
    if (serialReading[0] == 'E' && serialReading[1] == 'V' && serialReading[2] == 'K')
    {
        // Remove the first 3 elements
        for (uint8_t x = 0; x < serialBytesReceived; x++)
        {
            if (x < (serialBytesReceived - 3))
            {
                // Move elements 3 positions
                serialReading[x] = serialReading[x + 3];
            }
            else
            {
                // Put 0 in the others
                serialReading[x] = 0;
            }
        }

        // Return true, because the data is valid
        return true;
    }
    // Return false if not valid
    else
    {
        return false;
    }
}

// Send the parameters report via serial
void sendParametersReport()
{
    // Pass by the ParamFRAM array to send codes and values via serial
    for (uint8_t x = 0; x < maxNumParameters; x++)
    {
        // Send only the parameters already configured
        if (paramFRAM[x].isConfigured())
        {
            // Send "pr", so the computer software will show in the text box
            Serial.print("pr");

            // Send the parameter number
            Serial.print(x);

            // Used for divisions
            Serial.print("|");

            // Send the value
            paramFRAM[x].printValue();

            // Break the line
            Serial.println();

            // Delay for serial
            delay(3);
        }
    }
}
