/******************************************************************************
 * task_Display.cpp
 * This file has the code for task SaveToMemories and related functions
 *****************************************************************************/

#include "task_ScreenUART.h"

// UART screen instance
DwinPanelDisplay dwinPanel(0x01, 0x00);

// If there is an error and the cycle to show the error message starts, show all the errors till the end before remove the icon and triangle
static bool showingErrorMessages = false;

// Code for the task that shows in the display
void taskCode_ScreenUART(void *pvParameters)
{
    // Start the serial instance
    SerialPanel.begin(115200, SERIAL_8N1, pinRXD2, pinTXD2);

    // Starts the screen
    // It's necessary to run the commands to start the screen when the ECU is powered on and when the key is activated
    // It needs to be this way because when the charger is connected, the ECU is also powered on, but the screen not
    // So keeping track of just the last state of the key and starting the lastKeyState as false will make works for both ways
    static bool lastKeyState = false; // Keep track of the last key state
    static uint32_t lastUpdate = 0;

    while (true)
    {
        // Start execution debug
        task_ScreenUART.initDebugExec();

        // Starts the screen on the key onRising event (it happens on the event and when the ECU is poweredOn)
        if ((lastKeyState != inputs.key) && (inputs.key))
        {
            // A delay is necessary when the screen is powered on, before start do send commands
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            startScreen();
        }
        lastKeyState = inputs.key;

        // The screen will just be on if the screen is on, so the key can be used as a condition to send something to the screen
        if (inputs.key)
        {
            // If the vehicle is stored, shows the storage mode screen
            if (vehicleInfo.isStored)
            {
                dwinPanel.setCurrentPage(dwinPanel.screenIsStored);
                dwinPanel.updateDisplayPage();
            }
            // If the vehicle is charging and the current is equal or bigger 1 amp, shows the charging mode screen
            else if (vehicleInfo.isCharging)
            {
                dwinPanel.setCurrentPage(dwinPanel.screenIsCharging);
                dwinPanel.updateDisplayPage();

                // Shows the SOC in the charging screen
                dwinPanel.writeTextSOCCharging(BMSData.SOC);
            }
            // If the vehicle is locked, shows the locked screen
            else if (vehicleInfo.isLocked)
            {
                dwinPanel.setCurrentPage(dwinPanel.screenIsLocked);
                dwinPanel.updateDisplayPage();
            }
            // Otherwise shows the riding screen (homepage)
            else
            {
                // Just to have sure that the info shown will be accurate, a refresh each 1000 ms is forced
                if (millis() - lastUpdate >= 500)
                {
                    lastUpdate = millis();
                    forceUpdate();
                }

                // If the speed unit is KM, shows the screen with KM
                if (vehicleParam.speedUnit == 0)
                    dwinPanel.setCurrentPage(dwinPanel.screenRidingKMH);
                // If the speed unit is miles, shows the screen with miles
                else if (vehicleParam.speedUnit == 1)
                    dwinPanel.setCurrentPage(dwinPanel.screenRidingMPH);

                // Update just if there is changes
                dwinPanel.updateDisplayPage();

                // If the vehicle is in reverse mode, blinks the warning signal and write REVERSE in the text box
                // Just if the ECU throttle protection error is not triggered, because in this case the driver needs to know
                if ((vehicleInfo.isInReverseMode) && (!errorArray.get(312)))
                {
                    char buffer[] = "REVERSE";          //warning message must be avoided
                    dwinPanel.writeTextWarning(buffer); // Write the word REVERSE
                    blinkIconWarning();                 // Remove the warning signal

                    if (vehicleInfo.errorsDetected)
                    {
                        dwinPanel.showIconOBD(true); // Show the ODB Icon
                    }
                    else
                    {
                        dwinPanel.showIconOBD(false); // Remove the ODB Icon
                    }
                }
                // If it's not in reverse and there are error, it shows the error code
                // OR if is in the middle of a showing messages cycle
                else if (vehicleInfo.errorsDetected || showingErrorMessages)
                {
                    // Shows all code errors that are happening at the moment
                    showErrorMessages();

                    dwinPanel.showIconWarning(true); // Shows the warning signal
                    dwinPanel.showIconOBD(true);     // Shows the ODB Icon
                }
                // Otherwise, doesn't show anything in the warning place
                else
                {
                    char buffer[] = "";                 // Necessary to avoid warning message
                    dwinPanel.writeTextWarning(buffer); // Write nothing

                    dwinPanel.showIconWarning(false); // Remove the warning signal
                    dwinPanel.showIconOBD(false);     // Remove the ODB Icon
                }

                // Writes the BMS Temperature
                dwinPanel.writeTextBMSTemp((int16_t)BMSData.avgBatteryTemperature);

                // Shows the SOC
                dwinPanel.showGraphSOC((uint16_t)BMSData.SOC);

                // Shows the motor temperature
                dwinPanel.showGraphMotorTemperature((int16_t)MCUData.motorTemperature);

                // Shows the BMS Current
                dwinPanel.showGraphCurrent((int16_t)BMSData.current);

                // Check the units to show speed, trip and odometer
                if (vehicleParam.speedUnit == 0)
                {
                    dwinPanel.writeTextSpeed((uint16_t)vehicleInfo.speedKmh);
                    dwinPanel.writeTextTrip((uint16_t)vehicleInfo.tripKm);
                    dwinPanel.writeTextOdometer((uint32_t)vehicleInfo.odometerKm);
                }
                else if (vehicleParam.speedUnit == 1)
                {
                    dwinPanel.writeTextSpeed((uint16_t)convertKmhToMph(vehicleInfo.speedKmh));
                    dwinPanel.writeTextTrip((uint16_t)convertKmToM(vehicleInfo.tripKm));
                    dwinPanel.writeTextOdometer((uint32_t)convertKmToM(vehicleInfo.odometerKm));
                }

                // Shows the MCU Voltage
                dwinPanel.showGraphMCUVoltage((uint16_t)MCUData.voltage);

                // Shows the riding mode
                // 0=Eco, 1=City, 2=Pro, 4=Eco (Red), 5=City (Red), 6=Pro (Red)
                dwinPanel.showIconRidingMode((uint16_t)vehicleInfo.ridingMode);

                // Not defined when to show
                dwinPanel.showIconBattery(false);

                // Shows turn left icon
                dwinPanel.showIconTurnLeft(inputs.turnLeftSwitchDelayed);

                // Shows turn right icon
                dwinPanel.showIconTurnRight(inputs.turnRightSwitchDelayed);

                // Shows highbeam icon
                dwinPanel.showIconHighbeam(inputs.headlightHighBeam);

                // Show charging icon
                dwinPanel.showIconCharging(vehicleInfo.isChargerConnected);
            }
        }

        // End execution debug
        task_ScreenUART.endDebugExec(uxTaskGetStackHighWaterMark(NULL));

        // Put the task to sleep
        vTaskDelay(task_ScreenUART.getInterval() / portTICK_PERIOD_MS);
    }
}

