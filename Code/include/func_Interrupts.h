/******************************************************************************
 * func_Interrupts.h
 * Header for func_Interrupts.cpp
 *****************************************************************************/
#ifndef __FUNC_INTERRUPT_H__
#define __FUNC_INTERRUPT_H__

#include <Arduino.h>

#include "global_Constants.h"
#include "global_Variables.h"
#include "func_MCU.h"

// If the speed sensor rose
void ifSpeedSensorRose();

// Used for the ticker that generates a heartbeat in the led on the ECU
void heartbeat();

// Used for the ticker that force the RPM to 0 if the bike is stopped when there is trasmission and the rpm comes from the speedsensor
void forceRPMTo0();

#endif