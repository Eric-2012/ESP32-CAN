/******************************************************************************
 * class_ParamFRAM.h
 * Header for class_ParamFRAM.cpp
 *****************************************************************************/
#ifndef __CLASS_PARAM_H__
#define __CLASS_PARAM_H__

#include <Arduino.h>

#include "global_Constants.h"
#include "global_Variables.h"
#include "func_Misc.h"

// Instance for I2C0
extern TwoWire I2C0;

class ParamFRAM
{
private:
    // True if the object is already configured, false if not
    bool configured = false;

    // Parameter's address in the FRAM Memory
    uint16_t memAddr;

    /* Type of parameter
    * 0 = not configurated
    * 1 = bool
    * 2 = int8_t
    * 3 = int16_t
    * 4 = int32_t
    * 5 = uint8_t
    * 6 = uint16_t
    * 7 = uint32_t
    * 8 = double
    * 9 = char array
    */
    uint8_t type;

    // Size in bytes
    int size;

    // Update requested
    bool updateRequested = false;

    // Variables that have the parameter value
    // As sizes are differents, they are just pointers at first, but they are configured when the parameter is created
    bool *valBool;
    int8_t *valInt8;
    int16_t *valInt16;
    int32_t *valInt32;
    uint8_t *valUint8;
    uint16_t *valUint16;
    uint32_t *valUint32;
    double *valDouble;
    char *valChar;

    // Write function - If the data is a char array
    // To keep an order in the use of the I2C bus, the writing function is done just indide
    // of the class. The writing update request are done with the requestWrite function
    void write(char *source);

    // Write function - If the data is NOT a char array
    // To keep an order in the use of the I2C bus, the writing function is done just indide
    // of the class. The writing update request are done with the requestWrite function
    template <typename varType>
    void write(varType &source)
    {
        // Allow writing in the FRAM Memory
        digitalWrite(pinWP_FRAM, LOW);
        delay(5);

        // Creates an union, that means, both different kind of variables will use same positions in memory
        union split
        {
            varType data = 0xFF;
            uint8_t data8[sizeof(varType)];
        };
        union split split;
        split.data = source;

        // Write to the FRAM
        I2C0.beginTransmission(addrI2C_FRAM);
        I2C0.write((int)(memAddr >> 8));   // MSB
        I2C0.write((int)(memAddr & 0xFF)); // LSB
        for (uint8_t i = 0; i < sizeof(varType); i++)
        {
            I2C0.write(split.data8[i]);
        }
        I2C0.endTransmission();

        // Protect writing in the FRAM Memory
        digitalWrite(pinWP_FRAM, HIGH);
    }

public:
    // Class constructors
    ParamFRAM();
    ~ParamFRAM();

    // Configurates the parameter
    void conf(uint16_t _memAddr, uint8_t _type, uint8_t _size = 0);

    // Just the task I2C uses the I2C. If other part will start to use it will have conflict
    // So if the request to write a parameter is comming from other part, it needs to be scheduled, then the task will update
    void requestWrite(char *source);

    template <typename varType>
    void requestWrite(varType &source)
    {
        // Flags that an uptade needs to be done
        updateRequested = true;

        // Depending on the parameter type, put the value on different variables
        switch (type)
        {
        case 1: // bool
            valBool[0] = (bool)source;
            break;

        case 2: // int8_t
            valInt8[0] = (int8_t)source;
            break;

        case 3: // int16_t
            valInt16[0] = (int16_t)source;
            break;

        case 4: // int32_t
            valInt32[0] = (int32_t)source;
            break;

        case 5: // uint8_t
            valUint8[0] = (uint8_t)source;
            break;

        case 6: // uint16_t
            valUint16[0] = (uint16_t)source;
            break;

        case 7: // uint32_t
            valUint32[0] = (uint32_t)source;
            break;

        case 8: // double
            valDouble[0] = (double)source;
            break;
        }
    }

    // Check if there is an update request and do it
    void checkNWrite();

    // Read function - If the data is a char array
    void read(char *data);

    // Read function - If the data is NOT a char array
    template <typename varType>
    void read(varType &target)
    {
        // Creates an union, that means, both different kind of variables will use same positions in memory
        union split
        {
            varType data = 0xFF;
            uint8_t data8[sizeof(varType)];
        };
        union split split;

        // Read from FRAM
        I2C0.beginTransmission(addrI2C_FRAM);
        I2C0.write((int)(memAddr >> 8));   // MSB
        I2C0.write((int)(memAddr & 0xFF)); // LSB
        I2C0.endTransmission();
        I2C0.requestFrom(addrI2C_FRAM, sizeof(varType));
        for (uint8_t i = 0; i < sizeof(varType); i++)
        {
            if (I2C0.available())
                split.data8[i] = I2C0.read();
        }

        // As the array of bytes are in the same memory position of the data, return the data
        target = split.data;

        // Keep the value saved inside of the object
        switch (type)
        {
        case 1: // bool
            valBool[0] = (bool)split.data;
            break;

        case 2: // int8_t
            valInt8[0] = (int8_t)split.data;
            break;

        case 3: // int16_t
            valInt16[0] = (int16_t)split.data;
            break;

        case 4: // int32_t
            valInt32[0] = (int32_t)split.data;
            break;

        case 5: // uint8_t
            valUint8[0] = (uint8_t)split.data;
            break;

        case 6: // uint16_t
            valUint16[0] = (uint16_t)split.data;
            break;

        case 7: // uint32_t
            valUint32[0] = (uint32_t)split.data;
            break;

        case 8: // double
            valDouble[0] = (double)split.data;
            break;
        }
    }

    // Returns the type
    uint8_t getType();

    // Returns the size
    uint8_t getSize();

    // Print the value in the serial port
    void printValue();

    // Return if the object is configured
    bool isConfigured();
};
#endif