/******************************************************************************
 * func_Expander.h
 * Header for func_Expander.cpp
 *****************************************************************************/
#ifndef __FUNC_EXPANDER_H__
#define __FUNC_EXPANDER_H__

#include <Arduino.h>
#include <Wire.h>

#include "global_Constants.h"
#include "global_Variables.h"
#include "func_Misc.h"

// Configure the I2C expanders
void configExpanders();

// Force 0 to I2C expanders pins
void configExpandersForce0();

// Update inputs and outputs
void updateIOs();

#endif