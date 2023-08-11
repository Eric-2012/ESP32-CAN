/******************************************************************************
 * class_Math.h
 * Header for class_Math.cpp
 *****************************************************************************/
#ifndef __CLASS_MATH_H__
#define __CLASS_MATH_H__

#include <Arduino.h>

#include "global_Constants.h"
#include "global_Variables.h"

class Math
{
private:
    // ==================================================================
    // Variables for average

    // Number of elements on the array to calculates the average
    uint16_t arraySize;

    /* Type of parameter
    * 2 = int8_t
    * 3 = int16_t
    * 4 = int32_t
    * 5 = uint8_t
    * 6 = uint16_t
    * 7 = uint32_t
    * 8 = double
    */
    uint8_t type = 0;

    // Current average
    double average = 0;

    // Index used to pupulate the array
    uint16_t index = 0;

    // Arrays
    // As it can be different variable types, there are different arrays possibilities
    int8_t *arrayInt8;
    int16_t *arrayInt16;
    int32_t *arrayInt32;
    uint8_t *arrayUint8;
    uint16_t *arrayUint16;
    uint32_t *arrayUint32;
    double *arrayDouble;

    // ==================================================================
    // Variables for power reduction

    // Range for the map function
    double minLimit;
    double maxLimit;

    // Maximum reduction percentage
    double maxPercentage;

    // Current power reduction percentage
    double pwrReduction = 0;

    // Map function using decimal numbers
    double mapd(double x, double in_min, double in_max, double out_min, double out_max);

public:
    // Class constructors
    Math();
    ~Math();

    //Follow the current speed and speed range change regen
    double Regend(double max_speed,double min_speed, double cur_speed, double cur_regen, double percentage);

    // Configures the object for average calculation
    void confAvgCalc(uint16_t _arraySize, uint8_t _type, uint16_t initValue = 0);

    // Configures the object for power reduction calculation. Also configures for average
    void confPwrReductionCalc(double _minLimit, double _maxLimit, double _maxPercentage, uint16_t _arraySize, uint8_t _type, uint16_t initValue = 0);

    // Get the current average
    double getAvg();

    // Get the current power reduction percentage
    double getPwrReduction();

    // Set all array data to a specific value to achieve a desired average (limited from 0 to 65535 int)
    void forceAvg(uint16_t value);

    // Add a new data to the array and return the current average
    template <typename varType>
    double addNCalcAvg(varType &value)
    {
        // Variables used in this function
        double auxAverage = 0;

        // Put the value in the array and prepares to the average calculation
        // Depending on the type of variable, a different array is used
        switch (type)
        {
        // If it's int8
        case 2:
            arrayInt8[index] = (int8_t)value;
            for (uint16_t i = 0; i < arraySize; i++)
                auxAverage += (double)arrayInt8[i];
            break;

        // If it's int16
        case 3:
            arrayInt16[index] = (int16_t)value;
            for (uint16_t i = 0; i < arraySize; i++)
                auxAverage += (double)arrayInt16[i];
            break;

        // If it's int32
        case 4:
            arrayInt32[index] = (int32_t)value;
            for (uint16_t i = 0; i < arraySize; i++)
                auxAverage += (double)arrayInt32[i];
            break;

        // If it's uint8
        case 5:
            arrayUint8[index] = (uint8_t)value;
            for (uint16_t i = 0; i < arraySize; i++)
                auxAverage += (double)arrayUint8[i];
            break;

        // If it's uint16
        case 6:
            arrayUint16[index] = (uint16_t)value;
            for (uint16_t i = 0; i < arraySize; i++)
                auxAverage += (double)arrayUint16[i];
            break;

        // If it's uint32
        case 7:
            arrayUint32[index] = (uint32_t)value;
            for (uint16_t i = 0; i < arraySize; i++)
                auxAverage += (double)arrayUint32[i];
            break;

        // If it's double
        case 8:
            arrayDouble[index] = (double)value;
            for (uint16_t i = 0; i < arraySize; i++)
                auxAverage += (double)arrayDouble[i];
            break;

        default:
            break;
        }

        // Calculates the average
        auxAverage /= (double)arraySize;

        // Prepares the index for the next cycle
        index++;
        if (index >= arraySize)
        {
            index = 0;
        }

        // Output the value
        average = auxAverage;
        return average;
    }

    // Add a new data to the array and return the current average
    template <typename varType>
    double addNCalcPwrReduction(varType &value)
    {
        // Calculates the average
        addNCalcAvg(value);

        // Calculates the power reduction
        double auxPwrReduction = mapd(average, minLimit, maxLimit, 0.0, maxPercentage);

        // Return the PwrReduction percentage
        pwrReduction = (auxPwrReduction < 0) ? 0.0 : auxPwrReduction;
        return pwrReduction;
    }
};

#endif