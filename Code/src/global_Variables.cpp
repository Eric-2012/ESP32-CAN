/******************************************************************************
 * global_Variables.cpp
 * Variables that will be used in the whole project
 *****************************************************************************/

#include "global_Variables.h"

// Pins that change according to the board
// They are set in the main file, after reading the parameters
gpio_num_t pinTXCAN2; // CAN2_TXD - TX from embedded CAN Controller
gpio_num_t pinRXCAN2; // CAN2_RXD - RX from embedded CAN Controller
uint8_t pinHSPI_MOSI; // HSPI_MOSI - MOSI for HSPI

StructVehicleParam vehicleParam; // Parameters from the vehicle
StructVehicleInfo vehicleInfo;   // Information from the vehicle
StructMCU MCUData;               // All data acquired from MCU
StructCharger chargerData;       // All data acquired from charger
StructBMS BMSData;               // All data acquired from BMS
StructTime timeNow;              // Updated time. Updated by the task BusI2C.
StructTime timeToUpdate;         // New time to be applied. Read by the task BusI2C.
InfoToSave infoToSave;           // Struct with all variables saved in the flash memory
StructInputs inputs;             // All the inputs
StructOutputs outputs;           // All the outputs

HardwareSerial SerialBMS(1);   // Used to change the Serial1 name
HardwareSerial SerialPanel(2); // Used to change the Serial2 name
SPIClass HSPIBus(HSPI);        // HSPI Class, to be used in the flash memory and inertial sensor
SPIClass VSPIBus(VSPI);        // VSPI Class, to be used in the CAN BUS

TwoWire I2C0 = TwoWire(0);             // Instance for I2C0
ParamFRAM paramFRAM[maxNumParameters]; // Array with the objects used to handle information saved in the FRAM. PAY ATTENTION TO THE PARAMETER CODES
bool flagToReadFromFRAM = false;       // Used to indicates that the task BusI2C needs to read all data again. IT CANNOT BE READ ANYTIME, JUST IN THE TASK, IT'LL NOT WORK
bool flagTimeToUpdate = false;         // Indicates to the task UpdateTime that there is a time update scheduled
bool flagChangeRidingMode = false;     // Indicates that the rider wants to change the riding mode using the button

// Used to know when the last pulse from the speed sensor happened
// This variable is just used when there is transmission and the RPM is coming from the speed sensor
uint32_t lastSpeedSensorPulse;

// Ticker used to generates a heartbeat in the LED on the ECU
Ticker tickHeartbeat;

// This variable is just used when the bike has transmition. In this case the speed is measured using the speed sensor
// Each pulse from a magnet causes a interruption and the time between pulses is used to calculates the RPM
// When the bike will be stopped, no pulse will happen, so the speed will never be 0, so a ticker is used to force 0 after a time without pulses
Ticker tickForceRPMTo0;

// Array that enables or disables the debug serial messages
// It's 32 flags, that will be stored in the FRAM Memory using 4 bytes
BoolArray serialDebugFlags;

// Array thay will store the errors
BoolArray errorArray;

// Keep the error array from the time that the messages start to show in the display
// To force to be shown at least one time, even if the error is not active anymore
BoolArray errorArrayTemp;

// Dummy variables, used when the type of variable is more important than the value
/* #region DUMMY */
int8_t dummyInt8 = 0;
int16_t dummyInt16 = 0;
int32_t dummyInt32 = 0;
uint8_t dummyUint8 = 0;
uint16_t dummyUint16 = 0;
uint32_t dummyUint32 = 0;
double dummyDouble = 0;
/* #endregion */

// Objects for average calculations
/* #region Average calculation objects */
Math avgBMSTemp;          // Used for displaying BMS temperature in the display without flickering
Math avgMotorTemp;        // Used for displaying motor temperature in the display without flickering
Math avgMCUVoltage;       // Used to calculate the MCU voltage average within 20 seconds to calculate efficiency
Math avgMCUCurrentOut;    // Used to smooth the currentOut when the riding mode is descreased
Math avgECUSupplyVoltage; // Used to stabilize the reading of the analog input
Math avgEfficiency;       // Used to calculates the average efficiency within 5 seconds
/* #endregion */

