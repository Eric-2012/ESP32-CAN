/******************************************************************************
 * global_Constants.h
 * Constants that will be used in the whole project
 *****************************************************************************/
#ifndef __GLOBAL_CONSTANTS_H__
#define __GLOBAL_CONSTANTS_H__

#include <Arduino.h>

/* #region ADDR_I2C_BUS */
// Keep int to not have warning messages
const int addrI2C_Outputs = 0x20; // I2C address from I2C Expander for OUTPUTS (MCP23017)
const int addrI2C_Inputs = 0x24;  // I2C address from I2C Expander for INPUTS (MCP23017)
const int addrI2C_FRAM = 0x51;    // I2C address from external FRAM Memory (MB85RC)
const int addrI2C_RTC = 0x68;     // I2C address from real time clock (DS3231)
/* #endregion */

/* #region PINS */
// Some pins change between different boards, these pins should be considered as variables, because it depends on parameters
// CHECK THE PCB CHANGELOG

// Generical pins
const uint8_t pinReset = 33;       // Reset_pin - It is used as reset input in the components that have this pin
const uint8_t pinSpeedSensor = 15; // Speed_sensor - Pin where the speed sensor is connected, it generates an interruption
const uint8_t pin12VMonitor = 34;  // 12VMonitor - Used to measure the 12V Battery

// Pins related to serial ports
const uint8_t pinTXD = 1; // TXD - TX from standard serial port
const uint8_t pinRXD = 3; // RXD - RX from standard serial port

const uint8_t pinTXD1 = 4;  // TXD1 - TX from serial port 1
const uint8_t pinRXD1 = 36; // RXD1 (Sensor VP) - RX from serial port 1

const uint8_t pinTXD2 = 17; // TXD1 - TX from serial port 2
const uint8_t pinRXD2 = 16; // RXD2 - RX from serial port 2

// Pins related to HSPI
const uint8_t pinHSPI_CS_Acc = 5;    // HSPI_CS1 - Chip select 1 for HSPI (ACCELEROMETER)
const uint8_t pinHSPI_CS_Flash = 26; // HSPI_CS2 - Chip select 2 for HSPI (FLASH MEMORY)
// The HSPI_MOSI changes between boards, so it is a variable. The value is set in the main file, after reading the parameters
const uint8_t pinHSPI_MISO = 12;     // HSPI_MISO - MISO for HSPI
const uint8_t pinHSPI_CLK = 14;      // HSPI_CLK - Clock for HSPI
const uint8_t pinINT_Acc = 39;       // MPU9250_INT (Sensor VN) - Interruption from the Accelerometer

// Pins related to VSPI
const int8_t pinVSPI_CS_Can = 25; // VSPI_CS1 - Chip select 1 for VSPI (CAN CONTROLLER)
const int8_t pinVSPI_MOSI = 23;   // VSPI_MOSI - MOSI for VSPI
const int8_t pinVSPI_MISO = 19;   // VSPI_MISO - MISO for VSPI
const int8_t pinVSPI_CLK = 18;    // VSPI_CLK - Clock for VSPI

// Pins related to I2C Bus
const uint8_t pinSDA = 21;          // I2C_SDA - Data (SDA) for I2C communication bus
const uint8_t pinSCL = 22;          // I2C_SCL - Clock (SCL) for I2C communication bus
const uint8_t pinWP_FRAM = 27;      // WP_FRAM_pin - Write protection pin in the FRAM Memory
const uint8_t pinINT_Extender = 35; // IO_EX_INT  - Interruption from the I2C Expander used for INPUTS
                                    /* #endregion */

/* #region CAN */
// Charger IDs
const uint32_t addrChargerRecv = 0x18FF50E5;
const uint32_t addrChargerSend = 0x1806E5F4;

// MCU IDs
const uint32_t addrMCUAPTRecv1 = 0x180128F1;
const uint32_t addrMCUAPTRecv2 = 0x180228F1;
const uint32_t addrMCUAPTSend = 0x180238F1;


// HMI IDs
const uint32_t addrHMISend1 = 0x180413F5;
const uint32_t addrHMISend2 = 0x180423F5;
const uint32_t addrHMISend3 = 0x180433F5;
const uint32_t addrHMISend4 = 0x180443F5;

