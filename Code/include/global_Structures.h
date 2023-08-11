/******************************************************************************
 * global_Structures.h
 * Structures that will be used in the whole project
 *****************************************************************************/

#ifndef __GLOBAL_STRUCTURES_H__
#define __GLOBAL_STRUCTURES_H__

#include <Arduino.h>

// Struct using to handle date and time
struct StructTime
{
	uint8_t second = 0;
	uint8_t minute = 0;
	uint8_t hour = 0;
	uint8_t dayW = 0;
	uint8_t dayM = 0;
	uint8_t month = 0;
	uint8_t year = 0;
};

// Struct holding inputs
struct StructInputs
{
	// Sofware adjusted inputs
	bool headlightHighBeam = false; // REAL input signal
	bool turnLeftSwitch = false;	// REAL input signal
	bool turnRightSwitch = false;	// REAL input signal
	bool modeButton = false;		// REAL input signal
	bool kickstand = false;			// INVERTED input signal
	bool killswitch = false;		// INVERTED input signal
	bool powerFlag = false;			// REAL input signal
	bool key = false;				// REAL input signal
	bool fastChargeSwitch = false;	// REAL input signal
	bool breakSwitch = false;		// Is the break switch active? (from MCU)

	// True when pressed, but with a delay to turn off, because of the changes due to the input blinking relay
	bool turnLeftSwitchDelayed = false;
	bool turnRightSwitchDelayed = false;

	// Physical inputs
	bool in1 = false;
	bool in2 = false;
	bool in3 = false;
	bool in4 = false;
	bool in5 = false;
	bool in6 = false;
	bool in7 = false;
	bool in8 = false;
	bool in9 = false;
	bool in10 = false;
	bool in11 = false;
	bool in12 = false;

	// For debug
	uint16_t inputsI2CDebug = 0;
};

// Struct holding outputs that will be mirrored to be pins
struct StructOutputs
{
	// Sofware outputs
	bool ledHeartbeat = false;

	// Physical outputs
	bool r1 = false;
	bool r2 = false;
	bool rd1 = false;
	bool rd2 = false;
	bool rd3 = false;
	bool rd4 = false;
	bool rd5 = false;
	bool rd6 = false;
	bool rd7 = false;
	bool rd8 = false;
	bool rd9 = false;
	bool led = false;

	// For debug
	uint16_t outputsI2CDebug = 0;
};

// Struct holding parameters informed by computer that will never change by the software
// They are stored in the FRAM memory
struct StructVehicleParam
{
	// Full Vehicle Targets
	uint16_t maximumSpeed;			   // PARAM_COD0 - Maximum speed (kmh) from factory
	uint16_t rechargingTimeFullCharge; // PARAM_COD1 - Estimated recharging time for full charge (seconds)
	uint16_t maximumPayload;		   // PARAM_COD2 - Maximum Payload (kg)
	uint16_t maxCurbWeight;			   // PARAM_COD3 - Max curb weight (kg)

	// Battery Specification
	char chemistry[15];			   // PARAM_COD4 - Chemistry
	double nominalCapacity;		   // PARAM_COD5 - Capacity, Nominal (kwh)
	char liionBatterySetup[15];	   // PARAM_COD6 - Li-ion battery setup
	uint16_t maximumChargeCurrent; // PARAM_COD7 - Maximum charge current (A DC)
	double maximumChargeVoltage;   // PARAM_COD52 - Maximum charge voltage (V)
	char BMS[25];				   // PARAM_COD8 - BMS Model
	uint8_t BMSType;			   // PARAM_COD53 - BMS Type - 0 = UART, 1 = CAN
	char thermalManagement[40];	   // PARAM_COD9 - Thermal management
	bool doubleCharger;			   // PARAM_COD10 - If the vehicle has two chargers. True = yes, False = no
	char backup12Battery[45];	   // PARAM_COD25 - Backup 12v battery
	char onboardCharger[45];	   // PARAM_COD26 - Onboard charger

