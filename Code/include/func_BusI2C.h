/******************************************************************************
 * func_BusI2C.h
 * Header for func_BusI2C.cpp
 *****************************************************************************/
#ifndef __FUNC_BUSI2C__
#define __FUNC_BUSI2C__

#include <Arduino.h>

#include "global_Constants.h"
#include "global_Variables.h"
#include "func_Misc.h"

// Configure and read parameters
void configParametersObjects();

// Read all parameters
void readAllParameters();

#endif