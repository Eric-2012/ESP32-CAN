/******************************************************************************
 * func_BusI2C.cpp
 * This file has functions that uses the I2C Bus
 *****************************************************************************/

#include "func_BusI2C.h"

// Configure and read parameters
void configParametersObjects()
{
	// The parameters are keep in the RAM memory, in the vehicleParam struct and in the FRAM memory,
	// that is handled by objects
	// The objects are keep in a array, where the position is the paremeter code
	// THESE CODE CANNOT BE CHANGED, BECAUSE IS THE SAME USED BY THE CONFIGURATOR SOFTWARE

	// == Full Vehicle Targets ================================================

	paramFRAM[p_maximumSpeed].conf(0x0100, checkVarType(&vehicleParam.maximumSpeed));						  // PARAM_COD0 2 bytes - Maximum speed (kmh) from factory
	paramFRAM[p_rechargingTimeFullCharge].conf(0x0102, checkVarType(&vehicleParam.rechargingTimeFullCharge)); // PARAM_COD1 2 bytes - Estimated recharging time for full charge (seconds)
	paramFRAM[p_maximumPayload].conf(0x0104, checkVarType(&vehicleParam.maximumPayload));					  // PARAM_COD2 2 bytes - Maximum Payload (kg)
	paramFRAM[p_maxCurbWeight].conf(0x0106, checkVarType(&vehicleParam.maxCurbWeight));						  // PARAM_COD3 2 bytes - Max curb weight (kg)

	// == Battery Specification================================================
	paramFRAM[p_chemistry].conf(0x0108, checkVarType(&vehicleParam.chemistry[0]), 15);				   // PARAM_COD4 15 bytes - Chemistry
	paramFRAM[p_nominalCapacity].conf(0x0117, checkVarType(&vehicleParam.nominalCapacity));			   // PARAM_COD5 8 bytes - Capacity, Nominal (kwh)
	paramFRAM[p_liionBatterySetup].conf(0x011F, checkVarType(&vehicleParam.liionBatterySetup[0]), 15); // PARAM_COD6 15 bytes - Li-ion battery setup
	paramFRAM[p_maximumChargeCurrent].conf(0x012E, checkVarType(&vehicleParam.maximumChargeCurrent));  // PARAM_COD7 2 bytes - Maximum charge current (A DC)
	paramFRAM[p_maximumChargeVoltage].conf(0x0431, checkVarType(&vehicleParam.maximumChargeVoltage));  // PARAM_COD52 8 bytes - Maximum charge voltage (V)
	paramFRAM[p_BMS].conf(0x0130, checkVarType(&vehicleParam.BMS[0]), 25);							   // PARAM_COD8 25 bytes - BMS Model
	paramFRAM[p_BMSType].conf(0x0130, checkVarType(&vehicleParam.BMSType));							   // PARAM_COD53 - BMS Type - 0 = UART, 1 = CAN
	paramFRAM[p_thermalManagement].conf(0x0149, checkVarType(&vehicleParam.thermalManagement[0]), 40); // PARAM_COD9 40 bytes - Thermal management
	paramFRAM[p_doubleCharger].conf(0x0171, checkVarType(&vehicleParam.doubleCharger));				   // PARAM_COD10 1 byte - If the vehicle has two chargers. True = yes, False = no
	paramFRAM[p_backup12Battery].conf(0x0232, checkVarType(&vehicleParam.backup12Battery[0]), 45);	   // PARAM_COD25 45 bytes - Backup 12v battery
	paramFRAM[p_onboardCharger].conf(0x025F, checkVarType(&vehicleParam.onboardCharger[0]), 45);	   // PARAM_COD26 45 bytes - Onboard charger

	// == Motor & Controller Specification ====================================
	paramFRAM[p_continuousPower].conf(0x0172, checkVarType(&vehicleParam.continuousPower));							 // PARAM_COD11 2 bytes - Continuous power (kW)
	paramFRAM[p_peakPower].conf(0x0174, checkVarType(&vehicleParam.peakPower));										 // PARAM_COD12 2 bytes - Peak power (kW)
	paramFRAM[p_motorType].conf(0x0176, checkVarType(&vehicleParam.motorType[0]), 40);								 // PARAM_COD13 40 bytes - Motor type
	paramFRAM[p_ratedSpeed].conf(0x019E, checkVarType(&vehicleParam.ratedSpeed));									 // PARAM_COD14 2 bytes - Rated speed (RPM max)
	paramFRAM[p_targetGearRatio].conf(0x01A0, checkVarType(&vehicleParam.targetGearRatio[0]), 15);					 // PARAM_COD15 15 bytes - Target Gear Ratio
	paramFRAM[p_preferredMethodOfCooling].conf(0x01AF, checkVarType(&vehicleParam.preferredMethodOfCooling[0]), 25); // PARAM_COD16 25 bytes - Preferred method of cooling
	paramFRAM[p_torque].conf(0x01C8, checkVarType(&vehicleParam.torque));											 // PARAM_COD17 2 bytes - Torque (Nm)
	paramFRAM[p_finalDrive].conf(0x01CA, checkVarType(&vehicleParam.finalDrive[0]), 15);							 // PARAM_COD18 15 bytes - Final drive
	paramFRAM[p_hasOptionToReverse].conf(0x01D9, checkVarType(&vehicleParam.hasOptionToReverse));					 // PARAM_COD19 1 byte - True = yes, False = no
	paramFRAM[p_hasTransmission].conf(0x01DA, checkVarType(&vehicleParam.hasTransmission));							 // PARAM_COD20 1 byte- True = yes, False = no

	// == Misc Electronics & Wiring Specification =============================
	paramFRAM[p_ECUHardwareVersion].conf(0x01DB, checkVarType(&vehicleParam.ECUHardwareVersion[0]), 14); // PARAM_COD21 14 bytes - Hardware Version
	paramFRAM[p_ECUSoftwareVersion].conf(0x01E9, checkVarType(&vehicleParam.ECUSoftwareVersion[0]), 14); // PARAM_COD22 14 bytes - Software Version
	paramFRAM[p_wireHarnessVersion].conf(0x01F7, checkVarType(&vehicleParam.wireHarnessVersion[0]), 14); // PARAM_COD23 14 bytes - Wire harness version
	paramFRAM[p_dcdc].conf(0x0205, checkVarType(&vehicleParam.dcdc[0]), 45);							 // PARAM_COD24 45 bytes - DC-DC

	// Screen Specification
	paramFRAM[p_multSpeed].conf(0x028C, checkVarType(&vehicleParam.multSpeed));				// PARAM_COD27 8 bytes - Multiplier used to increse the speed to show in the display
	paramFRAM[p_speedUnit].conf(0x0294, checkVarType(&vehicleParam.speedUnit));				// PARAM_COD28 1 byte - 0 = km/h, 1 = mph
	paramFRAM[p_temperatureUnit].conf(0x0430, checkVarType(&vehicleParam.temperatureUnit)); // PARAM_COD51 - 0 = celsius, 1 = fahrenheit
	paramFRAM[p_screenType].conf(0x0295, checkVarType(&vehicleParam.screenType));			// PARAM_COD29 1 byte - 0 = UART, 1 = CAN

	// == Frame Specification =================================================
	paramFRAM[p_wheelbase].conf(0x0296, checkVarType(&vehicleParam.wheelbase)); // PARAM_COD30 2 bytes - Wheelbase (mm)

	// Suspension Specification
	paramFRAM[p_rakeAngle].conf(0x0298, checkVarType(&vehicleParam.rakeAngle)); // PARAM_COD31 1 byte - Rake angle (degree)
	paramFRAM[p_trail].conf(0x0299, checkVarType(&vehicleParam.trail));			// PARAM_COD32 8 bytes - Trail (mm)

	// == Wheel, Tire & Brake Specification ===================================
	paramFRAM[p_frontWheel].conf(0x02A1, checkVarType(&vehicleParam.frontWheel[0]), 45);			   // PARAM_COD33 45 bytes - Front wheel
	paramFRAM[p_frontTireSize].conf(0x02CE, checkVarType(&vehicleParam.frontTireSize[0]), 45);		   // PARAM_COD34 45 bytes - Front tire size
	paramFRAM[p_frontBrakeCaliper].conf(0x02FB, checkVarType(&vehicleParam.frontBrakeCaliper[0]), 45); // PARAM_COD35 45 bytes - Front brake caliper
	paramFRAM[p_frontBrakeDisc].conf(0x0328, checkVarType(&vehicleParam.frontBrakeDisc[0]), 45);	   // PARAM_COD36 45 bytes - Front brake disc
	paramFRAM[p_rearWheel].conf(0x0355, checkVarType(&vehicleParam.rearWheel[0]), 45);				   // PARAM_COD37 45 bytes - Rear wheel
	paramFRAM[p_rearTireSize].conf(0x0382, checkVarType(&vehicleParam.rearTireSize[0]), 45);		   // PARAM_COD38 45 bytes - Rear tire size
	paramFRAM[p_rearBrakeCaliper].conf(0x03AF, checkVarType(&vehicleParam.rearBrakeCaliper[0]), 45);   // PARAM_COD39 45 bytes - Rear brake caliper
	paramFRAM[p_rearBrakeDisc].conf(0x03DC, checkVarType(&vehicleParam.rearBrakeDisc[0]), 45);		   // PARAM_COD40 45 bytes - Rear brake disc
	//paramFRAM[p_frontTireSensorID].conf(0x0409, checkVarType(&vehicleParam.frontTireSensorID));      // NOT BEING USED NOW, BECAUSE THE SENSOR IN THE FRONT IS ODD - PARAM_COD41 2 bytes - ID of the TPMS sensor in the front tire
	//paramFRAM[p_rearTireSensorID].conf(0x040B, checkVarType(&vehicleParam.rearTireSensorID));        // NOT BEING USED NOW, BECAUSE THE SENSOR IN THE REAR IS EVEN - PARAM_COD42 2 bytes - ID of the TPMS sensor in the rear tire
	paramFRAM[p_numMagnetsSpeedSensor].conf(0x040D, checkVarType(&vehicleParam.numMagnetsSpeedSensor)); // PARAM_COD43 1 byte - Number of magnets for the speed sensor (number of pulses for 1 full rotation)
	paramFRAM[p_wheelRadius].conf(0x040E, checkVarType(&vehicleParam.wheelRadius));						// PARAM_COD44 8 bytes - Wheel radius in centimeter (For speed and distance calculations)

	// == For testing =========================================================
	paramFRAM[p_ignoreKillswitchKickstand].conf(0x0416, checkVarType(&vehicleParam.ignoreKillswitchKickstand)); // PARAM_COD45 1 byte - When true, the inputs killswitch and kickstand will be ignored

	// == Vehicle info ========================================================
	paramFRAM[p_btDeviceName].conf(0x0417, checkVarType(&vehicleInfo.btDeviceName[0]), 25); // PARAM_COD50 25 bytes - Bluetooth device name
	paramFRAM[p_odometerKm].conf(0x0000, checkVarType(&vehicleInfo.odometerKm));			// PARAM_COD46 8 bytes - Odometer in kilometers. Updated by the function motionAnalysis.
	paramFRAM[p_isStored].conf(0x0008, checkVarType(&vehicleInfo.isStored));				// PARAM_COD47 1 byte - True if the storage mode is on, false if not
	paramFRAM[p_isLocked].conf(0x0009, checkVarType(&vehicleInfo.isLocked));				// PARAM_COD48 1 byte - True if is locked, false if not
	paramFRAM[p_customMaxSpeed].conf(0x000A, checkVarType(&vehicleInfo.customMaxSpeed));	// PARAM_COD49 2 bytes - Custom maximum speed (kmh)
}