	// Motor & Controller Specification
	uint16_t continuousPower;		   // PARAM_COD11 - Continuous power (kW)
	uint16_t peakPower;				   // PARAM_COD12 - Peak power (kW)
	char motorType[40];				   // PARAM_COD13 - Motor type
	uint16_t ratedSpeed;			   // PARAM_COD14 - Rated speed (RPM max)
	char targetGearRatio[15];		   // PARAM_COD15 - Target Gear Ratio
	char preferredMethodOfCooling[25]; // PARAM_COD16 - Preferred method of cooling
	uint16_t torque;				   // PARAM_COD17 - Torque (Nm)
	char finalDrive[15];			   // PARAM_COD18 - Final drive
	bool hasOptionToReverse;		   // PARAM_COD19 - True = yes, False = no
	bool hasTransmission;			   // PARAM_COD20 - True = yes, False = no

	// Misc Electronics & Wiring Specification
	char ECUHardwareVersion[14]; // PARAM_COD21 - Hardware Version
	char ECUSoftwareVersion[14]; // PARAM_COD22 - Software Version
	char wireHarnessVersion[14]; // PARAM_COD23 - Wire harness version
	char dcdc[45];				 // PARAM_COD24 - DC-DC

	// Screen Specification
	double multSpeed;		 // PARAM_COD27 - Multiplier used to increse the speed to show in the display
	uint8_t speedUnit;		 // PARAM_COD28 - 0 = km/h, 1 = mph
	uint8_t temperatureUnit; // PARAM_COD51 - 0 = celsius, 1 = fahrenheit
	uint8_t screenType;		 // PARAM_COD29 - 0 = UART, 1 = CAN

	// Frame Specification
	uint16_t wheelbase; // PARAM_COD30 - Wheelbase (mm)

	// Suspension Specification
	uint8_t rakeAngle; // PARAM_COD31 - Rake angle (degree)
	double trail;	   // PARAM_COD32 - Trail (mm)

	// Wheel, Tire & Brake Specification
	char frontWheel[45];		   // PARAM_COD33 - Front wheel
	char frontTireSize[45];		   // PARAM_COD34 - Front tire size
	char frontBrakeCaliper[45];	   // PARAM_COD35 - Front brake caliper
	char frontBrakeDisc[45];	   // PARAM_COD36 - Front brake disc
	char rearWheel[45];			   // PARAM_COD37 - Rear wheel
	char rearTireSize[45];		   // PARAM_COD38 - Rear tire size
	char rearBrakeCaliper[45];	   // PARAM_COD39 - Rear brake caliper
	char rearBrakeDisc[45];		   // PARAM_COD40 - Rear brake disc
	uint16_t frontTireSensorID;	   // PARAM_COD41 - ID of the TPMS sensor in the front tire
	uint16_t rearTireSensorID;	   // PARAM_COD42 - ID of the TPMS sensor in the rear tire
	uint8_t numMagnetsSpeedSensor; // PARAM_COD43 - Number of magnets for the speed sensor (number of pulses for 1 full rotation)
	double wheelRadius;			   // PARAM_COD44 - Wheel radius in centimeter (For speed and distance calculations)

	// For testing
	bool ignoreKillswitchKickstand; // PARAM_COD45 - When true, the inputs killswitch and kickstand will be ignored
};

// Struct holding parameters that the software can change
struct StructVehicleInfo
{
	// Necessary to keep saved in the FRAM
	char btDeviceName[25];	 // PARAM_COD50 - Bluetooth device name
	double odometerKm = 0;	 // PARAM_COD46 - Odometer in kilometers. Updated by the function parseSpeed.
	bool isStored = 0;		 // PARAM_COD47 - True if the storage mode is on, false if not
	bool isLocked = 0;		 // PARAM_COD48 - True if is locked, false if not
	uint16_t customMaxSpeed; // PARAM_COD49 - Custom maximum speed (kmh)

	// Not necessary to keep saved in the FRAM
	int8_t boardTemperature = 0;   // Temperature from RTC device in degrees Celsius. Updated by the task UpdateTime.
	double boardSupplyVoltage = 0; // Voltage that the ECU is being supplied
	double tripKm = 0;			   // Register the trip distance
	double speedKmh = 0;		   // Speed in km/h. Updated by the motionAnalysis function.
	uint16_t rpm = 0;			   // Rotations per minute ALWAYS POSITIVE. Updated by the speed sensor in case the bike has transmission, or the MCU in case doesn't have transmission
	int16_t efficiency = 0;		   // Efficiency in Wh/km, based in an average of 5 seconds of data being saved in the flash memory

