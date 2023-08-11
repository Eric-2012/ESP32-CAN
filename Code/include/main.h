/******************************************************************************
 * main.h
 * Header for the main file
 *****************************************************************************/
#ifndef __MAIN_H__
#define __MAIN_H__

#include <Arduino.h>
#include <ACAN2515.h>

#include "global_Constants.h"
#include "global_Variables.h"
#include "func_BusI2C.h"
#include "func_Interrupts.h"
#include "func_Misc.h"
#include "func_SerialEvents.h"
#include "class_Math.h"

#include "task_BusCANExt.h"
#include "task_BusHSPI.h"
#include "task_BusI2C.h"
#include "task_Errors.h"
#include "task_ModeSelector.h"
#include "task_BMSUART.h"
#include "task_ScreenUART.h"
#include "task_UpdateIO.h"
#include "tasks_Bluetooth.h"

void serialEvent();

#endif