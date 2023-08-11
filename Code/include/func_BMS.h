/******************************************************************************
 * func_BMS.h
 * Header for func_BMS.cpp
 *****************************************************************************/
#ifndef __FUNC_BMS__
#define __FUNC_BMS__

#include <Arduino.h>

#include "global_Constants.h"
#include "global_Variables.h"
#include "func_Misc.h"

// Calculates the average battery temperature and smooth it
void calcAvgBatteryTemperature();

// Calculates the average FET temperature
void calcAvgFETTemperature();

// Calculates the effiency
void calcEfficiency();

// Converts the cell voltages of BMS data from double to uint8_t
// Voltage of each cell. Value 0 = 3V. Incrementing of 0.01V each bit increment.
uint8_t cellVoltageToSaveInFlash(double cellVoltage);

// Analyzes the packet 1 received from the BMS via CAN (UART2BMS Converter)
void BMSPack1Analysis(uint8_t *CANDataArray);

// Analyzes the packet 2 received from the BMS via CAN (UART2BMS Converter)
void BMSPack2Analysis(uint8_t *CANDataArray);

// Analyzes the packet 3 received from the BMS via CAN (UART2BMS Converter)
void BMSPack3Analysis(uint8_t *CANDataArray);

// Analyzes the packet 4 received from the BMS via CAN (UART2BMS Converter)
void BMSPack4Analysis(uint8_t *CANDataArray);

// Analyzes the packet 5 received from the BMS via CAN (UART2BMS Converter)
void BMSPack5Analysis(uint8_t *CANDataArray);

// Analyzes the packet 6 received from the BMS via CAN (UART2BMS Converter)
void BMSPack6Analysis(uint8_t *CANDataArray);

// Analyzes the packet 7 received from the BMS via CAN (UART2BMS Converter)
void BMSPack7Analysis(uint8_t *CANDataArray);

// Analyzes the packet 8 received from the BMS via CAN (UART2BMS Converter)
void BMSPack8Analysis(uint8_t *CANDataArray);

// Analyzes the packet 9 received from the BMS via CAN (UART2BMS Converter)
void BMSPack9Analysis(uint8_t *CANDataArray);

// Analyzes the packet 10 received from the BMS via CAN (UART2BMS Converter)
void BMSPack10Analysis(uint8_t *CANDataArray);

// Analyzes the packet 11 received from the BMS via CAN (UART2BMS Converter)
void BMSPack11Analysis(uint8_t *CANDataArray);

// Analyzes the packet 12 received from the BMS via CAN (UART2BMS Converter)
void BMSPack12Analysis(uint8_t *CANDataArray);

// Analyzes the packet 13 received from the BMS via CAN (UART2BMS Converter)
void BMSPack13Analysis(uint8_t *CANDataArray);

// Analyzes the packet 14 received from the BMS via CAN (UART2BMS Converter)
void BMSPack14Analysis(uint8_t *CANDataArray);

// Analyzes the packet 15 received from the BMS via CAN (UART2BMS Converter)
void BMSPack15Analysis(uint8_t *CANDataArray);

// Analyzes the packet 16 received from the BMS via CAN (UART2BMS Converter)
void BMSPack16Analysis(uint8_t *CANDataArray);

// Analyzes the packet 17 received from the BMS via CAN (UART2BMS Converter)
void BMSPack17Analysis(uint8_t *CANDataArray);

#endif