/******************************************************************************
 * global_Variables.h
 * Variables that will be used in the whole project
 * The values are in the global_Variables.cpp. There are here only the declarations.
 *****************************************************************************/
#ifndef __GLOBAL_VARIABLES_H__
#define __GLOBAL_VARIABLES_H__

#include <Arduino.h>
#include <Wire.h>
#include <BoolArray.h>
#include <ACAN2515.h>
#include <ESP32CAN.h>
#include <BluetoothSerial.h>
#include <Ticker.h>

#include "global_Constants.h"
#include "global_Structures.h"
#include "class_CANRecv.h"
#include "class_CANSend.h"
#include "class_Math.h"
#include "class_ParamFRAM.h"
#include "class_TaskManager.h"

// Let the compiler know such a class will be defined
// Necessary to avoid error messages
class CANRecv;
class CANSend;
class Math;
class ParamFRAM;

// Pins that change according to the board
extern gpio_num_t pinTXCAN2; // CAN2_TXD - TX from embedded CAN Controller
extern gpio_num_t pinRXCAN2; // CAN2_RXD - RX from embedded CAN Controller
extern uint8_t pinHSPI_MOSI; // HSPI_MOSI - MOSI for HSPI

extern StructVehicleParam vehicleParam; // Parameters for the vehicle
extern StructVehicleInfo vehicleInfo;   // Information from the vehicle
extern StructMCU MCUData;               // All data acquired from MCU
extern StructCharger chargerData;       // All data acquired from charger
extern StructBMS BMSData;               // All data acquired from BMS
extern StructTime timeNow;              // Updated time. Updated by the task UpdateTime.
extern StructTime timeToUpdate;         // New time to be applied. Read by the task UpdateTime.
extern InfoToSave infoToSave;           // Struct with all variables saved in the flash memory
extern StructInputs inputs;             // All the inputs
extern StructOutputs outputs;           // All the outputs

extern HardwareSerial SerialBMS;   // Used to change the Serial1 name
extern HardwareSerial SerialPanel; // Used to change the Serial2 name
extern SPIClass HSPIBus;           // HSPI Class, to be used in the flash memory and inertial sensor
extern SPIClass VSPIBus;           // VSPI Class, to be used in the CAN BUS

extern TwoWire I2C0;                          // Instance for I2C0
extern ParamFRAM paramFRAM[maxNumParameters]; // Array with the objects used to handle information saved in the FRAM. PAY ATTENTION TO THE PARAMETER CODES
extern bool flagToReadFromFRAM;               // Used to indicates that the task BusI2C needs to read all data again. IT CANNOT BE READ ANYTIME, JUST IN THE TASK, IT'LL NOT WORK
extern bool flagTimeToUpdate;                 // Indicates to the task UpdateTime that there is a time update scheduled
extern bool flagChangeRidingMode;             // Indicates that the rider wants to change the riding mode using the button

// Used to know when the last pulse from the speed sensor happened
// This variable is just used when there is transmission and the RPM is coming from the speed sensor
extern uint32_t lastSpeedSensorPulse;

// Ticker used to generates a heartbeat in the LED on the ECU
extern Ticker tickHeartbeat;

// This variable is just used when the bike has transmition. In this case the speed is measured using the speed sensor
// Each pulse from a magnet causes a interruption and the time between pulses is used to calculates the RPM
// When the bike will be stopped, no pulse will happen, so the speed will never be 0, so a ticker is used to force 0 after a time without pulses
extern Ticker tickForceRPMTo0;

// Array that enables or disables the debug serial messages
// It's 32 flags, that will be stored in the FRAM Memory using 4 bytes
extern BoolArray serialDebugFlags;

// Array thay will store the errors
extern BoolArray errorArray;

// Keep the error array from the time that the messages start to show in the display
// To force to be shown at least one time, even if the error is not active anymore
extern BoolArray errorArrayTemp;

// Dummy variables, used when the type of variable is more important than the value
/* #region DUMMY */
extern int8_t dummyInt8;
extern int16_t dummyInt16;
extern int32_t dummyInt32;
extern uint8_t dummyUint8;
extern uint16_t dummyUint16;
extern uint32_t dummyUint32;
extern double dummyDouble;
/* #endregion */

