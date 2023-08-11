/******************************************************************************
 * task_ModeSelector.cpp
 * This file has the code for task that changes the riding mode according to
 * the button or system conditions
 *****************************************************************************/

#include "task_ModeSelector.h"

// Code for the task ModeSelector
void taskCode_ModeSelector(void *pvParameters)
{
    // Aux variables for trigger weather mode
    static uint32_t startTime = 0;
	static uint32_t endTime = 0;
    static uint32_t interval = 180000;  // Wait 3 minutes for trigger winter mode
	// Init CheckUp
	// Make sure that at least one packet from BMS and MCU was received
	while (errorArray.get(300) || errorArray.get(302))
	{
		// Keep this variable as false while the initial conditions were not met
		vehicleInfo.passedInitCheckup = false;

		// Put task to sleep
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}

	// If it passed the loop, it means that the initial conditions were met, then put the variable as true
	vehicleInfo.passedInitCheckup = true;

	// Always start in CITY mode
	vehicleInfo.ridingMode = 1;

	// Used for key events, to force CITY mode when the key in turned on
	static bool lastKey = false;

	// Keep track of the last riding mode if the change happened because an error, to come back if the error is not active anymore
	static uint8_t lastRidingMode;

	while (true)
	{
		// Start execution debug
		task_ModeSelector.initDebugExec();

		// Priority of riding mode
		// 1 ECO mode is errors from the array errorsKeepEcoMode are happening
		// 2 ECO mode if SOC is less than 10
		// 3 CITY mode is errors from the array errorsKeepCityMode are happening
		// 4 CITY mode if SOC is less than 40
		// 5 CITY mode if key is turned on (Useful for when it's charging and the key is turned on without disconnecting the charger)
		// 6 normal operation

		// Keep in ECO mode if any problem of the array errorsKeepEcoMode is happening
		if (checkErrorsKeepEcoMode())
		{
			// If already change because of an error, just keep in ECO mode
			if (vehicleInfo.ridingModeChangedBecauseOfError)
			{
				vehicleInfo.ridingMode = 0;
			}
			// If not, flags that the change happended because of an error and keep track of the last riding mode
			else
			{
				// Flags the reason
				vehicleInfo.ridingModeChangedBecauseOfError = true;

				// Save the actual riding mode, to be back after solve the error
				lastRidingMode = vehicleInfo.ridingMode;

				// Keep in ECO mode
				vehicleInfo.ridingMode = 0;
			}
		}
		// If the SOC is less or equal than 10% KEEP in ECO Mode
		// Added condition of time(millis function) for avoid get in the ECO mode
		// in the startup process because of delay of the BMS response  
		else if (BMSData.SOC <= 10&&millis()>6000)
		{
			vehicleInfo.ridingMode = 0;
		}
		// Keep in CITY mode if any problem of the array errorsKeepCityMode is happening
		else if (checkErrorsKeepCityMode())
		{
			// If already change because of an error, just keep in CITY mode
			if (vehicleInfo.ridingModeChangedBecauseOfError)
			{
				vehicleInfo.ridingMode = 1;
			}
			else
			{
				// Save the actual riding mode, to be back after solve the error
				vehicleInfo.ridingModeChangedBecauseOfError = true;

				// Save the actual riding mode, to be back after solve the error
				lastRidingMode = vehicleInfo.ridingMode;

				// Keep in CITY mode
				vehicleInfo.ridingMode = 1;
			}
		}
		// If the SOC is less or equal than 40% KEEP in CITY Mode
		else if (BMSData.SOC <= 40)
		{
			vehicleInfo.ridingMode = 1;
		}
		// If the key was turned on, KEEP in CITY Mode
		else if ((lastKey != inputs.key) && (inputs.key))
		{
			vehicleInfo.ridingMode = 1;
		}
		// If it's normal
		else
		{
			// If the change happened because an error
			if (vehicleInfo.ridingModeChangedBecauseOfError)
			{
				// Come back to the riding mode that it was before the error
				vehicleInfo.ridingMode = lastRidingMode;

				// And remove the flag
				vehicleInfo.ridingModeChangedBecauseOfError = false;
			}

			// And look to the mode button events
			if (flagChangeRidingMode)
			{
				if (vehicleInfo.ridingMode == 1)
				{
					vehicleInfo.ridingMode = 2;
				}
				else
				{
					vehicleInfo.ridingMode = 1;
				}
			}
		}

		// Used to track changes of the key
		lastKey = inputs.key;

		// Remove the flag that indicates changes in the riding mode, because it was already been analyzed
		flagChangeRidingMode = false;

		// Select Weather conditions mode
		 if (vehicleInfo.boardTemperature<10&&BMSData.avgBatteryTemperature<10 ) {

			 if (endTime==0){
             startTime = millis();
		     }
			 endTime = millis();
		 }
		 else
		 {
			startTime = 0;
			endTime = 0;
		 }	
			if ( endTime -  startTime >= interval) {
			vehicleInfo.WinterMode == true;
			}
			else
			vehicleInfo.WinterMode == false;

		// End execution debug
		task_ModeSelector.endDebugExec(uxTaskGetStackHighWaterMark(NULL));

		// Put the task to sleep
		vTaskDelay(task_ModeSelector.getInterval() / portTICK_PERIOD_MS);
	}
}
