/******************************************************************************
 * tasks_Bluetooth.h
 * Header for tasks_Bluetooth.cpp
 *****************************************************************************/
#ifndef __TASKS_BLUETOOTH_H__
#define __TASKS_BLUETOOTH_H__

#include <Arduino.h>
#include <BluetoothSerial.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <rom/rtc.h>

#include "global_Constants.h"
#include "global_Variables.h"
#include "func_Misc.h"
#include "func_BusHSPI.h"

// Code for the task that takes care of classic Bluetooth
void taskCode_Bluetooth(void *pvParameters);

// Code for the task that takes care of Buetooth low energy
void taskCode_BLE(void *pvParameters);

// Prints the struct infoToSave via bluetooth
void printDebugViaBluetooth();

#endif