	// Status flags
	uint8_t ridingMode = 1;				  // 0 = Eco, 1 = City, 2 = Pro. Starts in City
	bool ridingModeChangedBecauseOfError; // Flag that indicates if the riding mode changed because an error. // If the change happened because an error, it puts this flag as true and come back to the riding mode above if the error disappear.
	bool passedInitCheckup = false;		  // In the beggining ot the task ModeSelector, a checkup is done to decide if the bike can run or not, setting this variable
	bool bluetoothRunning = false;		  // True if the bluetooth was already started, false if not
	bool isInReverseMode = false;		  // True if the bike is in reverse mode, false if not (based on the mode button pressed for an interval)
	bool isFastChargeOn = false;		  // True if the fast charging mode is on, false if not
	bool isChargerConnected = false;	  // True if the charger is connected, false if not
	bool isCharging = false;			  // True if is charging, false if not
	bool Neutral = false;                 // To be used in future for turn on neutral icon in the screen
	bool Positionlight = false;           // To be used in future for turn on position light  icon in the screen
	bool WinterMode = false;              // Used for adjust riding mode and power according with the weather conditions

	// Errors
	bool errorsDetected = false;  // True if errors were detected, false if not
	uint16_t numActiveErrors = 0; // Number of active errors
	uint32_t sumActiveErrors = 0; // Sum of active error codes, used to detect error changes between cycles

	// Inertial sensor
	double roll;
	double pitch;
	double accX, accY, accZ;
	double gyroX, gyroY, gyroZ;
	double magX, magY, magZ;

	// Data from the TPMS sensor
	double frontTirePressure = 0;	 // Pressure in the front tire (psi)
	int8_t frontTireTemperature = 0; // Temperature in the front tire (degrees celsius)
	int16_t frontTireSensorRSSI = 0; // RSSI of the sensor placed in the front tire
	double rearTirePressure = 0;	 // Pressure in the rear tire (psi)
	int8_t rearTireTemperature = 0;	 // Temperature in the rear tire (degrees celsius)
	int16_t rearTireSensorRSSI = 0;	 // RSSI of the sensor placed in the rear tire
};

// Struct holding the BMS data
struct StructBMS
{
	// Calculated parameters =============================================================

	// True if the BMS is working fine, without any timeout
	bool connectionCheck;

	// True if the BMS is already sending data, false if it's necessary to request new data
	bool BMSReceiving = false;

	// The average between the temperature probes from the battery
	// But just using temperature probes that are more than -70 °C and not an overflow (from GEN1 code)
	double avgBatteryTemperature = 0; // Temperatures 2, 3, 4 and 5
	double avgFETTemperature = 0;	  // Temperatures 0 and 1

	// Used to calculate power consumption
	double powerConsumption = 0.0;

	// Parameters from the BMS ===========================================================

	// Battery voltage - 0.000 V (bytes 4-5) DIVIDED BY 10
	double voltage = 0;

	// Cells voltage - 0.000 V (bytes 6-69) DIVIDED BY 1.000
	double cellsVoltage[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							   0, 0, 0, 0, 0, 0, 0, 0};

	// Battery current - 0.0 A (bytes 70-73) DIVIDED BY 10
	double current = 0;

	// STATE OF CHARGE -> Remaining battery percentage - (byte 74)
	uint8_t SOC = 0;

	// Battery physical capacity - 0.000000 AH (bytes 75-78) DIVIDED BY 1.000.000
	double batteryPhysicalCapacity = 0;

	// Battery remaining capacity - 0.000000 AH (bytes 79-82) DIVIDED BY 1.000.000
	double batteryRemainingCapacity = 0;

	// Total used capacity - 0.000 AH (bytes 83-86) DIVIDED BY 1.000
	double totalUsedCapacity = 0;

	// Accumulated time from power on seconds - 0 seconds (bytes 87-90)
	uint32_t turnOnTime = 0;

	// Temperature probes - Celsius (bytes 91-102)
	int16_t temperatures[6] = {0, 0, 0, 0, 0, 0};

