/******************************************************************************
 * func_MCU.cpp
 * This file has the code for the functions that analizes the MCU data, calculates
 * speed and prepares the outputs for it
 *****************************************************************************/

#include "func_MCU.h"

// Analyzes the first MCU package
void MCUPack1Analysis(uint8_t *CANDataArray)
{
	MCUData.voltage = ((double)uint8To_Uint16(CANDataArray[1], CANDataArray[0])) / 10.0;
	// Serial.printf("voltage:%f can1:%d can0:%d\n",MCUData.voltage,(int)CANDataArray[1],(int)CANDataArray[0]);
	MCUData.busCurrent = (((double)uint8To_Int16(CANDataArray[3], CANDataArray[2])) - 20000.0) / 10.0;
	MCUData.phaseCurrent = ((double)uint8To_Int16(CANDataArray[5], CANDataArray[4])) / 10.0;
	MCUData.rpm = (uint8To_Int16(CANDataArray[7], CANDataArray[6])) - 20000;
	// Calc the average of voltage during the last 30 seconds
	uint16_t auxVoltage = uint8To_Uint16(CANDataArray[1], CANDataArray[0]);
	MCUData.avgVoltage30s = (avgMCUVoltage.addNCalcAvg(auxVoltage)) / 10.0;
	/*Serial.print("Voltage: ");
	Serial.println(MCUData.voltage);
	Serial.print("BusCurrent: ");
	Serial.println(MCUData.busCurrent);
	Serial.print("PhaseCurrent: ");
	Serial.println(MCUData.phaseCurrent);
	Serial.print("RPM: ");
	Serial.println(MCUData.rpm);
	Serial.println();*/

	// If the vehicle doesn't have transmission, the speed is calculated using the rpm measured by the MCU
	// Important to have the abs function to not use negative numbers, this way even going reverse, the speed will be positive
	if (!vehicleParam.hasTransmission)
	{
		vehicleInfo.rpm = abs(MCUData.rpm);
		motionAnalysis();
	}
}

// Analyzes the second MCU package
void MCUPack2Analysis(uint8_t *CANDataArray)
{
	MCUData.controllerTemperature = CANDataArray[1] - 50;
	MCUData.throttle = CANDataArray[2];
	MCUData.lowByteStatus = CANDataArray[3];
	MCUData.highByteStatus = CANDataArray[4];
	MCUData.lowByteError = CANDataArray[5];
	MCUData.highByteError = CANDataArray[6];

	// Get the break switch
	inputs.breakSwitch = bitRead(MCUData.lowByteStatus, 6);

	// Smooth the motor temperature
	int8_t auxMotorTemp = (int8_t)(CANDataArray[0] - 50);
	MCUData.motorTemperature = (int8_t)avgMotorTemp.addNCalcAvg(auxMotorTemp);
}

