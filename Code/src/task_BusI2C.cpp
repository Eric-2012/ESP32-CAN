/******************************************************************************
 * task_BusI2C.cpp
 * This file has the code for task I2C, that will take care of the I2C bus
 *****************************************************************************/

#include "task_BusI2C.h"

// Code for the task I2C
void taskCode_BusI2C(void *pvParameters)
{
	//setTime(0, 35, 20, 3, 27, 4, 21);

	// Variables used to update the RTC 4 times each second
	static uint32_t currentMillis = 0;
	static uint32_t lastMillisRTCUpdate = 0;

	// Force Odometer
	//vehicleInfo.odometerKm = 100;
	//paramFRAM[p_odometerKm].write(vehicleInfo.odometerKm);

	while (true)
	{
		// Start execution debug
		task_BusI2C.initDebugExec();

		// I2C Expanders each cycle
		updateIOs();

		// RTC Device each 250 ms
		currentMillis = millis();
		if (currentMillis - lastMillisRTCUpdate >= 250)
		{
			updateRTC();
			lastMillisRTCUpdate = currentMillis;
		}

		// Check if there is an update of parameters scheduled
		for (uint8_t x = 0; x < maxNumParameters; x++)
		{
			if (paramFRAM[x].isConfigured())
			{
				paramFRAM[x].checkNWrite();
			}
		}

		// Check if it's necessary to read from FRAM
		if (flagToReadFromFRAM)
		{
			flagToReadFromFRAM = false;
			readAllParameters();
		}

		// End execution debug
		task_BusI2C.endDebugExec(uxTaskGetStackHighWaterMark(NULL));

		// Put the task to sleep
		vTaskDelay(task_BusI2C.getInterval() / portTICK_PERIOD_MS);
	}
}