// Blink the yellow triangle
void blinkIconWarning()
{
    static uint32_t lastMillis = 0;
    static uint32_t interval = 500;
    static bool show = false;

    if (millis() - lastMillis >= interval)
    {
        lastMillis = millis();
        show = !show;
        dwinPanel.showIconWarning(show);
    }
}

// Shows all code errors that are happening at the moment
void showErrorMessages()
{
    // Time variables used to change the text
    static uint32_t lastMillis = 0;
    static uint32_t interval = 2000;

    // Auxiliary variable used to show error code in the display
    static uint16_t auxScan = 0;
    char temp[4];
    char errorMessage[20];

    // Before starts to show the fist error, make a copy of the errorArray and sinalizes that it will start to show errors
    if (auxScan == 0)
    {
        // Keep saved the error array
        errorArrayTemp = errorArray;

        // Sinalizes that started a new cycle of showing messages
        showingErrorMessages = true;
    }

    // Each moment that the interval will pass, it will scan error array looking for negative values, that means that the error is true
    // starting from zero or from the last error shown
    if (millis() - lastMillis >= interval)
    {
        lastMillis = millis();

        while (auxScan < 1000)
        {
            // Analyses the actual error array and the one as it was in the beggining of the cycle to show error messages
            // To make sure that an error will be shown at least one time
            if (errorArray.get(auxScan) || errorArrayTemp.get(auxScan))
            {
                // Keep the format EXXX to search for the error message
                sprintf(temp, "E%03d", auxScan);

                // Get the message in the array of strings
                uint16_t x;
                for (x = 0; x < numberOfErrors; x++)
                {
                    // Get the error code from the array
                    errorMessages[x].substring(0, 4).toCharArray(errorMessage, sizeof(errorMessage));

                    // Compares to discover the position of the array where is the message
                    if (strcmp(temp, errorMessage) == 0)
                    {
                        break;
                    }
                }

                // Put the string in the char array to be written in the display
                errorMessages[x].toCharArray(errorMessage, sizeof(errorMessage));
                // Write in the display
                dwinPanel.writeTextWarning(errorMessage);

                // Even that the error was found, increments aux to prepare for next code that needs to be shown
                auxScan++;
                break;
            }

            // Increments aux
            auxScan++;
        }

        // Keep aux between 0 and the number of active error codes
        if (auxScan >= 1000)
        {
            // Put the index back to 0
            auxScan = 0;

            // Sinalizes that the cycle of showing messages ended
            showingErrorMessages = false;
        }
    }
}