// Objects for average calculations
/* #region Average calculation objects */
extern Math avgBMSTemp;          // Used for displaying BMS temperature in the display without flickering
extern Math avgMotorTemp;        // Used for displaying motor temperature in the display without flickering
extern Math avgMCUVoltage;       // Used to calculate the MCU voltage average within 20 seconds to calculate efficiency
extern Math avgMCUCurrentOut;    // Used to smooth the currentOut when the riding mode is descreased
extern Math avgECUSupplyVoltage; // Used to stabilize the reading of the analog input
extern Math avgEfficiency;       // Used to calculates the average efficiency within 5 seconds
/* #endregion */

// Objects for power reduction calculations
/* #region PwrReduction objects */
extern Math pwrRedFetTemp;
extern Math pwrRedBatTemp;
extern Math pwrRedMotorTemp;
/* #endregion */

//Used to calculate regen by speed
extern Math regenSpeed;

/* #region TASK_HANDLES */
extern TaskManager task_Bluetooth;
extern TaskManager task_BLE;
extern TaskManager task_BusCANExt;
extern TaskManager task_BusCANEmb;
extern TaskManager task_BusI2C;
extern TaskManager task_Errors;
extern TaskManager task_InertialSensor;
extern TaskManager task_ModeSelector;
extern TaskManager task_BMSUART;
extern TaskManager task_ScreenUART;
extern TaskManager task_UpdateIO;
/* #endregion */

/*#region CAN*/
// Used in external CAN
extern ACAN2515 CANExt;                // Last argument is 255 -> no interrupt pin
extern CANMessage CANExtReceiveFrame;  // The object that holds the data for incoming External CAN line packets.
extern CANMessage CANExtTransmitFrame; // The object that holds data for outgoing External CAN line packets.

// Used in embedded CAN
extern int CANEmbRXQueueSize;           // Receive Queue size
extern CAN_device_t CANEmbCgf;          // CAN Config
extern CAN_frame_t CANEmbReceiveFrame;  // The object that holds the data for incoming Embedded CAN line packets.
extern CAN_frame_t CANEmbTransmitFrame; // The object that holds data for outgoing Embedded CAN line packets.

extern CANRecv MCUAPTRecv1; // CAN Object used to receive the packet 1 from the MCU
extern CANRecv MCUAPTRecv2; // CAN Object used to receive the packet 2 from the MCU
extern CANRecv chargerRecv; // CAN Object used to receive the packet from the Charger
extern CANRecv BMSRecv1;    // CAN Object used to receive the packet 1 from the BMS
extern CANRecv BMSRecv2;    // CAN Object used to receive the packet 2 from the BMS
extern CANRecv BMSRecv3;    // CAN Object used to receive the packet 3 from the BMS
extern CANRecv BMSRecv4;    // CAN Object used to receive the packet 4 from the BMS
extern CANRecv BMSRecv5;    // CAN Object used to receive the packet 5 from the BMS
extern CANRecv BMSRecv6;    // CAN Object used to receive the packet 6 from the BMS
extern CANRecv BMSRecv7;    // CAN Object used to receive the packet 7 from the BMS
extern CANRecv BMSRecv8;    // CAN Object used to receive the packet 8 from the BMS
extern CANRecv BMSRecv9;    // CAN Object used to receive the packet 9 from the BMS
extern CANRecv BMSRecv10;   // CAN Object used to receive the packet 10 from the BMS
extern CANRecv BMSRecv11;   // CAN Object used to receive the packet 11 from the BMS
extern CANRecv BMSRecv12;   // CAN Object used to receive the packet 12 from the BMS
extern CANRecv BMSRecv13;   // CAN Object used to receive the packet 13 from the BMS
extern CANRecv BMSRecv14;   // CAN Object used to receive the packet 14 from the BMS
extern CANRecv BMSRecv15;   // CAN Object used to receive the packet 15 from the BMS
extern CANRecv BMSRecv16;   // CAN Object used to receive the packet 16 from the BMS
extern CANRecv BMSRecv17;   // CAN Object used to receive the packet 17 from the BMS

extern CANSend MCUAPTSend;  // CAN Object used to send packet to the MCU
extern CANSend chargerSend; // CAN Object used to send packet to the Charger

extern CANSend HMISend1;  // CAN Object used to send packet 1 to the HMI
extern CANSend HMISend2;  // CAN Object used to send packet 2 to the HMI
extern CANSend HMISend3;  // CAN Object used to send packet 3 to the HMI
extern CANSend HMISend4;  // CAN Object used to send packet 4 to the HMI
/*#endregion*/

extern bool tempIcon;   //Used to display the high temperature icon

#endif