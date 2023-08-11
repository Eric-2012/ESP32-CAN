/******************************************************************************
 * task_BusI2C.h
 * Header for task_SaveToMemories.cpp
 *****************************************************************************/
#ifndef __TASK_BUSI2C_H__
#define __TASK_BUSI2C_H__

#include <Arduino.h>

#include "global_Constants.h"
#include "global_Variables.h"

#include "class_ParamFRAM.h"
#include "func_BusI2C.h"
#include "func_Expander.h"
#include "func_RTC.h"

// Code for the task BusI2C
void taskCode_BusI2C(void *pvParameters);

#endif