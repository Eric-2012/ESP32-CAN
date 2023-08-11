/******************************************************************************
 * taks_Display.h
 * Header for task_SaveToMemories.cpp
 *****************************************************************************/
#ifndef __TASK_DISPLAY_H__
#define __TASK_DISPLAY_H__

#include <Arduino.h>

#include "global_Constants.h"
#include "global_Variables.h"
#include "class_ScreenUARTProtocol.h"
#include "class_ScreenUART.h"
#include "func_Errors.h"
#include "func_Misc.h"

// Code for the task that shows in the display
void taskCode_ScreenUART(void *pvParameters);

// Blink the warning icon
void blinkIconWarning();

// Shows all code errors that are happening at the moment
void showErrorMessages();

// Starts the screen
void startScreen();

// Force an update in the screen each 1 second
void forceUpdate();

#endif