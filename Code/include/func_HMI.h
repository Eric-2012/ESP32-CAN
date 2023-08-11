/******************************************************************************
 * func_HMI.h
 * Header for func_HMI.cpp
 *****************************************************************************/

#ifndef __FUNC_HMI_H__
#define __FUNC_HMI_H__

#include <Arduino.h>

#include "global_Constants.h"
#include "global_Variables.h"
#include "func_Errors.h"
#include "func_Misc.h"


// Prepares the 1 CAN packet to send via can to the HMI
void prepareCANHMI1Out();

// Prepares the 2 CAN packet to send via can to the HMI
void prepareCANHMI2Out();

// Prepares the 3 CAN packet to send via can to the HMI
void prepareCANHMI3Out();

// Prepares the 4 CAN packet to send via can to the HMI
void prepareCANHMI4Out();

uint16_t HexToDex(uint16_t auxE);

#endif