// BMS IDs (Using the UART2CAN Converter)
// AntBMS_CAN IDs 
const uint32_t addrBMSRecv1 = 0x18FF28F4;
const uint32_t addrBMSRecv2 = 0x18FE28F4;
const uint32_t addrBMSRecv3 = 0x18C828F4;
const uint32_t addrBMSRecv4 = 0x18C928F4;
const uint32_t addrBMSRecv5 = 0x18CA28F4;
const uint32_t addrBMSRecv6 = 0x18CB28F4;
const uint32_t addrBMSRecv7 = 0x18CC28F4;
const uint32_t addrBMSRecv8 = 0x18CD28F4;
const uint32_t addrBMSRecv9 = 0x18CE28F4;
const uint32_t addrBMSRecv10 = 0x18B428F4;
const uint32_t addrBMSRecv11 = 0x1806E5F4;
// The below packets are not used for the time being
const uint32_t addrBMSRecv12 = 0x16810112;
const uint32_t addrBMSRecv13 = 0x16810113;
const uint32_t addrBMSRecv14 = 0x16810114;
const uint32_t addrBMSRecv15 = 0x16810115;
const uint32_t addrBMSRecv16 = 0x16810116;
const uint32_t addrBMSRecv17 = 0x16810117;


// Identification of each byte of the CAN packet sent to the motor controller
// Byte 0 and 5 are reserved
const uint8_t MCUAPTSend_byteCurrentOut = 1;      // Max Phase Current - Values from 0 to 256 (Each unit is 4A)
const uint8_t MCUAPTSend_byteStatus = 2;          // Bit 7 Direction (0 Forward, 1 Reverse) || Bit 5 MCU Mode (0 Stop, 1 Run)
const uint8_t MCUAPTSend_byteMinThrottle = 3;     // Throttle Minimum - Values from 0 to 256 (Each unit is 0.0195V)
const uint8_t MCUAPTSend_byteMaxThrottle = 4;     // Throttle Maximum - Values from 0 to 256 (Each unit is 0.0195V)
const uint8_t MCUAPTSend_byteMaxAcceleration = 6; // Max Acceleration - Values from 0 to 200 (Each unit is 1kphs)
const uint8_t MCUAPTSend_byteMaxRegen = 7;        // Max Regen Current - Values from 0 to 256 (Each unit is 2A, so it goes till 512A)

// Identification of each byte of the CAN packet sent to the charger
// Byte 4, 5, 6 and 7 are reserved
const uint8_t chargerSend_byteMaxVoltageHighByte = 0; // High byte of the highest allowable charging terminal voltage MULTIPLIED BY 10
const uint8_t chargerSend_byteMaxVoltageLowByte = 1;  // Low byte of the highest allowable charging terminal voltage MULTIPLIED BY 10
const uint8_t chargerSend_byteMaxCurrentHighByte = 2; // High byte of maximum allowable charging current MULTIPLIED BY 10
const uint8_t chargerSend_byteMaxCurrentLowByte = 3;  // Low byte of maximum allowable charging current MULTIPLIED BY 10


// Identification of each byte of the CAN packet sent to the HMI

// Packet 1 
// (byte 7 reserved)

const uint8_t HMISend1_byteSpeedLowByte= 0;
const uint8_t HMISend1_byteSpeedHighByte= 1;
const uint8_t HMISend1_byteVoltageLowByte= 2;
const uint8_t HMISend1_byteVoltageHighByte= 3;
const uint8_t HMISend1_byteCurrentLowByte= 4;
const uint8_t HMISend1_byteCurrentHighByte= 5;
const uint8_t HMISend1_byteSpeed_UnitSettings= 6;


// Packet 2
// (byte 7 reserved)
const uint8_t HMISend2_byteBMS_avg_temp = 0;
const uint8_t HMISend2_byteMoto_temp  = 1;
const uint8_t HMISend2_byteController_temp = 2;
const uint8_t HMISend2_byteTrip_meter= 3;
const uint8_t HMISend2_byteSOC = 4;
const uint8_t HMISend2_byteRemaining_RangeLowByte = 5;
const uint8_t HMISend2_byteRemaining_RangeHighByte = 6;

// Packet 3
// (byte 7 reserved)
const uint8_t HMISend3_byteIconSignalLowByte = 0;
const uint8_t HMISend3_byteIconSignalMidByte = 1;
const uint8_t HMISend3_byteIconSignalHighByte = 2;
const uint8_t HMISend3_byteRidersMode = 3;
const uint8_t HMISend3_byteOdometerLowByte = 4;
const uint8_t HMISend3_byteOdometerMidByte = 5;
const uint8_t HMISend3_byteOdometerHighByte = 6;


// Packet 4
// (byte 3,4,5,6,7 reserved)
/*
const uint8_t HMISend4_byteClockLowByte = 0;
const uint8_t HMISend4_byteClockHighByte = 1;
const uint8_t HMISend4_byteAmbient_Temperature = 2;
*/  // wzh20230517 note  
// Packet 4  // Errors Frame

const uint8_t HMISend4_byteError1LowByte= 0;
const uint8_t HMISend4_byteError1HighByte= 1;
const uint8_t HMISend4_byteError2LowByte= 2;
const uint8_t HMISend4_byteError2HighByte= 3;
const uint8_t HMISend4_byteError3LowByte= 4;
const uint8_t HMISend4_byteError3HighByte= 5;
const uint8_t HMISend4_byteError4LowByte= 6;
const uint8_t HMISend4_byteError4HighByte= 7;

