/******************************************************************************
 * task_BMSUART.h
 * Header for task_BMSUART.cpp
 *****************************************************************************/
#ifndef __TASK_BMSUART_H__
#define __TASK_BMSUART_H__

#include <Arduino.h>

#include "global_Constants.h"
#include "global_Variables.h"
#include "func_BMS.h"
#include "func_Misc.h"

// Code for the task to get data from the BMS
void taskCode_BMSUART(void *pvParameters);

// This function keep tracking the amount of bad packets received before give the error of connection with the BMS
// If the parameter is false or not informed, the counter is increased. If it is true, it'll restart the counter
void BMSBadDataCounter(bool reset = false);

#endif