// Prepares the CAN packet to send via can to the MCU
void prepareCANMCUOut()
{
	// Resets MCU CAN Packet
	MCUAPTSend.resetData();

	// Variables used to smooth the current output
	static uint8_t lastRidingMode = 0;		 // Used to detect when the riding mode was decreased
	static uint16_t smootherCounter = 0;	 // Counter used to just smooth for some cycles after transition
	const uint16_t smootherNumElements = 20; // Number of currents to smooth

	// Keep in the first state if the vehicle cannot run or if the reverse mode changed
	if ((!checkIfBikeCanRun()) || (MCUData.reverseModeChanged))
	{
		MCUData.state = 0;
		MCUData.reverseModeChanged = false;

		// If the MCU state was changed to 0 due other conditions that are not the throttle, removes the ECU throttle high protection
		MCUData.ECUHighThrottleProt = false;

		// Resets the variables used to smooth the currentOut
		avgMCUCurrentOut.forceAvg(0);
	}

	// As the throttle information is lost when the MCU is turned off, it will always be on, and the control of it will be based on current output
	MCUData.stateDebug = MCUData.state;

	// Check the power reduction
	// It needs to be here to update the averages
	double pwrToReduce = checkPowerReduction() / 100.0;

	static uint32_t millishightemperature = 0;
	if(pwrToReduce > 0)		//If the temperature is above 120, the riding mode changes to city
		vehicleInfo.ridingMode = 1;
	if((pwrToReduce) > 0.93)	//If the temperature is higher than 140, the high temperature icon is solid
	{
		tempIcon = 1; 
	}
	else if((pwrToReduce) > 0.7)	// If the temperature is higher than 135, the high temperature icon flashes once in 0.5s
	{
		if(millishightemperature == 0 || (millis() - millishightemperature) > 500)
		{
			millishightemperature = millis();
			tempIcon = !tempIcon;
		}
	}
	else if(pwrToReduce > 0) // If the temperature is higher than 120, the high temperature icon flashes once in 1s
	{
		if(millishightemperature == 0 || (millis() - millishightemperature) > 1000)
		{
			millishightemperature = millis();
			tempIcon = !tempIcon;
		}
	}
	else
	{
		tempIcon = 0;	//wzh20230413
	}
	
	switch (MCUData.state)
	{
	case 0:
		// Run the MCU
		MCUAPTSend.writeByte(MCUAPTSend_byteStatus, 0b00100000);

		// Keep the current as 0 (No power)
		MCUAPTSend.writeByte(MCUAPTSend_byteCurrentOut, 0);

		// Next state
		MCUData.state++;
		break;

	case 1:
		// Keep the MCU on, but without power
		MCUAPTSend.writeByte(MCUAPTSend_byteStatus, 0b00100000);

		// Keep the current as 0 (No power)
		MCUAPTSend.writeByte(MCUAPTSend_byteCurrentOut, 0);

		// If the throttle is not 0, come back to the 1st state and flag the protection error
		if (MCUData.throttle != 0)
		{
			MCUData.ECUHighThrottleProt = true;
			MCUData.state = 0;
		}
		// If the throttle is 0, turn the motor on normaly and remove the error
		else
		{
			MCUData.ECUHighThrottleProt = false;
			MCUData.state++;
		}
		break;

	case 2:
		// If it reverse mode is active
		if (vehicleInfo.isInReverseMode)
		{
			// Put in reverse mode, but if the speed is bigger than 10Km, put the MCU in forward mode without current, it is equivalent of turning MCU off
			// This is done because if the MCU is turned off, some informations are lost
			if (vehicleInfo.speedKmh <= 10)
			{
				// The reverse mode has pre configured settings for power, so it's just necessary to set the direction bit
				MCUAPTSend.writeByte(MCUAPTSend_byteStatus, 0b10100000);
			}
			else
			{
				MCUAPTSend.writeByte(MCUAPTSend_byteStatus, 0b00100000);
				MCUAPTSend.writeByte(MCUAPTSend_byteCurrentOut, 0);
			}
		}
		// If the break input is active, removes the power from the MCU
		/*else if (inputs.breakSwitch)
		{
			mcuPowerControl.status = 0b00100000;
			mcuPowerControl.MCUOutCurrent = 0;
		}*/
		else
		{
			// OUT CURRENT ============================================================
			// Check the maximum speed, based on possible limitations
			uint16_t speedLimit = checkMaxSpeed();
	
			// If the riding mode was decreased, flags that the output current needs to be smoothed
			// It will use the smoothed data when the smootherCounter is different from 0
			if (((lastRidingMode == 2) && (vehicleInfo.ridingMode == 1)) ||
				((lastRidingMode == 1) && (vehicleInfo.ridingMode == 0)))
			{
				smootherCounter = smootherNumElements;
			}

			// If the riding mode was increased, flags that it's not necessary to smooth anymore
			// Put 0 in the smootherCounter
			if (((lastRidingMode == 0) && (vehicleInfo.ridingMode == 1)) ||
				((lastRidingMode == 1) && (vehicleInfo.ridingMode == 2)))
			{
				smootherCounter = 0;
			}

			// It always calculates the smoother, but just use when the smootherCounter is different from 0
			uint8_t auxCurrentOut = adjustCurrentOut(speedLimit);

			// Check the power reduction
			pwrToReduce = (double)auxCurrentOut * pwrToReduce;
			if (pwrToReduce < 0)
				pwrToReduce = 0;
			else if (pwrToReduce > (double)auxCurrentOut)
				pwrToReduce = (double)auxCurrentOut;
			auxCurrentOut = auxCurrentOut - (uint8_t)pwrToReduce;

			//Current is limited by dropout voltage
			double maxDropoutVoltage = 1, minDropoutVoltage = 0.8;
			double dropoutVoltage = BMSData.cellHighestVoltageValue - BMSData.cellLowestVoltageValue;
			//If the dropout voltage is greater than or equal to the maximum achievable dropout, the output current is 0
			if(dropoutVoltage > maxDropoutVoltage || dropoutVoltage == maxDropoutVoltage)
				auxCurrentOut = 0;
			//If the dropout voltage is greater than the set minimum voltage difference, the current is limited proportionally
			else if(dropoutVoltage > minDropoutVoltage)
				auxCurrentOut *= (maxDropoutVoltage - dropoutVoltage) / (maxDropoutVoltage - minDropoutVoltage);			

			double currentOutSmoothed = avgMCUCurrentOut.addNCalcAvg(auxCurrentOut);

			// If the smootherCounter is different from 0, used the smoothed current and decreases one each cycle
			if (smootherCounter != 0)
			{
				MCUAPTSend.writeByte(MCUAPTSend_byteCurrentOut, (uint8_t)currentOutSmoothed);
				smootherCounter--;
			}
			// If it is equal 0, just use the adjustCurrentOut function
			else
			{
				MCUAPTSend.writeByte(MCUAPTSend_byteCurrentOut, auxCurrentOut);
			}

			// Update the lastRidingMode to prepare for next cycle
			lastRidingMode = vehicleInfo.ridingMode;

			// REGEN ==================================================================
			float regen_Soc = 0;	//Stores the return current calculated by capacity
			float regen_Speed = 0;	//Stores the return current calculated by velocity
			float regen = 0;
			switch (vehicleInfo.ridingMode)
			{
			// City mode
			case 1:
				if(BMSData.SOC > 95)	//When the capacity is greater than 95
					regen_Soc = (100-BMSData.SOC) * (60 / 5);   
				else					//When the capacity is less than or equal to 95
					regen_Soc = 60;
				//When the riding speed is greater than or equal to 70, it is less than the maximum set speed
				if((vehicleInfo.speedKmh >= 70) && (vehicleInfo.speedKmh < infoToSave.maximumSpeed))
					regen_Speed = regenSpeed.Regend(infoToSave.maximumSpeed, 70, vehicleInfo.speedKmh, 60, 0.5);
					//When the speed is greater than the maximum set speed
				else if(vehicleInfo.speedKmh > infoToSave.maximumSpeed)
					regen_Speed = 60 / 2;
					
				//When the speed is less than 70
				else
					regen_Speed = 60;
				break;
			// Pro mode
			case 2:
				if(BMSData.SOC > 95)
					regen_Soc = (100-BMSData.SOC) * (95 / 5);
				else
					regen_Soc = 95;

				if((vehicleInfo.speedKmh >= 70) && (vehicleInfo.speedKmh < infoToSave.maximumSpeed))
					regen_Speed = regenSpeed.Regend(infoToSave.maximumSpeed, 70, vehicleInfo.speedKmh, 95, 0.5);
				else if(vehicleInfo.speedKmh > infoToSave.maximumSpeed)
					regen_Speed = 95 / 2;
				else
					regen_Speed = 95;
				break;

			// Eco mode
			default:
				if(BMSData.SOC > 95)
					regen_Soc = (100-BMSData.SOC) * (60 / 5);
				else
					regen_Soc = 60;

				if((vehicleInfo.speedKmh >= 70) && (vehicleInfo.speedKmh < infoToSave.maximumSpeed))
					regen_Speed = regenSpeed.Regend(infoToSave.maximumSpeed, 70, vehicleInfo.speedKmh, 60, 0.5);
				else if(vehicleInfo.speedKmh > infoToSave.maximumSpeed)
					regen_Speed = 60 / 2;
				else
					regen_Speed = 60;	
				break;
			}
			//The regeneration current calculated by capacity and riding speed is subject to the large reduction in the regeneration current
			regen = (regen_Soc < regen_Speed) ? regen_Soc : regen_Speed;
			MCUAPTSend.writeByte(MCUAPTSend_byteMaxRegen, regen);
			
			//20% increase in brake REGEN in Pro mode and 30% in other modes
		if((vehicleInfo.ridingMode == 2) && (inputs.breakSwitch))
			{
				MCUAPTSend.writeByte(MCUAPTSend_byteMaxRegen, regen * 1.2);
			} 
			else if(inputs.breakSwitch)
				MCUAPTSend.writeByte(MCUAPTSend_byteMaxRegen, regen * 1.3);
			// Status byte ====================================================
			// Just the running bit = 1
			MCUAPTSend.writeByte(MCUAPTSend_byteStatus, 0b00100000);
		}
		break;

	// If this condition will happen, it means that something bad happened in the software, so turn off the motor
	default:
		MCUAPTSend.writeByte(MCUAPTSend_byteStatus, 0b00000000);
		break;
	}

	// Always the same ========================================================
	MCUAPTSend.writeByte(MCUAPTSend_byteMinThrottle, 50);
	MCUAPTSend.writeByte(MCUAPTSend_byteMaxThrottle, 180);
	MCUAPTSend.writeByte(MCUAPTSend_byteMaxAcceleration, 200);

	// Save the output for debug
	MCUData.statusOutDebug = MCUAPTSend.getByte(MCUAPTSend_byteStatus);
	MCUData.currentOutDebug = MCUAPTSend.getByte(MCUAPTSend_byteCurrentOut);
	MCUData.regenOutDebug = MCUAPTSend.getByte(MCUAPTSend_byteMaxRegen);
	MCUData.minThrottleDebug = MCUAPTSend.getByte(MCUAPTSend_byteMinThrottle);
	MCUData.maxThrottleDebug = MCUAPTSend.getByte(MCUAPTSend_byteMaxThrottle);

	// Keep stopped - FOR DEBUG
	// MCUAPTSend.writeByte(MCUAPTSend_byteCurrentOut, 0);
}

