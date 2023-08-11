/******************************************************************************
 * task_BMSUART.cpp
 * In this file is the function to get data from the BMS
 *****************************************************************************/

#include "task_BMSUART.h"

// Code for the task to get data from the BMS
void taskCode_BMSUART(void *pvParameters)
{
    // Start the serial instance
    SerialBMS.begin(19200, SERIAL_8N1, pinRXD1, pinTXD1);

    // Internal variables for this task
    const uint8_t BMSrequest[] = {0x5A, 0x5A, 0x00, 0x00, 0x00, 0x00};
    const uint8_t BMSheader[] = {0xAA, 0X55, 0XAA, 0XFF};
    const uint16_t BMSTimeOut = 250;
    const uint8_t BMSPacketSize = 140;

    static uint8_t BMSArrayIndex = 0;
    static uint8_t BMSArray[BMSPacketSize];
    static uint16_t CRC;
    static uint32_t requestMillis;

    // Variable used to check if the SOC is accurate
    // If the last SOC is different from 0 and the actual SOC is 0, count for some cycles to have sure that 0 is 0 instead of just bad data
    static uint8_t auxSOC;
    static uint8_t lastSOC;
    static uint8_t countSOC0;

    while (true)
    {        
        // Start execution debug
        task_BMSUART.initDebugExec();

        // If it is not receiving data from the BMS, request it
        if (!BMSData.BMSReceiving)
        {
            // Delay for DEBUG
            // delay(1000);

            // Clear serial buffer
            if (SerialBMS.available())
            {
                while (SerialBMS.available())
                {
                    SerialBMS.read();
                }
            }

            // Clear the BMSArray
            for (uint8_t x = 0; x < BMSPacketSize; x++)
            {
                BMSArray[x] = 0;
            }

            // Send request
            for (uint8_t x = 0; x < 6; x++)
            {
                SerialBMS.write(BMSrequest[x]);
                SerialBMS.flush();
            }

            // Get the time that the request was done to detect the timeout error
            requestMillis = millis();

            // Put the index used to fill the array as 0
            BMSArrayIndex = 0;

            // Put the CRC as 0, because a new packet will start
            CRC = 0;

            // Inform that it is in a data receiving process
            BMSData.BMSReceiving = true;
        }
        else
        {
            // If it is in a data receiving process, get the data avaiable for read and put in the array, increasing the index each cycle
            if (SerialBMS.available())
            {
                while (SerialBMS.available())
                {
                    // Keep receiving till the index reach the BMS packet size
                    if (BMSArrayIndex < BMSPacketSize)
                    {
                        // Computes the CRC
                        CRC += SerialBMS.peek();

                        // Put the byte in the array
                        BMSArray[BMSArrayIndex] = SerialBMS.read();
                        BMSArrayIndex++;
                    }
                    // If the index will be bigger, it means that and error happened, the bad data counter need to be increased and the serial buffer needs to be clean
                    else
                    {
                        // Shows debug message if the flag is active
                        if (serialDebugFlags.get(1))
                        {
                            Serial.printf("BMS ERROR: More bytes than %u are coming.\n", BMSPacketSize);
                        }

                        // Clear serial buffer
                        if (SerialBMS.available())
                        {
                            while (SerialBMS.available())
                            {
                                SerialBMS.read();
                            }
                        }

                        // Increase the BMSBadDataCounter and go out of this funtion
                        BMSBadDataCounter();
                        return;
                    }
                }
            }
            else
            {
                // If it passed more milliseconds than informed in the variable BMSTimeOut since the request,
                // it means that it had a timeout problem and it's necessary to request again
                if (millis() - requestMillis >= BMSTimeOut)
                {
                    // Shows debug message if the flag is active
                    if (serialDebugFlags.get(1))
                    {
                        Serial.printf("BMS ERROR: TIMEOUT. %u bytes received. %u bytes were expected.\n", BMSArrayIndex, BMSPacketSize);

                        for (uint8_t x = 0; x < BMSArrayIndex; x++)
                        {
                            Serial.print(BMSArray[x]);
                            Serial.print(" ");
                        }
                        Serial.println();
                    }

                    // Increase the BMSBadDataCounter and go out of this funtion
                    BMSBadDataCounter();
                }
                // If the array was filled successfully, checks the header and CRC before use the data
                else if (BMSArrayIndex == BMSPacketSize)
                {
                    // Shows debug message if the flag is active
                    if (serialDebugFlags.get(1))
                    {
                        for (uint8_t x = 0; x < BMSPacketSize; x++)
                        {
                            Serial.print(BMSArray[x]);
                            Serial.print(" ");
                        }
                        Serial.println();
                    }

                    // Remove the header and CRC from CRC calc
                    CRC -= (BMSArray[0] + BMSArray[1] + BMSArray[2] + BMSArray[3] + BMSArray[138] + BMSArray[139]);

                    // Checks the header
                    // If header is not valid
                    if ((BMSArray[0] != BMSheader[0]) || (BMSArray[1] != BMSheader[1]) || (BMSArray[2] != BMSheader[2]) || (BMSArray[3] != BMSheader[3]))
                    {
                        // Shows debug message if the flag is active
                        if (serialDebugFlags.get(1))
                        {
                            Serial.println("BMS ERROR: Bad header");
                        }

                        // Increase the BMSBadDataCounter
                        BMSBadDataCounter();
                    }
                    // Checks the CRC
                    // If CRC is not valid
                    else if ((BMSArray[138] != ((uint8_t)(CRC >> 8))) || (BMSArray[139] != ((uint8_t)(CRC & 0x00FF))))
                    {
                        if (serialDebugFlags.get(1))
                        {
                            Serial.println("BMS ERROR: Bad CRC");
                        }

                        // Increase the BMSBadDataCounter and go out of this funtion
                        BMSBadDataCounter();
                    }
                    // Data is valid
                    else
                    {
                        // It it passed all checks, it means that this packet is done with success, the BMSBadDataCounter can be resetted and a new packet can be requested
                        BMSBadDataCounter(true);

                        // If it passed all verification, put the rest of the array in the structure
                        BMSData.voltage = ((double)uint8To_Uint16(BMSArray[4], BMSArray[5])) / 10.0;
                        BMSData.current = ((double)uint8To_Int32(BMSArray[70], BMSArray[71], BMSArray[72], BMSArray[73])) / 10.0;
                        auxSOC = BMSArray[74];
                        BMSData.batteryPhysicalCapacity = ((double)uint8To_Uint32(BMSArray[75], BMSArray[76], BMSArray[77], BMSArray[78])) / 1000000.0;
                        BMSData.batteryRemainingCapacity = ((double)uint8To_Uint32(BMSArray[79], BMSArray[80], BMSArray[81], BMSArray[82])) / 1000000.0;
                        BMSData.totalUsedCapacity = ((double)uint8To_Uint32(BMSArray[83], BMSArray[84], BMSArray[85], BMSArray[86])) / 1000.0;
                        BMSData.turnOnTime = uint8To_Uint32(BMSArray[87], BMSArray[88], BMSArray[89], BMSArray[90]);
                        BMSData.chargingMosfetStatus = BMSArray[103];
                        BMSData.dischargingMosfetStatus = BMSArray[104];
                        BMSData.balanceStateSign = BMSArray[105];
                        BMSData.wheelDiameter = uint8To_Uint16(BMSArray[106], BMSArray[107]);
                        BMSData.weeklyHeartbeat = uint8To_Uint16(BMSArray[108], BMSArray[109]);
                        BMSData.bmsSwitch = BMSArray[110];
                        BMSData.powerOut = uint8To_Int32(BMSArray[111], BMSArray[112], BMSArray[113], BMSArray[114]);
                        BMSData.cellHighestVoltageNumber = BMSArray[115];
                        BMSData.cellHighestVoltageValue = ((double)uint8To_Uint16(BMSArray[116], BMSArray[117])) / 1000.0;
                        BMSData.cellLowestVoltageNumber = BMSArray[118];
                        BMSData.cellLowestVoltageValue = ((double)uint8To_Uint16(BMSArray[119], BMSArray[120])) / 1000.0;
                        BMSData.averageCellsVoltage = ((double)uint8To_Uint16(BMSArray[121], BMSArray[122])) / 1000.0;
                        BMSData.numberOfBatteries = BMSArray[123];
                        BMSData.voltageDS = ((double)uint8To_Uint16(BMSArray[124], BMSArray[125])) / 10.0;
                        BMSData.dischargingMOSFETDriveVoltage = ((double)uint8To_Uint16(BMSArray[126], BMSArray[127])) / 10.0;
                        BMSData.chargingMOSFETDriveVoltage = ((double)uint8To_Uint16(BMSArray[128], BMSArray[129])) / 10.0;
                        BMSData.initialComparatorVoltage = uint8To_Uint16(BMSArray[130], BMSArray[131]);
                        BMSData.equalizationControl = uint8To_Uint32(BMSArray[132], BMSArray[133], BMSArray[134], BMSArray[135]);
                        BMSData.systemLog = uint8To_Uint16(BMSArray[136], BMSArray[137]);

                        // SOC analysis
                        // If the last SOC was not 0, the actual is 0 and the counter is still not active, activates the counter
                        if ((lastSOC != 0) && (auxSOC == 0) && (countSOC0 == 0))
                        {
                            countSOC0 = 5;
                        }
                        // If the counter is active, decreases it and keep the last SOC as actual
                        else if (countSOC0 > 0)
                        {
                            // Dreacreses the counter
                            countSOC0--;

                            // Keep the last SOC
                            BMSData.SOC = lastSOC;

                            // If the atual SOC is different than 0, put the counter as 0
                            if (auxSOC != 0)
                            {
                                countSOC0 = 0;
                            }
                        }

                        // When the counter is not active, just put the actual SOC in the structure and refresh the lastSOC
                        if (countSOC0 == 0)
                        {
                            BMSData.SOC = auxSOC;
                            lastSOC = auxSOC;
                        }

                        // ARRAYS
                        // Bytes 6-69
                        for (uint8_t i = 0, x = 6; i < 32; i++, x = x + 2)
                        {
                            BMSData.cellsVoltage[i] = ((double)uint8To_Uint16(BMSArray[x], BMSArray[x + 1])) / 1000.0;
                        }
                        // Bytes 91-102
                        for (uint8_t i = 0, x = 91; i < 6; i++, x = x + 2)
                        {
                            BMSData.temperatures[i] = uint8To_Int16(BMSArray[x], BMSArray[x + 1]);
                        }

                        // Calculates the average battery temperature
                        calcAvgBatteryTemperature();

                        // Calculates the vehicle efficiency
                        calcEfficiency();

                        // Shows debug message if the flag is active
                        if (serialDebugFlags.get(1))
                        {
                            Serial.print("voltage: ");
                            Serial.println(BMSData.voltage);

                            Serial.print("cellsVoltage[0]: ");
                            Serial.println(BMSData.cellsVoltage[0]);

                            Serial.print("cellsVoltage[1]: ");
                            Serial.println(BMSData.cellsVoltage[1]);

                            Serial.print("cellsVoltage[2]: ");
                            Serial.println(BMSData.cellsVoltage[2]);

                            Serial.print("cellsVoltage[3]: ");
                            Serial.println(BMSData.cellsVoltage[3]);

                            Serial.print("cellsVoltage[4]: ");
                            Serial.println(BMSData.cellsVoltage[4]);

                            Serial.print("cellsVoltage[5]: ");
                            Serial.println(BMSData.cellsVoltage[5]);

                            Serial.print("cellsVoltage[6]: ");
                            Serial.println(BMSData.cellsVoltage[6]);

                            Serial.print("cellsVoltage[7]: ");
                            Serial.println(BMSData.cellsVoltage[7]);

                            Serial.print("cellsVoltage[8]: ");
                            Serial.println(BMSData.cellsVoltage[8]);

                            Serial.print("cellsVoltage[9]: ");
                            Serial.println(BMSData.cellsVoltage[9]);

                            Serial.print("cellsVoltage[10]: ");
                            Serial.println(BMSData.cellsVoltage[10]);

                            Serial.print("cellsVoltage[11]: ");
                            Serial.println(BMSData.cellsVoltage[11]);

                            Serial.print("cellsVoltage[12]: ");
                            Serial.println(BMSData.cellsVoltage[12]);

                            Serial.print("cellsVoltage[13]: ");
                            Serial.println(BMSData.cellsVoltage[13]);

                            Serial.print("cellsVoltage[14]: ");
                            Serial.println(BMSData.cellsVoltage[14]);

                            Serial.print("cellsVoltage[15]: ");
                            Serial.println(BMSData.cellsVoltage[15]);

                            Serial.print("cellsVoltage[16]: ");
                            Serial.println(BMSData.cellsVoltage[16]);

                            Serial.print("cellsVoltage[17]: ");
                            Serial.println(BMSData.cellsVoltage[17]);

                            Serial.print("cellsVoltage[18]: ");
                            Serial.println(BMSData.cellsVoltage[18]);

                            Serial.print("cellsVoltage[19]: ");
                            Serial.println(BMSData.cellsVoltage[19]);

                            Serial.print("cellsVoltage[20]: ");
                            Serial.println(BMSData.cellsVoltage[20]);

                            Serial.print("cellsVoltage[21]: ");
                            Serial.println(BMSData.cellsVoltage[21]);

                            Serial.print("cellsVoltage[22]: ");
                            Serial.println(BMSData.cellsVoltage[22]);

                            Serial.print("cellsVoltage[23]: ");
                            Serial.println(BMSData.cellsVoltage[23]);

                            Serial.print("cellsVoltage[24]: ");
                            Serial.println(BMSData.cellsVoltage[24]);

                            Serial.print("cellsVoltage[25]: ");
                            Serial.println(BMSData.cellsVoltage[25]);

                            Serial.print("cellsVoltage[26]: ");
                            Serial.println(BMSData.cellsVoltage[26]);

                            Serial.print("cellsVoltage[27]: ");
                            Serial.println(BMSData.cellsVoltage[27]);

                            Serial.print("cellsVoltage[28]: ");
                            Serial.println(BMSData.cellsVoltage[28]);

                            Serial.print("cellsVoltage[29]: ");
                            Serial.println(BMSData.cellsVoltage[29]);

                            Serial.print("cellsVoltage[30]: ");
                            Serial.println(BMSData.cellsVoltage[30]);

                            Serial.print("cellsVoltage[31]: ");
                            Serial.println(BMSData.cellsVoltage[31]);

                            Serial.print("current: ");
                            Serial.println(BMSData.current);

                            Serial.print("SOC: ");
                            Serial.println(BMSData.SOC);

                            Serial.print("batteryPhysicalCapacity: ");
                            Serial.println(BMSData.batteryPhysicalCapacity);

                            Serial.print("batteryRemainingCapacity: ");
                            Serial.println(BMSData.batteryRemainingCapacity);

                            Serial.print("totalUsedCapacity: ");
                            Serial.println(BMSData.totalUsedCapacity);

                            Serial.print("turnOnTime: ");
                            Serial.println(BMSData.turnOnTime);

                            Serial.print("temperatures[0]: ");
                            Serial.println(BMSData.temperatures[0]);

                            Serial.print("temperatures[1]: ");
                            Serial.println(BMSData.temperatures[1]);

                            Serial.print("temperatures[2]: ");
                            Serial.println(BMSData.temperatures[2]);

                            Serial.print("temperatures[3]: ");
                            Serial.println(BMSData.temperatures[3]);

                            Serial.print("temperatures[4]: ");
                            Serial.println(BMSData.temperatures[4]);

                            Serial.print("temperatures[5]: ");
                            Serial.println(BMSData.temperatures[5]);

                            Serial.print("chargingMosfetStatus: ");
                            Serial.println(BMSData.chargingMosfetStatus);

                            Serial.print("dischargingMosfetStatus: ");
                            Serial.println(BMSData.dischargingMosfetStatus);

                            Serial.print("balanceStateSign: ");
                            Serial.println(BMSData.balanceStateSign);

                            Serial.print("wheelDiameter: ");
                            Serial.println(BMSData.wheelDiameter);

                            Serial.print("weeklyHeartbeat: ");
                            Serial.println(BMSData.weeklyHeartbeat);

                            Serial.print("bmsSwitch: ");
                            Serial.println(BMSData.bmsSwitch);

                            Serial.print("powerOut: ");
                            Serial.println(BMSData.powerOut);

                            Serial.print("cellHighestVoltageNumber: ");
                            Serial.println(BMSData.cellHighestVoltageNumber);

                            Serial.print("cellHighestVoltageValue: ");
                            Serial.println(BMSData.cellHighestVoltageValue);

                            Serial.print("cellLowestVoltageNumber: ");
                            Serial.println(BMSData.cellLowestVoltageNumber);

                            Serial.print("cellLowestVoltageValue: ");
                            Serial.println(BMSData.cellLowestVoltageValue);

                            Serial.print("averageCellsVoltage: ");
                            Serial.println(BMSData.averageCellsVoltage);

                            Serial.print("numberOfBatteries: ");
                            Serial.println(BMSData.numberOfBatteries);

                            Serial.print("voltageDS: ");
                            Serial.println(BMSData.voltageDS);

                            Serial.print("dischargingMOSFETDriveVoltage: ");
                            Serial.println(BMSData.dischargingMOSFETDriveVoltage);

                            Serial.print("chargingMOSFETDriveVoltage: ");
                            Serial.println(BMSData.chargingMOSFETDriveVoltage);

                            Serial.print("initialComparatorVoltage: ");
                            Serial.println(BMSData.initialComparatorVoltage);

                            Serial.print("equalizationControl: ");
                            Serial.println(BMSData.equalizationControl);

                            Serial.print("systemLog: ");
                            Serial.println(BMSData.systemLog);

                            Serial.print("averageBatteryTemperature: ");
                            Serial.println(BMSData.avgBatteryTemperature);

                            Serial.println();
                            Serial.println();
                            Serial.println();
                        }
                    }
                }
            }
        }

        // End execution debug
        task_BMSUART.endDebugExec(uxTaskGetStackHighWaterMark(NULL));

        // Put the task to sleep
        vTaskDelay(task_BMSUART.getInterval() / portTICK_PERIOD_MS);
    }
}

// This function keep tracking the amount of bad packets received before give the error of connection with the BMS
// If the parameter is false or not informed, the counter is increased. If it is true, it'll restart the counter
void BMSBadDataCounter(bool reset)
{
    // Maximum number of bad packets received to indicate a connection problem
    const uint8_t limitBMSBadData = 30;

    // Variable used to count the number of bad packets received
    static uint8_t counter = 0;

    // If this function was called, it means that the actual packet is already done or it had an error, so it's necessary to request another
    BMSData.BMSReceiving = false;

    // If the paremeter reset is true, it means that a good packet was received, so the counter is kept in 0
    // and the BMS connection flag is put as true
    if (reset)
    {
        counter = 0;
        BMSData.connectionCheck = true;
    }
    else
    {
        // If the reset parameter is false or not informed, count one more packet
        if (counter < limitBMSBadData)
        {
            counter++;
        }
        // If it'll reach the maximum number of bad packets, it'll put the BMS connection flag as false
        else
        {
            BMSData.connectionCheck = false;
        }
    }
}