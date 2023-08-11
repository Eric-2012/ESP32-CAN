/******************************************************************************
 * task_BusCANEmb.h
 * Header for task_BusCANEmb.cpp
 *****************************************************************************/
#ifndef __TASK_BUSCANEMB_H__
#define __TASK_BUSCANEMB_H__

#include <Arduino.h>
#include <ESP32CAN.h>

#include "global_Constants.h"
#include "global_Variables.h"

// Code for the task CANBus
void taskCode_BusCANEmb(void *pvParameters);

// Function that monitors CAN connection times
void CANEmbWatchDog();

#endif