// Return the max speed, looking at the possible limitations
// IMPLEMENT IN THE FUTURE THE CUSTOM TOP SPEED, THAT WILL BE CONFIGURED VIA APP
uint16_t checkMaxSpeed()
{
	return vehicleParam.maximumSpeed;
}

// Adjust the current output
uint8_t adjustCurrentOut(uint16_t speedLimit)
{
	// Check the percentage of the actual speed based on the maximum speed
	uint16_t percentageOfMaxSpeed = vehicleInfo.speedKmh / speedLimit * 100;

	// Check the number of divisions that are being used for power calculations
	// As the diviosions are uint8_t, the number is equal of the bytes size
	uint8_t numCurrentOutDiv = sizeof(currentOutDiv);

	// Detects the actual speed section for the power calculation
	uint8_t x = 0;
	for (x = 0; x < numCurrentOutDiv; x++)
	{
		if (percentageOfMaxSpeed < currentOutDiv[x])
		{
			break;
		}
	}

	// Keep x between 1 and the (number of divisions)-1, to not have overflow
	if (x < 1)
		x = 1;
	if (x > (numCurrentOutDiv - 1))
		x = (numCurrentOutDiv - 1);

	// Calculates the currentOut, keeping linear between the power informed in the arrays
	int16_t currentOut;
	switch (vehicleInfo.ridingMode)
	{
	// City mode
	case 1:
		currentOut = map(percentageOfMaxSpeed, currentOutDiv[x - 1], currentOutDiv[x], currentOutCityMode[x - 1], currentOutCityMode[x]);
		break;

	// Pro mode
	case 2:
		currentOut = map(percentageOfMaxSpeed, currentOutDiv[x - 1], currentOutDiv[x], currentOutProMode[x - 1], currentOutProMode[x]);
		break;

	// Eco mode
	default:
		currentOut = map(percentageOfMaxSpeed, currentOutDiv[x - 1], currentOutDiv[x], currentOutEcoMode[x - 1], currentOutEcoMode[x]);
		break;
	}

	// Divide currentOut by 4, because it's the ratio between current out and the values sent via CAN
	currentOut /= 4;

	// If there is some of the errors that limit the power in 50% happening, divide the current by 2
	if (checkErrors50Power())
	{
		currentOut /= 2;
	}
	// Make sure that the currentOut will be between 1 and 200
	if (currentOut > 200)
		currentOut = 200;
	else if (currentOut < 1)
		currentOut = 1;

	return (uint8_t)currentOut;
}