/* #endregion */

/* #region POWER ADJUSTMENTS */
// Number that represents the percentage of the maximum speed and it is used as divisions to calculate the current output
const uint8_t currentOutDiv[] = {0, 8, 15, 23, 31, 38, 46, 54, 62, 69, 85, 92, 100, 108, 112, 119, 192};

// Current output for each riding mode
const uint16_t currentOutEcoMode[] = {250, 220, 200, 170, 150, 110, 90, 80, 80, 70, 20, 10, 5, 5, 2, 1, 1};
const uint16_t currentOutCityMode[] = {565, 524, 498, 464, 430, 396, 362, 311, 250, 150, 33, 30, 25, 20, 25, 20, 3};
const uint16_t currentOutProMode[] = {800, 800, 760, 690, 620, 576, 542, 507, 442, 376, 300, 300, 300, 300, 300, 300, 3};
/* #endregion */

/* #region ERRORS */
// BMS TIMEOUT WAS CHANGED TO CITY MODE
// Errors that need to turn off the motor
const uint16_t errorsKeepStopped[] = {1, 2, 3, 4, 5, 11, 12, 13, 15, 20, 200, 201, 203, 205, 206, 207, 210, 211, 212, 302, 304, 305, 310, 311};

// Errors that keep in the CITY mode
const uint16_t errorsKeepCityMode[] = {6, 7, 8, 9, 10, 16, 17, 18, 19, 21, 22, 23, 25, 202, 204, 209, 300};

// Errors that keep in the ECO mode
// Besides keeping in ECO mode, errors 14 and 213 have power limitation of 50%
const uint16_t errorsKeepEcoMode[] = {208, 14, 213};
const uint16_t errors50Power[] = {14, 213};
/* #endregion */

/* #region PARAMETER CODES */
// The maximum possible number of parameters to be registered (number of positions in the array)
const uint16_t maxNumParameters = 100;

// Info
const uint16_t p_btDeviceName = 50;
const uint16_t p_odometerKm = 46;
const uint16_t p_isStored = 47;
const uint16_t p_isLocked = 48;
const uint16_t p_customMaxSpeed = 49;

// Full Vehicle Targets
const uint16_t p_maximumSpeed = 0;
const uint16_t p_rechargingTimeFullCharge = 1;
const uint16_t p_maximumPayload = 2;
const uint16_t p_maxCurbWeight = 3;

// Battery Specification
const uint16_t p_chemistry = 4;
const uint16_t p_nominalCapacity = 5;
const uint16_t p_liionBatterySetup = 6;
const uint16_t p_maximumChargeCurrent = 7;
const uint16_t p_maximumChargeVoltage = 52;
const uint16_t p_BMS = 8;
const uint16_t p_BMSType = 53;
const uint16_t p_thermalManagement = 9;
const uint16_t p_doubleCharger = 10;
const uint16_t p_backup12Battery = 25;
const uint16_t p_onboardCharger = 26;

// Motor & Controller Specification
const uint16_t p_continuousPower = 11;
const uint16_t p_peakPower = 12;
const uint16_t p_motorType = 13;
const uint16_t p_ratedSpeed = 14;
const uint16_t p_targetGearRatio = 15;
const uint16_t p_preferredMethodOfCooling = 16;
const uint16_t p_torque = 17;
const uint16_t p_finalDrive = 18;
const uint16_t p_hasOptionToReverse = 19;
const uint16_t p_hasTransmission = 20;

// Misc Electronics & Wiring Specification
const uint16_t p_ECUHardwareVersion = 21;
const uint16_t p_ECUSoftwareVersion = 22;
const uint16_t p_wireHarnessVersion = 23;
const uint16_t p_dcdc = 24;

// Screen Specification
const uint16_t p_multSpeed = 27;
const uint16_t p_speedUnit = 28;
const uint16_t p_temperatureUnit = 51;
const uint16_t p_screenType = 29;

// Frame Specification
const uint16_t p_wheelbase = 30;

// Suspension Specification
const uint16_t p_rakeAngle = 31;
const uint16_t p_trail = 32;

// Wheel, Tire & Brake Specification
const uint16_t p_frontWheel = 33;
const uint16_t p_frontTireSize = 34;
const uint16_t p_frontBrakeCaliper = 35;
const uint16_t p_frontBrakeDisc = 36;
const uint16_t p_rearWheel = 37;
const uint16_t p_rearTireSize = 38;
const uint16_t p_rearBrakeCaliper = 39;
const uint16_t p_rearBrakeDisc = 40;
const uint16_t p_frontTireSensorID = 41;
const uint16_t p_rearTireSensorID = 42;
const uint16_t p_numMagnetsSpeedSensor = 43;
const uint16_t p_wheelRadius = 44;

// For testing
const uint16_t p_ignoreKillswitchKickstand = 45;
/* #endregion */

#endif