/******************************************************************************
 * func_MCU.h
 * Header for func_MCU.cpp
 *****************************************************************************/
#ifndef __FUNC_MCU_H__
#define __FUNC_MCU_H__

#include <Arduino.h>

#include "global_Constants.h"
#include "global_Variables.h"
#include "func_Errors.h"
#include "func_Misc.h"

// Analyzes the first MCU package
void MCUPack1Analysis(uint8_t *CANDataArray);

// Analyzes the second MCU package
void MCUPack2Analysis(uint8_t *CANDataArray);

// Prepares the CAN packet to send via can to the MCU
void prepareCANMCUOut();

// Return the max speed, looking at the possible limitations
uint16_t checkMaxSpeed();

// Adjust the current output
uint8_t adjustCurrentOut(uint16_t speedLimit);

// Calculates the speed, odometer and trip meter, based on the RPM
void motionAnalysis();

// Checks power reductions
double checkPowerReduction();

#endif