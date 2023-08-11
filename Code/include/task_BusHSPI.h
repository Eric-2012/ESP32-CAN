/******************************************************************************
 * task_BusHSPI.h
 * Header for task_BusHSPI.cpp
 *****************************************************************************/
#ifndef __TASK_BUSHSPI_H__
#define __TASK_BUSHSPI_H__

#include <Arduino.h>
#include <MPU9250.h>
#include <Kalman.h>

#include "global_Constants.h"
#include "global_Variables.h"

void taskCode_BusHSPI(void *pvParameters);

#endif