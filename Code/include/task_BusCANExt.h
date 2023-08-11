/******************************************************************************
 * task_BusCANExt.h
 * Header for task_BusCANExt.cpp
 *****************************************************************************/
#ifndef __TASK_BUSCANEXT_H__
#define __TASK_BUSCANEXT_H__

#include <Arduino.h>
#include <ACAN2515.h>

#include "global_Constants.h"
#include "global_Variables.h"
#include "func_BMS.h"
#include "func_MCU.h"
#include "func_Charger.h"
#include "func_HMI.h"

// Code for the task CANBus
void taskCode_BusCANExt(void *pvParameters);

// Function that monitors CAN connection times
void CANExtWatchDog();

#endif