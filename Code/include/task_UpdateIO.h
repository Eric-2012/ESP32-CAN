/******************************************************************************
 * task_UpdateIO.h
 * Header for task_UpdateIO.cpp
 *****************************************************************************/
#ifndef __TASK_UPDATEIO_H__
#define __TASK_UPDATEIO_H__

#include <Arduino.h>

#include "global_Constants.h"
#include "global_Variables.h"
#include "func_Misc.h"
#include "func_Interrupts.h"
#include "task_ScreenUART.h"

// Code for the task UpdateIO
void taskCode_UpdateIO(void *pvParameters);

#endif