// Calculates the speed, odometer and trip meter, based on the RPM
void motionAnalysis()
{
	// Variables used in this function
	static uint32_t lastAnalysisMicros;
	static uint32_t analyzedTime;
	double parseDistance;

	// The wheel circumference is necessary to add to the odometer each rotation
	static const double wheelCircumference = 2.0 * (double)PI * (vehicleParam.wheelRadius / 100.0); // Wheel circumference in meters, to use in speed calculation

	// To convert from RPM to speed in Km/h is necessary to divide rpm by 60, multiply by the wheel circumference
	// and multiply by 3.6 to have the speed in Km/h. To not be necessary to do all calcutations, the part that
	// never changes is done below
	static const double ratioRPMToSpeedKM = (wheelCircumference * 3.6) / 60.0;

	// If the RPM is 0, it's not necessary to calculate anything, because the speed is also 0

	if (vehicleInfo.rpm == 0)
	{
		vehicleInfo.speedKmh = 0;
	}
	// If not, calulates the speed and increase the trip meter and odometer
	else
	{
		// Convert from RPM to speed in Km/h and multiply
		vehicleInfo.speedKmh = ((double)vehicleInfo.rpm * ratioRPMToSpeedKM) * vehicleParam.multSpeed;

		// Time passed since last execution of this function
		analyzedTime = micros() - lastAnalysisMicros;

		// Prepare the timer for next execution
		lastAnalysisMicros = micros();

		// Multiply the speed by the analyzed time to get how much did the motorcycle moved this time and add to the odometer and trip meter
		parseDistance = vehicleInfo.speedKmh * ((double)analyzedTime / 3600000000.0);
		vehicleInfo.odometerKm += parseDistance;
		vehicleInfo.tripKm += parseDistance;
	}
}

// Checks power reductions
double checkPowerReduction()
{
	// Variables to be used in this function
	double auxPowerReduction;

	// Calculate the power reduction based on the FET temperatures
	auxPowerReduction = pwrRedFetTemp.addNCalcPwrReduction(BMSData.avgFETTemperature);

	// Check if the power reduction based on the battery temperatures is bigger
	if (pwrRedBatTemp.addNCalcPwrReduction(BMSData.avgBatteryTemperature) > auxPowerReduction)
		auxPowerReduction = pwrRedBatTemp.getPwrReduction();

	// Check if the power reduction based on the motor temperature is bigger
	if (pwrRedMotorTemp.addNCalcPwrReduction(MCUData.motorTemperature) > auxPowerReduction)
		auxPowerReduction = pwrRedMotorTemp.getPwrReduction();

	// Return the biggest reduction
	return auxPowerReduction;
}