	/* Charging MOSFET status flag - (byte 103)
	* Value 0: FET off
	* Value 1: FET on
	* Value 2: Overvoltage protection
	* Value 3: Overcurrent protection
	* Value 4: Fully charged
	* Value 5: Pack overvoltage
	* Value 6: Battery over temperature
	* Value 7: FET over temperature
	* Value 8: Abnormal current
	* Value 9: Pack out of balance
	* Value 10: Motherboard overheat
	* Value 13: FET error
	* Value 15: Manually closed
	*/
	uint8_t chargingMosfetStatus = 0;

	/* Discharging MOSFET status flag - (byte 104)
	* Value 0: FET off
	* Value 1: FET on
	* Value 2: Overvoltage protection
	* Value 3: Overcurrent protection
	* Value 5: Pack under voltage
	* Value 6: Battery over temperature
	* Value 7: FET over temperature
	* Value 8: Abnormal current
	* Value 9: Pack out of balance
	* Value 10: Motherboard overheat
	* Value 11: Charging on
	* Value 12: Short circuit protection
	* Value 13: FET error
	* Value 14: Abnormal startup
	* Value 15: Manually closed
	*/
	uint8_t dischargingMosfetStatus = 0;

	/* Balance status - (byte 105)
	* Value 0: Off
	* Value 1: Pack imbalanced
	* Value 2: Pack charging imbalanced
	* Value 3: Balance over temperature
	* Value 4: Automatic balance
	* Value 10: Motherboard overheat
	*/
	uint8_t balanceStateSign = 0;

	// Wheel diameter (No idea) - 0 mm (bytes 106-107)
	uint16_t wheelDiameter = 0;

	// Number of pulses per week - (bytes 108-109)
	uint16_t weeklyHeartbeat = 0;

	// BMS switch - (byte 110)
	uint8_t bmsSwitch = 0;

	// Power out current power (bytes 111-114)
	int32_t powerOut = 0;

	// Cell with highest voltage - (byte 115)
	uint8_t cellHighestVoltageNumber = 0;

	// Voltage in the cell with highest voltage - 0.000V（bytes 116-117) DIVIDED BY 1.000
	double cellHighestVoltageValue = 0;

	// Cell with lowest voltage - (byte 118)
	uint8_t cellLowestVoltageNumber = 0;

	// Voltage in the cell with lowest voltage - 0.000V（bytes 119-120) DIVIDED BY 1.000
	double cellLowestVoltageValue = 0;

	// Average of voltage from all cells - 0.000V（bytes 121-122) DIVIDED BY 1.000
	double averageCellsVoltage = 0;

	// Number of effective batteries - (byte 123)
	uint8_t numberOfBatteries = 0;

	// The voltage between the D-S poles of the detected discharge tube - 0.0 V (bytes 124-125) DIVIDED BY 10
	double voltageDS = 0;

	// Discharging MOSFET drive voltage - 0.0V (bytes 126-127) DIVIDED BY 10
	double dischargingMOSFETDriveVoltage = 0;

	// Charging MOSFET driving voltage - 0.0V (bytes 128-129) DIVIDED BY 10
	double chargingMOSFETDriveVoltage = 0;

	// The initial value of the comparator when the detected current is 0 - (bytes 130-131)
	uint16_t initialComparatorVoltage = 0;

	// Equalization control - 1 bit for each cell (bytes 132-135)
	uint32_t equalizationControl = 0;

	/* System log - (bytes 136-137)
	* Bit 0-4 battery number
	* Bit 5-9 sequence
	* Bit 10-14 charge and discharge
	* Bit 15 (1 discharge, 0 charge)
	*/
	uint16_t systemLog = 0;
};

// Struct holding the motor controller data
// New data each 100 ms
struct StructMCU
{
	// Calculated parameters =============================================================

	// When the motor changes the direction, and the throttle is high, keep the bike stopped till put it in 0
	// This is the error E312
	bool ECUHighThrottleProt = false;

	// Average of voltage during 30 seconds
	double avgVoltage30s = 0.0;

	// Keep saved output variables for debug
	uint8_t statusOutDebug;
	uint8_t currentOutDebug;
	uint8_t regenOutDebug;
	uint8_t minThrottleDebug;
	uint8_t maxThrottleDebug;

