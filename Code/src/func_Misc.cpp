/******************************************************************************
 * func_Misc.cpp
 * This file has miscellaneous functions
 *****************************************************************************/

#include "func_Misc.h"

// Put reset pin to HIGH
void activatePeripheralComponents()
{
    digitalWrite(pinReset, HIGH);
}

// Put reset pin to LOW
void disablePeripheralComponents()
{
    digitalWrite(pinReset, LOW);
}

// Convert degrees Celsius to degrees Fahrenheit
double convertCelsiusToFahrenheit(double celsius)
{
    return (celsius * 9 / 5) + 32;
}

// Convert km to miles
double convertKmToM(double km)
{
    return km / 1.609;
}

// Convert Km/h to Mph
double convertKmhToMph(double kmh)
{
    return convertKmToM(kmh);
}

// Receives 2 bytes and return an unsigned int with 16 bits
uint16_t uint8To_Uint16(uint8_t byte1, uint8_t byte0)
{
    uint16_t aux = 0;
    aux = (uint16_t)byte1 << 8;
    aux += (uint16_t)byte0;

    return aux;
}

// Receives 4 bytes and return an unsigned int with 32 bits
uint32_t uint8To_Uint32(uint8_t byte3, uint8_t byte2, uint8_t byte1, uint8_t byte0)
{
    uint32_t aux = 0;
    aux = (uint32_t)byte3 << 24;
    aux += (uint32_t)byte2 << 16;
    aux += (uint32_t)byte1 << 8;
    aux += (uint32_t)byte0;

    return aux;
}

// Receives 2 bytes and return a signed int with 16 bits
int16_t uint8To_Int16(uint8_t byte1, uint8_t byte0)
{
    int16_t aux = 0;
    aux = (int16_t)byte1 << 8;
    aux += (int16_t)byte0;

    return aux;
}

// Receives 4 bytes and return a signed int with 32 bits
int32_t uint8To_Int32(uint8_t byte3, uint8_t byte2, uint8_t byte1, uint8_t byte0)
{
    int32_t aux = 0;
    aux = (int32_t)byte3 << 24;
    aux += (int32_t)byte2 << 16;
    aux += (int32_t)byte1 << 8;
    aux += (int32_t)byte0;

    return aux;
}

// Receive the actual state from a variable and check if it was up the minimal time to be used as high
void debounce(bool actualState, uint8_t interval, int8_t *count, bool *input)
{
    // If the input if high, checks if already passed the minumun count of cycles to put as high, if not, increment the counter
    if (actualState)
    {
        if (*count >= interval)
        {
            *input = true;
        }
        else
        {
            *count = *count + 1;
        }
    }
    // If the input is low, decrease the counter, doing a debounce to be low as well
    else
    {
        if (*count <= 0)
        {
            *input = false;
        }
        else
        {
            *count = *count - 1;
        }
    }
}

/* Type of parameter
 *0 = not configurated
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
uint8_t checkVarType(bool *aux) { return 1; }
uint8_t checkVarType(int8_t *aux) { return 2; }
uint8_t checkVarType(int16_t *aux) { return 3; }
uint8_t checkVarType(int32_t *aux) { return 4; }
uint8_t checkVarType(uint8_t *aux) { return 5; }
uint8_t checkVarType(uint16_t *aux) { return 6; }
uint8_t checkVarType(uint32_t *aux) { return 7; }
uint8_t checkVarType(double *aux) { return 8; }
uint8_t checkVarType(char *aux) { return 9; }