// Starts the screen
void startScreen()
{
    // Disable the touchscreen
    dwinPanel.disableTouch();

    // If the speed unit is KM, shows the screen with KM
    if (vehicleParam.speedUnit == 0)
        dwinPanel.setCurrentPage(dwinPanel.screenRidingKMH);
    // If the speed unit is miles, shows the screen with miles
    else if (vehicleParam.speedUnit == 1)
        dwinPanel.setCurrentPage(dwinPanel.screenRidingMPH);

    // Force an update in the screen
    dwinPanel.updateDisplayPage();                    // Update the internal variable "previousPage"
    dwinPanel.changePage(dwinPanel.getCurrentPage()); // Force an update if it was not done by the last command

    // Force an update in the screen values
    char buffer[] = "";
    dwinPanel.writeTextTopLeft(buffer, true);
    dwinPanel.writeTextBMSTemp((int16_t)BMSData.avgBatteryTemperature, true);
    dwinPanel.showGraphSOC((uint16_t)BMSData.SOC, true);
    dwinPanel.showGraphMotorTemperature((int16_t)MCUData.motorTemperature, true);
    dwinPanel.showGraphCurrent((int16_t)BMSData.current, true);
    dwinPanel.showGraphMCUVoltage((uint16_t)MCUData.voltage, true);
    dwinPanel.showIconRidingMode((uint16_t)vehicleInfo.ridingMode, true);
    dwinPanel.showIconWarning(false, true);
    dwinPanel.writeTextWarning(buffer, true);
    dwinPanel.showIconBattery(false, true);
    dwinPanel.showIconOBD(false, true);
    dwinPanel.showIconTurnLeft(false, true);
    dwinPanel.showIconTurnRight(false, true);
    dwinPanel.showIconHighbeam(false, true);
    dwinPanel.showIconCharging(false, true);
    dwinPanel.writeTextSOCCharging((uint16_t)BMSData.SOC, true);

    if (vehicleParam.speedUnit == 0)
    {
        dwinPanel.writeTextSpeed((uint16_t)vehicleInfo.speedKmh, true);
        dwinPanel.writeTextTrip((uint16_t)vehicleInfo.tripKm, true);
        dwinPanel.writeTextOdometer((uint32_t)vehicleInfo.odometerKm, true);
    }
    else if (vehicleParam.speedUnit == 1)
    {
        dwinPanel.writeTextSpeed((uint16_t)convertKmhToMph(vehicleInfo.speedKmh), true);
        dwinPanel.writeTextTrip((uint16_t)convertKmToM(vehicleInfo.tripKm), true);
        dwinPanel.writeTextOdometer((uint32_t)convertKmToM(vehicleInfo.odometerKm), true);
    }
}

// Force an update in the screen each 1 second
void forceUpdate()
{
    dwinPanel.writeTextBMSTemp((int16_t)BMSData.avgBatteryTemperature, true);
    dwinPanel.showGraphSOC((uint16_t)BMSData.SOC, true);
    dwinPanel.showGraphMotorTemperature((int16_t)MCUData.motorTemperature, true);
    dwinPanel.showGraphCurrent((int16_t)BMSData.current, true);
    dwinPanel.showGraphMCUVoltage((uint16_t)MCUData.voltage, true);
    dwinPanel.showIconRidingMode((uint16_t)vehicleInfo.ridingMode, true);
    dwinPanel.writeTextSOCCharging((uint16_t)BMSData.SOC, true);

    if (vehicleParam.speedUnit == 0)
    {
        dwinPanel.writeTextSpeed((uint16_t)vehicleInfo.speedKmh, true);
        dwinPanel.writeTextTrip((uint16_t)vehicleInfo.tripKm, true);
        dwinPanel.writeTextOdometer((uint32_t)vehicleInfo.odometerKm, true);
    }
    else if (vehicleParam.speedUnit == 1)
    {
        dwinPanel.writeTextSpeed((uint16_t)convertKmhToMph(vehicleInfo.speedKmh), true);
        dwinPanel.writeTextTrip((uint16_t)convertKmToM(vehicleInfo.tripKm), true);
        dwinPanel.writeTextOdometer((uint32_t)convertKmToM(vehicleInfo.odometerKm), true);
    }
}