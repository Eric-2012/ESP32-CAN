/******************************************************************************
 * func_Errors.cpp
 * This file has all the things that are related with errors, such as functions,
 * variables and definitions
 *****************************************************************************/

#include "func_Errors.h"

// Update the error array and the variable vehicleInfo.errorsDetected in the end of each check cycle
void updateErrorArray(uint16_t errorCode, bool value, bool endCheckCycle)
{
    // Auxiliary variable for error detection
    static bool auxErrorsDetected = false;
    static uint16_t auxNumActiveErrors = 0;
    static uint32_t auxSumActiveErrors = 0;
    // If it is the end of the check cycle
    if (endCheckCycle)
    {
        // Mirrors the values of the auxiliaries variables for the variable in the global struct
        vehicleInfo.errorsDetected = auxErrorsDetected;
        vehicleInfo.numActiveErrors = auxNumActiveErrors;
        vehicleInfo.sumActiveErrors = auxSumActiveErrors;

        // And prepares the auxiliaries variables for a new verification cycle
        auxErrorsDetected = false;
        auxNumActiveErrors = 0;
        auxSumActiveErrors = 0;
    }
    // If it is NOT the end of the check cycle
    else
    {
        // Put the error flag as true in case of any error will be found, increment the counter and sum
        if (value)
        {
            auxErrorsDetected = true;
            auxNumActiveErrors++;
            auxSumActiveErrors += errorCode;
        }
		// Set the error array
        errorArray.set(errorCode, value);
    }
}

// Check if the bike can run
bool checkIfBikeCanRun()
{
	// The array errorsKeepStopped  in the constants file has all the error codes that the motor cannot work
	// So as the values are uint16, the number of errors is the size of the array, divided by 2
	uint16_t numberOfErrorsKeepStopped  = sizeof(errorsKeepStopped) / 2;

	// It goes arround the array, and if any of the errors will be true, the functions goes out, returning false
	for (uint16_t x = 0; x < numberOfErrorsKeepStopped; x++)
	{
		if (errorArray.get(errorsKeepStopped[x]))
		{
			return false;
		}
	}

	// At end, checks if the bike is charging
	if (vehicleInfo.isChargerConnected)
	{
		return false;
	}
	else
	{
		return true;
	}
}

// Check some of the errors that will limit 50% of power is happening
bool checkErrors50Power()
{
	// The number of errors is equal of the byte size of the array divided by 2
	uint16_t numErrors50Power = sizeof(errors50Power) / 2;

	// If any error was found, return true
	for (uint16_t x = 0; x < numErrors50Power; x++)
	{
		if (errorArray.get(errors50Power[x]))
		{
			return true;
		}
	}

	// If there are not errors happening, return false
	return false;
}

// Check some of the errors that will keep in city mode is happening
bool checkErrorsKeepCityMode()
{
	// The number of errors is equal of the byte size of the array divided by 2
	uint16_t numErrorsKeepCityMode = sizeof(errorsKeepCityMode) / 2;

	// If any error was found, return true
	for (uint16_t x = 0; x < numErrorsKeepCityMode; x++)
	{
		if (errorArray.get(errorsKeepCityMode[x]))
		{
			return true;
		}
	}

	// If there are not errors happening, return false
	return false;
}

// Check some of the errors that will keep in eco mode is happening
bool checkErrorsKeepEcoMode()
{
	// The number of errors is equal of the byte size of the array divided by 2
	uint16_t numErrorsKeepEcoMode = sizeof(errorsKeepEcoMode) / 2;

	// If any error was found, return true
	for (uint16_t x = 0; x < numErrorsKeepEcoMode; x++)
	{
		if (errorArray.get(errorsKeepEcoMode[x]))
		{
			return true;
		}
	}

	// If there are not errors happening, return false
	return false;
}