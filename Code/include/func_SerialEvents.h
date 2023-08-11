/******************************************************************************
 * func_SerialEvents.h
 * Header for func_SerialEvents.cpp
 *****************************************************************************/
#ifndef __TASK_SERIALEVENTS_H__
#define __TASK_SERIALEVENTS_H__

#include <Arduino.h>

#include "global_Constants.h"
#include "global_Variables.h"
#include "func_BusI2C.h"

// Handle the serial events
void serialEvent();

// Decrypt the reading array and verify if is valid
bool decryptNVerifySerialArray();

// Send the parameters report via serial
void sendParametersReport();

#endif