// Read all parameters
void readAllParameters()
{
	// == Full Vehicle Targets ================================================
	paramFRAM[p_maximumSpeed].read(vehicleParam.maximumSpeed);						   // PARAM_COD0 2 bytes - Maximum speed (kmh) from factory
	paramFRAM[p_rechargingTimeFullCharge].read(vehicleParam.rechargingTimeFullCharge); // PARAM_COD1 2 bytes - Estimated recharging time for full charge (seconds)
	paramFRAM[p_maximumPayload].read(vehicleParam.maximumPayload);					   // PARAM_COD2 2 bytes - Maximum Payload (kg)
	paramFRAM[p_maxCurbWeight].read(vehicleParam.maxCurbWeight);					   // PARAM_COD3 2 bytes - Max curb weight (kg)

	// == Battery Specification================================================
	paramFRAM[p_chemistry].read(vehicleParam.chemistry);					   // PARAM_COD4 15 bytes - Chemistry
	paramFRAM[p_nominalCapacity].read(vehicleParam.nominalCapacity);		   // PARAM_COD5 8 bytes - Capacity, Nominal (kwh)
	paramFRAM[p_liionBatterySetup].read(vehicleParam.liionBatterySetup);	   // PARAM_COD6 15 bytes - Li-ion battery setup
	paramFRAM[p_maximumChargeCurrent].read(vehicleParam.maximumChargeCurrent); // PARAM_COD7 2 bytes - Maximum charge current (A DC)
	paramFRAM[p_maximumChargeVoltage].read(vehicleParam.maximumChargeVoltage); // PARAM_COD52 8 bytes - Maximum charge voltage (V)
	paramFRAM[p_BMS].read(vehicleParam.BMS);								   // PARAM_COD8 25 bytes - BMS Model
	paramFRAM[p_BMSType].read(vehicleParam.BMSType);						   // PARAM_COD53 - BMS Type - 0 = UART, 1 = CAN
	paramFRAM[p_thermalManagement].read(vehicleParam.thermalManagement);	   // PARAM_COD9 40 bytes - Thermal management
	paramFRAM[p_doubleCharger].read(vehicleParam.doubleCharger);			   // PARAM_COD10 1 byte - If the vehicle has two chargers. True = yes, False = no
	paramFRAM[p_backup12Battery].read(vehicleParam.backup12Battery);		   // PARAM_COD25 45 bytes - Backup 12v battery
	paramFRAM[p_onboardCharger].read(vehicleParam.onboardCharger);			   // PARAM_COD26 45 bytes - Onboard charger

	// == Motor & Controller Specification ====================================
	paramFRAM[p_continuousPower].read(vehicleParam.continuousPower);				   // PARAM_COD11 2 bytes - Continuous power (kW)
	paramFRAM[p_peakPower].read(vehicleParam.peakPower);							   // PARAM_COD12 2 bytes - Peak power (kW)
	paramFRAM[p_motorType].read(vehicleParam.motorType);							   // PARAM_COD13 40 bytes - Motor type
	paramFRAM[p_ratedSpeed].read(vehicleParam.ratedSpeed);							   // PARAM_COD14 2 bytes - Rated speed (RPM max)
	paramFRAM[p_targetGearRatio].read(vehicleParam.targetGearRatio);				   // PARAM_COD15 15 bytes - Target Gear Ratio
	paramFRAM[p_preferredMethodOfCooling].read(vehicleParam.preferredMethodOfCooling); // PARAM_COD16 25 bytes - Preferred method of cooling
	paramFRAM[p_torque].read(vehicleParam.torque);									   // PARAM_COD17 2 bytes - Torque (Nm)
	paramFRAM[p_finalDrive].read(vehicleParam.finalDrive);							   // PARAM_COD18 15 bytes - Final drive
	paramFRAM[p_hasOptionToReverse].read(vehicleParam.hasOptionToReverse);			   // PARAM_COD19 1 byte - True = yes, False = no
	paramFRAM[p_hasTransmission].read(vehicleParam.hasTransmission);				   // PARAM_COD20 1 byte- True = yes, False = no

	// == Misc Electronics & Wiring Specification =============================
	paramFRAM[p_ECUHardwareVersion].read(vehicleParam.ECUHardwareVersion); // PARAM_COD21 14 bytes - Hardware Version
	paramFRAM[p_ECUSoftwareVersion].read(vehicleParam.ECUSoftwareVersion); // PARAM_COD22 14 bytes - Software Version
	paramFRAM[p_wireHarnessVersion].read(vehicleParam.wireHarnessVersion); // PARAM_COD23 14 bytes - Wire harness version// PARAM_COD23 14 bytes - Wire harness version
	paramFRAM[p_dcdc].read(vehicleParam.dcdc);							   // PARAM_COD24 45 bytes - DC-DC

	// Screen Specification
	paramFRAM[p_multSpeed].read(vehicleParam.multSpeed);			 // PARAM_COD27 8 bytes - Multiplier used to increse the speed to show in the display
	paramFRAM[p_speedUnit].read(vehicleParam.speedUnit);			 // PARAM_COD28 1 byte - 0 = km/h, 1 = mph
	paramFRAM[p_temperatureUnit].read(vehicleParam.temperatureUnit); // PARAM_COD51 - 0 = celsius, 1 = fahrenheit
	paramFRAM[p_screenType].read(vehicleParam.screenType);			 // PARAM_COD29 1 byte - 0 = UART, 1 = CAN

	// == Frame Specification =================================================
	paramFRAM[p_wheelbase].read(vehicleParam.wheelbase); // PARAM_COD30 2 bytes - Wheelbase (mm)

	// Suspension Specification
	paramFRAM[p_rakeAngle].read(vehicleParam.rakeAngle); // PARAM_COD31 1 byte - Rake angle (degree)
	paramFRAM[p_trail].read(vehicleParam.trail);		 // PARAM_COD32 8 bytes - Trail (mm)

	// == Wheel, Tire & Brake Specification ===================================
	paramFRAM[p_frontWheel].read(vehicleParam.frontWheel);				 // PARAM_COD33 45 bytes - Front wheel
	paramFRAM[p_frontTireSize].read(vehicleParam.frontTireSize);		 // PARAM_COD34 45 bytes - Front tire size
	paramFRAM[p_frontBrakeCaliper].read(vehicleParam.frontBrakeCaliper); // PARAM_COD35 45 bytes - Front brake caliper
	paramFRAM[p_frontBrakeDisc].read(vehicleParam.frontBrakeDisc);		 // PARAM_COD36 45 bytes - Front brake disc
	paramFRAM[p_rearWheel].read(vehicleParam.rearWheel);				 // PARAM_COD37 45 bytes - Rear wheel
	paramFRAM[p_rearTireSize].read(vehicleParam.rearTireSize);			 // PARAM_COD38 45 bytes - Rear tire size
	paramFRAM[p_rearBrakeCaliper].read(vehicleParam.rearBrakeCaliper);	 // PARAM_COD39 45 bytes - Rear brake caliper
	paramFRAM[p_rearBrakeDisc].read(vehicleParam.rearBrakeDisc);		 // PARAM_COD40 45 bytes - Rear brake disc
	//paramFRAM[p_frontTireSensorID].read(vehicleParam.frontTireSensorID);  // NOT BEING USED NOW, BECAUSE THE SENSOR IN THE FRONT IS ODD - PARAM_COD41 2 bytes - ID of the TPMS sensor in the front tire
	//paramFRAM[p_rearTireSensorID].read(vehicleParam.rearTireSensorID);    // NOT BEING USED NOW, BECAUSE THE SENSOR IN THE REAR IS EVEN - PARAM_COD42 2 bytes - ID of the TPMS sensor in the rear tire
	paramFRAM[p_numMagnetsSpeedSensor].read(vehicleParam.numMagnetsSpeedSensor); // PARAM_COD43 1 byte - Number of magnets for the speed sensor (number of pulses for 1 full rotation)
	paramFRAM[p_wheelRadius].read(vehicleParam.wheelRadius);					 // PARAM_COD44 8 bytes - Wheel radius in centimeter (For speed and distance calculations)

	// == For testing =========================================================
	paramFRAM[p_ignoreKillswitchKickstand].read(vehicleParam.ignoreKillswitchKickstand); // PARAM_COD45 1 byte - When true, the inputs killswitch and kickstand will be ignored

	// == Vehicle info ========================================================
	paramFRAM[p_btDeviceName].read(vehicleInfo.btDeviceName);	  // PARAM_COD50 25 bytes - Bluetooth device name
	paramFRAM[p_odometerKm].read(vehicleInfo.odometerKm);		  // PARAM_COD46 8 bytes - Odometer in kilometers. Updated by the function motionAnalysis.
	paramFRAM[p_isStored].read(vehicleInfo.isStored);			  // PARAM_COD47 1 byte - True if the storage mode is on, false if not
	paramFRAM[p_isLocked].read(vehicleInfo.isLocked);			  // PARAM_COD48 1 byte - True if is locked, false if not
	paramFRAM[p_customMaxSpeed].read(vehicleInfo.customMaxSpeed); // PARAM_COD49 2 bytes - Custom maximum speed (kmh)
}