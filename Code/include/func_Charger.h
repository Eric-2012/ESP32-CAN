/******************************************************************************
 * func_Charger.h
 * Header for func_Charger.cpp
 *****************************************************************************/
#ifndef __FUNC_CHARGER_H__
#define __FUNC_CHARGER_H__

#include <Arduino.h>

#include "global_Constants.h"
#include "global_Variables.h"
#include "func_Misc.h"

// Analyzes the charger package
void chargerPackAnalysis(uint8_t *CANDataArray);

// Prepares the CAN packet to send via can to the Charger
void prepareCANChargerOut();

#endif