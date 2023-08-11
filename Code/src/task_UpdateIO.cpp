/******************************************************************************
 * task_UpdateIO.cpp
 * This file has the code for task UpdateIO and related functions
 * This task doesn't use the I2C Bus, it just analyze the data that was gotten
 * by the task BusI2C
 *****************************************************************************/

#include "task_UpdateIO.h"

// Code for the task UpdateIO
void taskCode_UpdateIO(void *pvParameters)
{
	// Variables for debounce
	static int8_t ctHeadlightHighBeam = 0; // Debounce counter for HeadlightHighBeam
	static int8_t ctTurnLeftSwitch = 0;	   // Debounce counter for TurnLeftSwitch
	static int8_t ctTurnRightSwitch = 0;   // Debounce counter for TurnRightSwitch
	static int8_t ctModeButton = 0;		   // Debounce counter for ModeButton
	static int8_t ctKickstand = 0;		   // Debounce counter for Kickstand
	static int8_t ctKillswitch = 0;		   // Debounce counter for Killswitch
	static int8_t ctKey = 0;			   // Debounce counter for Key
	static int8_t ctFastChargeSwitch = 0;  // Debounce counter for FastChargeSwitch

	// Used for modeButton events
	static bool lastModeButton = false;
	static uint32_t millisModeButtonPressed = 0;
	static bool flagModeButtonPressed = 0;
	static bool lastReverseMode = false; // To monitor the changes in the reverse mode

	// Used for turnLeftSwitch events
	static bool lastTurnLeftSwitch = false; // To monitor the changes in the turnLeftSwitch
	static uint32_t millisTurnLeftSwitchReleased = 0;
	static bool flagTurnLeftSwitchReleased = 0;

	// Used for turn right switch events
	static bool lastTurnRightSwitch = false; // To monitor the changes in the turnRightSwitch
	static uint32_t millisTurnRightSwitchReleased = 0;
	static bool flagTurnRightSwitchReleased = 0;

	// Used for key events, to reset the trip meter to 0 on the key onRising event
	static bool lastKey = false;

	// It's necessary to save the odometer each one KM and on the key on falling event
	// The variables below are used for this
	static double lastOdometerKm = 0;
	double currentOdometerKm = 0;

	// Used in the 12V monitor
	// 80 = R1+R2, 12 = R2, look below for more info
	const double multVoltageDivider = (68 + 12) / 12;

	while (true)
	{
		// Start execution debug
		task_UpdateIO.initDebugExec();

		// ECU Supply voltage monitor
		// The ESP32 analog reader has 12 bits, that means that the analog value goes from 0 to 4095
		// So first get the voltage in the pin, finding the equivalent number of the analog read between 0 and 3.3
		uint16_t auxAnalogRead = analogRead(pin12VMonitor);
		double auxECUSupplyVoltage = avgECUSupplyVoltage.addNCalcAvg(auxAnalogRead);
		auxECUSupplyVoltage = auxECUSupplyVoltage * 3.3 / 4095.0;
		// Once the voltage is calculated, it's necessary to calculate the real voltage, using the voltage divide ratio in the PCB
		// The voltage divide is done by 68K as R1 and 12K as R2. So the simplified calc can be: voltageRead * ((68+12)/12)
		auxECUSupplyVoltage = auxECUSupplyVoltage * multVoltageDivider;
		// The voltage measured was not the correct one, but the difference between the real and the measured was kind of linear
		// So Excel was used to calculate the trend line and the equation used below was extracted from there
		auxECUSupplyVoltage = 1.105 * auxECUSupplyVoltage + 1.15;
		vehicleInfo.boardSupplyVoltage = auxECUSupplyVoltage;

		// Map the outputs
		outputs.led = outputs.ledHeartbeat;

		// Debounce the inputs and save them in the structure of vehicle inputs
		debounce(inputs.in2, 2, &ctModeButton, &inputs.modeButton);				  // in2
		debounce(inputs.in8, 2, &ctHeadlightHighBeam, &inputs.headlightHighBeam); // in8
		debounce(inputs.in9, 2, &ctTurnLeftSwitch, &inputs.turnLeftSwitch);		  // in9
		debounce(inputs.in10, 2, &ctTurnRightSwitch, &inputs.turnRightSwitch);	  // in10
		debounce(inputs.in11, 2, &ctFastChargeSwitch, &inputs.fastChargeSwitch);  // in11
		debounce(inputs.in12, 2, &ctKey, &inputs.key);							  // in12

		// The kickstand and killswitch input are high when they are disabled, but it makes more sense
		// to work with them inverted, being high when they are activated. So in the lines below the
		// physical inputs are reflected inverted to the variables within the struct input
		debounce(!inputs.in7, 2, &ctKickstand, &inputs.kickstand);	 // in7
		debounce(!inputs.in6, 2, &ctKillswitch, &inputs.killswitch); // in6

		// ====================================================================
		// Pressing the mode button will change from modes City to PRO, but holding for 3 seconds will put in reverse mode
		// If the vehicle has the option to reverse, it'll pay attention to the 5 seconds
		// If not, it'll just change modesF
		/* #region modeButton events */
		if (vehicleParam.hasOptionToReverse)
		{
			// When the button is pressed, the millis is saved and put the flag that the button was pressed as true
			if ((inputs.modeButton != lastModeButton) && (inputs.modeButton))
			{
				millisModeButtonPressed = millis();
				flagModeButtonPressed = true;
			}

			// If the flag is true and passed 3 seconds or more since the button was pressed, put reverse as true and clean the flag
			if ((flagModeButtonPressed) && (millis() - millisModeButtonPressed >= 3000))
			{
				vehicleInfo.isInReverseMode = true;
				flagModeButtonPressed = false;
			}

			// If the button is released
			if ((inputs.modeButton != lastModeButton) && (!inputs.modeButton))
			{
				// First check if it was in reverse
				if (vehicleInfo.isInReverseMode)
				{
					vehicleInfo.isInReverseMode = false;
				}
				// If not, changes riding modes
				else
				{
					flagChangeRidingMode = true;
				}

				// Remove that flag that indicated that the button was pressed
				flagModeButtonPressed = false;
			}

			// Track changes in the reverse mode
			if (vehicleInfo.isInReverseMode != lastReverseMode)
			{
				// Say to the function that prepares the packet to be sent to the MCU that it has a change in the reverse mode
				MCUData.reverseModeChanged = true;
			}
			// Track the last state of the reverse mode
			lastReverseMode = vehicleInfo.isInReverseMode;
		}
		else
		{
			// If the state changed, it is down (it means event onFalling), updates the riding mode
			if ((inputs.modeButton != lastModeButton) && (!inputs.modeButton))
			{
				flagChangeRidingMode = true;
			}
		}
		// Track the last state of the mode button
		lastModeButton = inputs.modeButton;
		/* #endregion */

		// ====================================================================
		// Delay the turn switches to show in the display
		// The relay changes the input, so it's necessary to delay the signal to keep showing
		// the signal in the screen, that does the animation by itself
		/* #region turnLeftSwitch events */
		// If the input rises, put the delayed input as high
		if ((inputs.turnLeftSwitch != lastTurnLeftSwitch) && (inputs.turnLeftSwitch))
		{
			inputs.turnLeftSwitchDelayed = true;
			flagTurnLeftSwitchReleased = false;
		}
		// If the input falls, put the released flag as high and save the millisecond of this action
		if ((inputs.turnLeftSwitch != lastTurnLeftSwitch) && (!inputs.turnLeftSwitch))
		{
			millisTurnLeftSwitchReleased = millis();
			flagTurnLeftSwitchReleased = true;
		}
		// If the released flag is high and it passed 750 milliseconds, put the delayed input as low
		if ((flagTurnLeftSwitchReleased) && (millis() - millisTurnLeftSwitchReleased >= 750))
		{
			inputs.turnLeftSwitchDelayed = false;
			flagTurnLeftSwitchReleased = false;
		}
		// Keep track of last state of turn left switch
		lastTurnLeftSwitch = inputs.turnLeftSwitch;
		/* #endregion */

		// ====================================================================
		/* #region turnRightSwitch events */
		// If the input rises, put the delayed input as high
		if ((inputs.turnRightSwitch != lastTurnRightSwitch) && (inputs.turnRightSwitch))
		{
			inputs.turnRightSwitchDelayed = true;
			flagTurnRightSwitchReleased = false;
		}
		// If the input falls, put the released flag as high and save the millisecond of this action
		if ((inputs.turnRightSwitch != lastTurnRightSwitch) && (!inputs.turnRightSwitch))
		{
			millisTurnRightSwitchReleased = millis();
			flagTurnRightSwitchReleased = true;
		}
		// If the released flag is high and it passed 750 milliseconds, put the delayed input as low
		if ((flagTurnRightSwitchReleased) && (millis() - millisTurnRightSwitchReleased >= 750))
		{
			inputs.turnRightSwitchDelayed = false;
			flagTurnRightSwitchReleased = false;
		}
		// Keep track of last state of turn right switch
		lastTurnRightSwitch = inputs.turnRightSwitch;
		/* #endregion */

		// ====================================================================
		// Resets the trip meter on the key onRising event
		/* #region key events */
		// Put 0 in the trip meter
		if ((lastKey != inputs.key) && (inputs.key))
		{
			vehicleInfo.tripKm = 0;
		}

		// Save odometer in FRAM each 1 KM or in the key onFalling event
		currentOdometerKm = vehicleInfo.odometerKm;
		if ((currentOdometerKm - lastOdometerKm >= 1.0) || ((lastKey != inputs.key) && (!inputs.key)))
		{
			paramFRAM[p_odometerKm].requestWrite(vehicleInfo.odometerKm);
			lastOdometerKm = currentOdometerKm;
		}
		lastKey = inputs.key;
		/* #endregion */

		// End execution debug
		task_UpdateIO.endDebugExec(uxTaskGetStackHighWaterMark(NULL));

		// Put the task to sleep
		vTaskDelay(task_UpdateIO.getInterval() / portTICK_PERIOD_MS);
	}
}