	/* MCU state
	0 = turn on, but without power
	1 = turn on, but without power and check throttle
	3 = normal operation
	*/
	uint8_t state = 0;
	uint8_t stateDebug = 0; // Variable used for debugging, the value it's changed before the switch in the function that prepare the packet to be sent

	// Flag the changes in the reverse MODE, to be used by the function that prepare the CAN packet to the MCU
	bool reverseModeChanged = false;

	// Parameters from the MCU ===========================================================

	// Motor voltage - 0.0V (bytes 0L-1H Packet 1) DIVIDED BY 10
	double voltage = 0;

	// Bus current - 0.0A (bytes 2L-3H Packet 1) REMOVED THE OFFSET OF 20000 THEN DIVIDED BY 10
	double busCurrent = 0;

	// Phase current - 0.0A (bytes 4L-5H Packet 1) DIVIDED BY 10
	double phaseCurrent = 0;

	// RPM (bytes 6-7 Packet 1) REMOVED THE OFFSET OF 20000
	int16_t rpm = 0;

	// Motor temperature °C (byte 0 Packet 2) REMOVED THE OFFSET OF 50
	int16_t motorTemperature = 0;

	// Controller temperature °C (byte 1 Packet 2) REMOVED THE OFFSET OF 50
	int16_t controllerTemperature = 0;

	// Throttle position % (byte 2 Packet 2)
	uint8_t throttle = 0;

	// Status low byte (byte 3)
	uint8_t lowByteStatus = 0;

	// Status high byte (byte 4) - IT DOESN'T HAVE ANY DATA
	uint8_t highByteStatus = 0;

	/* MCU Low Byte Error - (byte 5)
	* Bit 0 DC contactor 3 failure
	* Bit 1 DC contactor 2 failure
	* Bit 2 Main contactor failure
	* Bit 3 Motor thermal shutdown
	* Bit 4 Motor thermal limit
	* Bit 5 Controller thermal shutdown
	* Bit 6 Controller thermal limit
	* Bit 7 Accelerator failure
	*/
	uint8_t lowByteError = 0;

	/* MCU High Byte Error - (byte 6)
	* Bit 0 Throttle high protection
	* Bit 1 Undervoltage shutdown
	* Bit 2 Limp mode (Speed limit 25%)
	* Bit 3 50% speed limit
	* Bit 4 Overvoltage protection
	* Bit 5 Overcurrent protection
	* Bit 6 Motor stalled
	* Bit 7 Hall sensor failure
	*/
	uint8_t highByteError = 0;
};

// Struct holding the charger data
// New data each 1000 ms
struct StructCharger
{
	// Calculated parameters =============================================================

	// If the process of receiving packet is normal
	bool connectionCheck = false;

	// To output
	double voltageOutput;
	double currentOutput;

	// Parameters from the charger =======================================================

	// Charger voltage - 0.0V (bytes 0H-1L) DIVIDED BY 10
	double voltage = 0;

	// Charger current - 0.0A (bytes 2H-3L) DIVIDED BY 10
	// Highest bit represents charging (0) or discharging (1).
	double current = 0;

	// Hardware failure (Bit 0 Byte 4) - 0 normal, 1 failure
	// Charger temperature (Bit 1 Byte 4) - 0 normal, 1 over-temperature protection
	// Input voltage (Bit 2 Byte 4) - 0 normal, 1 input voltage is wrong and the charger stops working
	// Start state (Bit 3 Byte 4) - 0 detects the battery voltage and enters the start state, 1 closed (used to prevent reverse battery connection)
	// Communication status (Bit 4 Byte 4) - 0 normal, 1 communication receiving timeout
	uint8_t statusByte = 0;
};

// Struct used to save data for debug in the flash memory, to be completed
struct InfoToSave
{
	// ====================================================================================
	// General Info

	// Code of the packet being saved
	uint32_t packetCode = 0;

	// Time from the RTC
	uint8_t second = 0;
	uint8_t minute = 0;
	uint8_t hour = 0;
	uint8_t dayW = 0;
	uint8_t dayM = 0;
	uint8_t month = 0;
	uint8_t year = 0;

