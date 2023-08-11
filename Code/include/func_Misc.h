/******************************************************************************
 * func_Misc.h
 * Header for func_Misc.cpp
 *****************************************************************************/
#ifndef __FUNC_MISC_H__
#define __FUNC_MISC_H__

#include <Arduino.h>

#include "global_Constants.h"
#include "global_Variables.h"

// Put reset pin to HIGH
void activatePeripheralComponents();

// Put reset pin to LOW
void disablePeripheralComponents();

// Convert degrees Celsius to degrees Fahrenheit
double convertCelsiusToFahrenheit(double celsius);

// Convert km to miles
double convertKmToM(double km);

// Convert Km/h to Mph
double convertKmhToMph(double kmh);

// Receives 2 bytes and return an unsigned int with 16 bits
uint16_t uint8To_Uint16(uint8_t byte1, uint8_t byte0);

// Receives 4 bytes and return an unsigned int with 32 bits
uint32_t uint8To_Uint32(uint8_t byte3, uint8_t byte2, uint8_t byte1, uint8_t byte0);

// Receives 2 bytes and return a signed int with 16 bits
int16_t uint8To_Int16(uint8_t byte1, uint8_t byte0);

// Receives 4 bytes and return a signed int with 32 bits
int32_t uint8To_Int32(uint8_t byte3, uint8_t byte2, uint8_t byte1, uint8_t byte0);

// Receive the actual state from a variable and check if it was up the minimal time to be used as high
void debounce(bool actualState, uint8_t interval, int8_t *count, bool *input);

// Return the variable type
uint8_t checkVarType(bool *aux);
uint8_t checkVarType(int8_t *aux);
uint8_t checkVarType(int16_t *aux);
uint8_t checkVarType(int32_t *aux);
uint8_t checkVarType(uint8_t *aux);
uint8_t checkVarType(uint16_t *aux);
uint8_t checkVarType(uint32_t *aux);
uint8_t checkVarType(double *aux);
uint8_t checkVarType(char *aux);

#endif