// Objects for power reduction calculations
/* #region PwrReduction objects */
Math pwrRedFetTemp;
Math pwrRedBatTemp;
Math pwrRedMotorTemp;
/* #endregion */

//Used to calculate regen by speed
Math regenSpeed;

/* #region TaskManager objects */
TaskManager task_Bluetooth;
TaskManager task_BLE;
TaskManager task_BusCANExt;
TaskManager task_BusCANEmb;
TaskManager task_BusI2C;
TaskManager task_Errors;
TaskManager task_InertialSensor;
TaskManager task_ModeSelector;
TaskManager task_BMSUART;
TaskManager task_ScreenUART;
TaskManager task_UpdateIO;
/* #endregion */

/*#region CAN*/
// Used in external CAN
ACAN2515 CANExt(pinVSPI_CS_Can, VSPIBus, 255); // Last argument is 255 -> no interrupt pin
CANMessage CANExtReceiveFrame;                 // The object that holds the data for incoming External CAN line packets.
CANMessage CANExtTransmitFrame;                // The object that holds data for outgoing External CAN line packets.

// Used in embedded CAN
int CANEmbRXQueueSize = 30;      // Receive Queue size
CAN_device_t CANEmbCgf;          // CAN Config
CAN_frame_t CANEmbReceiveFrame;  // The object that holds the data for incoming Embedded CAN line packets.
CAN_frame_t CANEmbTransmitFrame; // The object that holds data for outgoing Embedded CAN line packets.

CANRecv MCUAPTRecv1; // CAN Object used to receive the packet 1 from the MCU
CANRecv MCUAPTRecv2; // CAN Object used to receive the packet 2 from the MCU
CANRecv chargerRecv; // CAN Object used to receive the packet from the Charger
CANRecv BMSRecv1;    // CAN Object used to receive the packet 1 from the BMS
CANRecv BMSRecv2;    // CAN Object used to receive the packet 2 from the BMS
CANRecv BMSRecv3;    // CAN Object used to receive the packet 3 from the BMS
CANRecv BMSRecv4;    // CAN Object used to receive the packet 4 from the BMS
CANRecv BMSRecv5;    // CAN Object used to receive the packet 5 from the BMS
CANRecv BMSRecv6;    // CAN Object used to receive the packet 6 from the BMS
CANRecv BMSRecv7;    // CAN Object used to receive the packet 7 from the BMS
CANRecv BMSRecv8;    // CAN Object used to receive the packet 8 from the BMS
CANRecv BMSRecv9;    // CAN Object used to receive the packet 9 from the BMS
CANRecv BMSRecv10;   // CAN Object used to receive the packet 10 from the BMS
CANRecv BMSRecv11;   // CAN Object used to receive the packet 11 from the BMS
CANRecv BMSRecv12;   // CAN Object used to receive the packet 12 from the BMS
CANRecv BMSRecv13;   // CAN Object used to receive the packet 13 from the BMS
CANRecv BMSRecv14;   // CAN Object used to receive the packet 14 from the BMS
CANRecv BMSRecv15;   // CAN Object used to receive the packet 15 from the BMS
CANRecv BMSRecv16;   // CAN Object used to receive the packet 16 from the BMS
CANRecv BMSRecv17;   // CAN Object used to receive the packet 17 from the BMS

CANSend MCUAPTSend;  // CAN Object used to send packet to the MCU
CANSend chargerSend; // CAN Object used to send packet to the Charger

CANSend HMISend1; // CAN Object used to send packet 1 to the HMI
CANSend HMISend2; // CAN Object used to send packet 2 to the HMI
CANSend HMISend3; // CAN Object used to send packet 3 to the HMI
CANSend HMISend4; // CAN Object used to send packet 4 to the HMI

bool tempIcon = 0;  //Used to display the high temperature icon

/*#endregion*/