	// Board temperature (from RTC Device - DS3231)
	int8_t boardTemperature = 0;

	// Board supply voltage (MULTIPLIED BY 10)
	uint8_t boardSupplyVoltage = 0;

	// Odometer
	double odometerKm = 0;

	// Trip (in the code it is double, but to save, it's in uint16_t, to save space)
	uint16_t tripKm = 0;

	// Speed (in the code it is double, but to save, it's in uint16_t, to save space)
	uint16_t speedKmh = 0;

	// Maximum speed
	uint16_t maximumSpeed;

	/*
	* RPM (number used for speed, trip and odometer calculation)
	* If the vehicle has transmission, this number will be different from the RPM sent by the MCU
	* If the vehicle doesn't has transmission, this number will be the absolute value, always positive, even when going reverse
	*/
	uint16_t rpm = 0;

	// Efficiency
	int16_t efficiency;

	/*
	* Vehicle status byte 1
	* Bit 0 Storage mode
	* Bit 1 Lock mode
	* Bit 2 Reverse mode button (3 seconds pressing mode button)
	* Bit 3 Charged connected
	* Bit 4 Charging (Charged connected and current higher than 1A)
	* Bit 5 Fast charge enabled (can be from the switch or future backend)
	* Bit 6 Errors detected?
	* Bit 7 Riding mode changed because an error
	*/
	uint8_t vehicleStatuByte1 = 0;

	/*
	* Vehicle status byte 2
	* Bits 0-2 Riding mode (0 to 7)
	* Bit  3 RESERVED
	* Bit  4 BMSConnectionCheck
	* Bit  5 MCUConnectionCheck1
	* Bit  6 MCUConnectionCheck2
	* Bit  7 ECUHighThrottleProt
	*/
	uint8_t vehicleStatuByte2 = 0;

	// RESERVED vehicle status bytes
	uint8_t vehicleStatuByte3 = 0;
	uint8_t vehicleStatuByte4 = 0;
	uint8_t vehicleStatuByte5 = 0;

	// Number of active errors
	uint16_t numActiveErrors = 0;

	// Sum of active errors
	uint32_t sumActiveErrors = 0;

	// Data from inertial sensor
	int16_t roll = 0;  // INT PART
	int16_t pitch = 0; // INT PART

	// Data from TMPS sensor
	uint8_t frontTirePressure = 0;	 // Pressure in the front tire (psi) INT PART
	int8_t frontTireTemperature = 0; // Temperature in the front tire (degrees celsius)
	uint8_t rearTirePressure = 0;	 // Pressure in the rear tire (psi) INT PART
	int8_t rearTireTemperature = 0;	 // Temperature in the rear tire (degrees celsius)

	// Data from I2C Expander
	uint16_t inputsI2C = 0;	 // Physical state of the input (Pay attention to kickstand and killswitch that are inverted in the code)
	uint16_t outputsI2C = 0; // Variable written to the I2C expanders in the output registers

	// ====================================================================================
	// BMS Info

	// Battery voltage - 0.0 V MULTIPLIED BY 10
	uint16_t BMSVoltage = 0;

	// Voltage of each cell. Value 0 = 3V. Incrementing of 0.01V each bit increment.
	uint8_t BMSCellVoltages[32];

	// Battery current - 0.0 A MULTIPLIED BY 10
	int16_t BMSCurrent = 0;

	// Battery temperature probes (°C)
	int8_t BMSTemperatures[6];

	// State of charge (%)
	uint8_t BMSSoc = 0;

	// Battery Physical Capacity (AH) INT PART
	uint8_t BMSBatteryPhysicalCapacity = 0;

	// Battery remaining capacity - 0.000000 AH MULTIPLIED BY 1.000.000
	uint32_t BMSBatteryRemainingCapacity = 0;

	// BMS Turn on time (in seconds)
	uint32_t BMSTurnOnTime = 0;

	// Cell with highest voltage
	uint8_t BMSCellHighestVoltageNumber = 0;

	// Voltage in the cell with highest voltage. Value 0 = 3V. Incrementing of 0.01V each bit increment.
	uint8_t BMSCellHighestVoltageValue = 0;

