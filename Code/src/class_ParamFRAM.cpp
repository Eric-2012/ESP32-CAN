#include "class_ParamFRAM.h"

// Class constructors
ParamFRAM::ParamFRAM(){};
ParamFRAM::~ParamFRAM(){};

// Configure the object
void ParamFRAM::conf(uint16_t _memAddr, uint8_t _type, uint8_t _size)
{
    // Set the the object is already configured
    configured = true;

    // Save the address
    memAddr = _memAddr;

    // Save the type
    type = _type;

    // Save the size, according to the type
    if ((type == 1) || (type == 2) || (type == 5)) // If the type is bool, int8 or uint8, it's 1 byte
        size = 1;                                  //
    else if ((type == 3) || (type == 6))           // If the type is int16 or uint16, it's 2 bytes
        size = 2;                                  //
    else if ((type == 4) || (type == 7))           // If the type is int32 or uint32, it's 4 bytes
        size = 4;                                  //
    else if (type == 8)                            // If the type is double, it's 8 bytes
        size = 8;                                  //
    else if (type == 9)                            // If the type is char array, the size is the one used in the constructor
        size = _size;                              //

    // Configure the variable to store the value
    // First make sure that they are free
    if (valBool)
        free(valBool);
    if (valInt8)
        free(valInt8);
    if (valInt16)
        free(valInt16);
    if (valInt32)
        free(valInt32);
    if (valUint8)
        free(valUint8);
    if (valUint16)
        free(valUint16);
    if (valUint32)
        free(valUint32);
    if (valDouble)
        free(valDouble);
    if (valChar)
        free(valChar);

    // Configure the variable depending on the parameter type
    switch (type)
    {
    case 1: // bool
        valBool = (bool *)malloc(sizeof(bool));
        break;

    case 2: // int8_t
        valInt8 = (int8_t *)malloc(sizeof(int8_t));
        break;

    case 3: // int16_t
        valInt16 = (int16_t *)malloc(sizeof(int16_t));
        break;

    case 4: // int32_t
        valInt32 = (int32_t *)malloc(sizeof(int32_t));
        break;

    case 5: // uint8_t
        valUint8 = (uint8_t *)malloc(sizeof(uint8_t));
        break;

    case 6: // uint16_t
        valUint16 = (uint16_t *)malloc(sizeof(uint16_t));
        break;

    case 7: // uint32_t
        valUint32 = (uint32_t *)malloc(sizeof(uint32_t));
        break;

    case 8: // double
        valDouble = (double *)malloc(sizeof(double));
        break;

    case 9: // char
        valChar = (char *)malloc(size * sizeof(char));
        break;
    }
}

// Check if there is an update request and do it
void ParamFRAM::requestWrite(char *source)
{
    // Flags that an uptade needs to be done
    updateRequested = true;

    // This is just for char, so it's not necessary to verify the variable type
    strcpy(valChar, source);
}

// Check if there is an update request and do it
void ParamFRAM::checkNWrite()
{
    // Just if the parameter is configured
    if (configured)
    {
        // Check if there is an update to be done
        if (updateRequested)
        {
            // Flags that the update was already done
            updateRequested = false;

            // Depending on the parameter type, put the value on different variables
            switch (type)
            {
            case 1: // bool
                write(valBool[0]);
                break;

            case 2: // int8_t
                write(valInt8[0]);
                break;

            case 3: // int16_t
                write(valInt16[0]);
                break;

            case 4: // int32_t
                write(valInt32[0]);
                break;

            case 5: // uint8_t
                write(valUint8[0]);
                break;

            case 6: // uint16_t
                write(valUint16[0]);
                break;

            case 7: // uint32_t
                write(valUint32[0]);
                break;

            case 8: // double
                write(valDouble[0]);
                break;

            case 9: // char
                write(valChar);
                break;
            }
        }
    }
}

// Write function - If the data is a char array
// If the data is NOT a char array, the function is a templeate on the header file
void ParamFRAM::write(char *source)
{
    // Allow writing in the FRAM Memory
    digitalWrite(pinWP_FRAM, LOW);
    delay(5);

    // Write to the FRAM
    I2C0.beginTransmission(addrI2C_FRAM);
    I2C0.write((int)(memAddr >> 8));   // MSB
    I2C0.write((int)(memAddr & 0xFF)); // LSB
    for (uint8_t i = 0; i < size; i++)
    {
        I2C0.write(source[i]);
    }
    I2C0.endTransmission();

    // Protect writing in the FRAM Memory
    digitalWrite(pinWP_FRAM, HIGH);
}

// Read function - If the data is a char array
// If the data is NOT a char array, the function is a templeate on the header file
void ParamFRAM::read(char *target)
{
    // Read from FRAM
    I2C0.beginTransmission(addrI2C_FRAM);
    I2C0.write((int)(memAddr >> 8));   // MSB
    I2C0.write((int)(memAddr & 0xFF)); // LSB
    I2C0.endTransmission();
    I2C0.requestFrom(addrI2C_FRAM, size);
    for (uint8_t i = 0; i < size; i++)
    {
        if (I2C0.available()) {
            target[i] = I2C0.read();

            // Keep the value saved
            valChar[i] = target[i];
        }
    }
}

// Returns the type
uint8_t ParamFRAM::getType()
{
    return type;
}

// Returns the size
uint8_t ParamFRAM::getSize()
{
    return size;
}

// Print the value in the serial port
void ParamFRAM::printValue()
{
    // Depending on the parameter type, print different variable
    switch (type)
    {
    case 1: // bool
        Serial.print(valBool[0]);
        break;

    case 2: // int8_t
        Serial.print(valInt8[0]);
        break;

    case 3: // int16_t
        Serial.print(valInt16[0]);
        break;

    case 4: // int32_t
        Serial.print(valInt32[0]);
        break;

    case 5: // uint8_t
        Serial.print(valUint8[0]);
        break;

    case 6: // uint16_t
        Serial.print(valUint16[0]);
        break;

    case 7: // uint32_t
        Serial.print(valUint32[0]);
        break;

    case 8: // double
        Serial.print(valDouble[0]);
        break;

    case 9: // char
        /*for (uint8_t x = 0; x < size; x++)
        {
            Serial.print(valChar[x]);
        }*/
        Serial.print(valChar);
        break;
    }
}

// Return if the object is configured
bool ParamFRAM::isConfigured()
{
    return configured;
}