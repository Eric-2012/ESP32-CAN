/******************************************************************************
 * func_Interrupts.cpp
 * In this file are the functions called by interruptions when they happen,
 * it can be by a interruption pin or an event from the task UpdateIO
 *****************************************************************************/

#include "func_Interrupts.h"

// If the speed sensor rose
void ifSpeedSensorRose()
{
	// Internal variables for this interruption
	static uint32_t currentMicros = 0;
	static uint32_t elapsedTime = 0;

	// BEGIN - Calculate RPM
	currentMicros = micros();

	// As the micros counter overflows approximately every 70 minutes, it's necessary to have a protection that
	// in case happening, that means that the currentMicros is less that when the lastMicros, it will
	// calculate the duration by getting the bigger number of 32 bits, removing the lastMicros and adding the currentMicros
	if (currentMicros < lastSpeedSensorPulse)
	{
		elapsedTime = (0xFFFFFFFF - lastSpeedSensorPulse) + currentMicros;
	}
	// In case of not overflow, it just calculates the difference
	else
	{
		elapsedTime = currentMicros - lastSpeedSensorPulse;
	}

	// 1 minute has 60.000.000 microseconds, so the number of RPM is this number divided by the duration of a full rotation
	// As each magnet causes an interruption, the full rotation time is the elapsed time X the number of magnets
	vehicleInfo.rpm = 60000000 / (elapsedTime * vehicleParam.numMagnetsSpeedSensor);

	// Save the time that the last magnet was detected
	lastSpeedSensorPulse = currentMicros;
	// END - Calculate RPM

	// Calculates the speed, odometer and trip meter, based on the RPM
	motionAnalysis();
}

// Used for the ticker that generates a heartbeat in the led on the ECU
void heartbeat()
{
  outputs.ledHeartbeat = !outputs.ledHeartbeat;
}

// Used for the ticker that force the RPM to 0 if the bike is stopped when there is trasmission and the rpm comes from the speedsensor
void forceRPMTo0() {
	// If the last pulse happened more than 1 second ago, forces the RPM to 0 and calls the motionAnalysis function to calculate speed
	if (micros() - lastSpeedSensorPulse > 1000000) {
		vehicleInfo.rpm = 0;
		motionAnalysis();
	}
}