	// Cell with lowest voltage
	uint8_t BMCCellLowestVoltageNumber = 0;

	// Voltage in the cell with lowest voltage. Value 0 = 3V. Incrementing of 0.01V each bit increment.
	uint8_t BMSCellLowestVoltageValue = 0;

	// Number of effective batteries
	uint8_t BMSNumberOfBatteries = 0;

	// Charging MOSFET status flag
	uint8_t BMSChargingMosfetStatus = 0;

	// Discharging MOSFET status flag
	uint8_t BMSDischargingMosfetStatus = 0;

	// Balance status
	uint8_t BMSBalanceStateSign = 0;

	// ====================================================================================
	// MCU Info

	// Motor voltage - 0.0V MULTIPLIED BY 10
	uint16_t MCUVoltage = 0;

	// Bus current - 0.0A MULTIPLIED BY 10
	int16_t MCUBusCurrent = 0;

	// RPM
	int16_t MCURpm = 0;

	// Motor temperature °C
	int16_t MCUMotorTemperature = 0;

	// Controller temperature °C
	int16_t MCUControllerTemperature = 0;

	// Throttle position %
	uint8_t MCUThrottle = 0;

	// MCU Error flags
	uint8_t MCULowByteError;
	uint8_t MCUHighByteError;

	/*
	* MCU status byte (IT'S NOT THE SAME AS THE MCU)
	* Bit 0 MCU running (send)
	* Bit 1 MCU break (send - NOT IMPLEMENTED ON CAN YET)
	* Bit 2 MCU reverse mode (send)
	* Bit 3 MCU running (receive)
	* Bit 4 MCU break (receive)
	* Bit 5 MCU reverse mode (receive)
	* Bits 6-7 MCU State (according to the function that prepares the data to be sent)
	*/
	uint8_t MCUStatusByte = 0;

	// Max Phase Current (CAN send) - Values from 0 to 256 (Each unit is 4A)
	uint8_t MCUOutCurrent;

	// Max Regen Current (CAN send) - Values from 0 to 256 (Each unit is 2A, so it goes till 512A)
	uint8_t MCUMaxRegen;

	// Throttle Minimum (CAN send) - Values from 0 to 256 (Each unit is 0.0195V)
	uint8_t minimumThrottle;

	// Throttle Maximum (CAN send) - Values from 0 to 256 (Each unit is 0.0195V)
	uint8_t maximumThrottle;

	// ====================================================================================
	// Charger Info

	// Charger voltage V MULTIPLIED BY 10 (CAN receive)
	uint16_t chargerVoltage = 0;

	// Charger current A MULTIPLIED BY 10 (CAN receive)
	int16_t chargerCurrent = 0;

	// Status byte received from the charger
	uint8_t chargerStatusByte = 0;

	// Max charger voltage V MULTIPLIED BY 10 (CAN send)
	uint16_t maxChargerVoltage = 0;

	// Max charger current A MULTIPLIED BY 10 (CAN send)
	int16_t maxChargerCurrent = 0;

	// ====================================================================================
	// ECU Info

	// Timer in milliseconds since ECU started to run
	uint32_t millis = 0;

	//
	/*
	* Reason why the cores restarted last time
	* Bits 0-3 Core 1
	* Bits 4-7 Core 0
	* Values:
	* 0  : Vbat power on reset
    * 2  : Software reset digital core
    * 3  : Legacy watch dog reset digital core
    * 4  : Deep Sleep reset digital core
    * 5  : Reset by SLC module, reset digital core
    * 6  : Timer Group0 Watch dog reset digital core
    * 7  : Timer Group1 Watch dog reset digital core
    * 8  : RTC Watch dog Reset digital core
    * 9  : Instrusion tested to reset CPU
    * 10 : Time Group reset CPU
    * 11 : Software reset CPU
    * 12 : RTC Watch dog Reset CPU
    * 13 : for APP CPU, reseted by PRO CPU
    * 14 : Reset when the vdd voltage is not stable
    * 15 : RTC Watch dog reset digital core and rtc module
	*/
	uint8_t resetReason = 0;

	// Index based on the expected number of executions from each task
	uint8_t softwareHealthIndex;
};

#endif