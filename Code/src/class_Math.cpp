/******************************************************************************
 * class_Math.cpp
 * This file has the functions that do generical math calculations like average
 * and power reduction
 *****************************************************************************/

#include "class_Math.h"

// Class constructors
Math::Math(){};
Math::~Math(){};

// Configures the object for average calculation
void Math::confAvgCalc(uint16_t _arraySize, uint8_t _type, uint16_t initValue)
{
    // Save the parameters
    arraySize = _arraySize;
    type = _type;

    // Makes sure that there is not array attached
    if (arrayInt8)
        free(arrayInt8);
    if (arrayInt16)
        free(arrayInt16);
    if (arrayInt32)
        free(arrayInt32);
    if (arrayUint8)
        free(arrayUint8);
    if (arrayUint16)
        free(arrayUint16);
    if (arrayUint32)
        free(arrayUint32);
    if (arrayDouble)
        free(arrayDouble);

    // Reserves memory space for the array
    // Depending on the type of variable, a different array is used
    switch (type)
    {
    // If it's int8
    case 2:
        arrayInt8 = (int8_t *)malloc(arraySize * sizeof(int8_t));
        break;

    // If it's int16
    case 3:
        arrayInt16 = (int16_t *)malloc(arraySize * sizeof(int16_t));
        break;

    // If it's int32
    case 4:
        arrayInt32 = (int32_t *)malloc(arraySize * sizeof(int32_t));
        break;

    // If it's uint8
    case 5:
        arrayUint8 = (uint8_t *)malloc(arraySize * sizeof(uint8_t));
        break;

    // If it's uint16
    case 6:
        arrayUint16 = (uint16_t *)malloc(arraySize * sizeof(uint16_t));
        break;

    // If it's uint32
    case 7:
        arrayUint32 = (uint32_t *)malloc(arraySize * sizeof(uint32_t));
        break;

    // If it's double
    case 8:
        arrayDouble = (double *)malloc(arraySize * sizeof(double));
        break;

    default:
        break;
    }

    // Force the init value
    forceAvg(initValue);
}

// Configures the object for power reduction calculation. Also configures for average
void Math::confPwrReductionCalc(double _minLimit, double _maxLimit, double _maxPercentage, uint16_t _arraySize, uint8_t _type, uint16_t initValue)
{
    // Save the parameters
    minLimit = _minLimit;
    maxLimit = _maxLimit;
    maxPercentage = _maxPercentage;
    arraySize = _arraySize;
    type = _type;

    // Configurates the avg variables
    confAvgCalc(arraySize, type, initValue);
}

// Get the current average
double Math::getAvg()
{
    return average;
}

// Get the current power reduction percentage
double Math::getPwrReduction()
{
    return pwrReduction;
}

// Map function using decimal numbers
double Math::mapd(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//Follow the current speed and speed range change regen
double Math::Regend(double max_speed,double min_speed, double cur_speed, double cur_regen, double percentage)
{
    return cur_regen * percentage / (max_speed - min_speed) * (max_speed - cur_speed) + cur_regen * percentage;
}    

// Set all array data to a specific value to achieve a desired average (limited from 0 to 65535 int)
void Math::forceAvg(uint16_t value)
{
    // Put the desired value in all positions of the array
    // Depending on the type of variable, a different array is used
    for (uint16_t i = 0; i < arraySize; i++)
    {
        switch (type)
        {
        // If it's int8
        case 2:
            arrayInt8[i] = (int8_t)value;
            break;

        // If it's int16
        case 3:
            arrayInt16[i] = (int16_t)value;
            break;

        // If it's int32
        case 4:
            arrayInt32[i] = (int32_t)value;
            break;

        // If it's uint8
        case 5:
            arrayUint8[i] = (uint8_t)value;
            break;

        // If it's uint16
        case 6:
            arrayUint16[i] = (uint16_t)value;
            break;

        // If it's uint32
        case 7:
            arrayUint32[i] = (uint32_t)value;
            break;

        // If it's double
        case 8:
            arrayDouble[i] = (double)value;
            break;

        default:
            break;
        }
    }
}