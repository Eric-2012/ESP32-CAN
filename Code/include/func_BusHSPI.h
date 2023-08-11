/******************************************************************************
 * func_BusHSPI.h
 * Header for func_BusHSPI.cpp
 *****************************************************************************/
#ifndef __FUNC_BUSHSPI__
#define __FUNC_BUSHSPI__

#include <Arduino.h>
#include <rom/rtc.h>

#include "global_Constants.h"
#include "global_Variables.h"
#include "func_BMS.h"

// Populates the struct infoToSave
void prepareInfoToSave();

#endif