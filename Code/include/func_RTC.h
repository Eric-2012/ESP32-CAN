/******************************************************************************
 * func_RTC.h
 * Header for func_RTC.cpp
 *****************************************************************************/
#ifndef __FUNC_RTC_H__
#define __FUNC_RTC_H__

#include <Arduino.h>
#include <Wire.h>
#include <time.h>

#include "global_Constants.h"
#include "global_Variables.h"
#include "func_Misc.h"

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val);

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val);

// Set the time if necessary and update the variables board temperature and time
void updateRTC();

// Put the new time to be applied in the structure timeToUpdate, then put the variable flagTimeToUpdate to true to indicates that there is an update scheduled
void setTime(uint8_t second, uint8_t minute, uint8_t hour, uint8_t dayW, uint8_t dayM, uint8_t month, uint8_t year);

// Display actual time in the serial port
void displayTime();

#endif