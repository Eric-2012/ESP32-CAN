/******************************************************************************
 * func_BMS.cpp
 * This file has functions that are used in the Ant_BMS_CAN task
 *****************************************************************************/

#include "func_BMS.h"

// Calculates the average battery temperature and smooth it
void calcAvgBatteryTemperature()
{
    // Calcute the average temperature
    uint8_t countAverage = 0;
    double auxAverage = 0;

    // Go through the array, checking the temperature of the temp probes in the battery (temperatures 2, 3, 4 and 5), summing and counting
    // The checking is done because sometimes the sensor might not be connected or passing wrong data, this was based on Gen1 code
    auxAverage = 0;
    for (uint8_t x = 2; x < 6; x++)
    {
        if (BMSData.temperatures[x] > -70 && !(BMSData.temperatures[x] == 0xFFFF))
        {
            auxAverage += BMSData.temperatures[x];
            countAverage++;
        }
    }
    int16_t auxTemperature = (int16_t)(auxAverage / (double)countAverage);

    // Smooth the BMS temperature
    BMSData.avgBatteryTemperature = avgBMSTemp.addNCalcAvg(auxTemperature);
}

// Calculates the average FET temperature
void calcAvgFETTemperature()
{
    BMSData.avgFETTemperature = ((double)(BMSData.temperatures[0] + BMSData.temperatures[1]) / 2.0);
}

// Calculates the effiency
void calcEfficiency()
{
    // Variables for this function
    static double travelledDistance = 0.0;
    static double lastBatteryRemainingCapacity = 0.0;

    // Get the travelled distance between cycles
    travelledDistance = vehicleInfo.tripKm - travelledDistance;

    // Get the amount of power that it was consumed in the last cycle (Wh)
    BMSData.powerConsumption = (lastBatteryRemainingCapacity - BMSData.batteryRemainingCapacity) * (MCUData.avgVoltage30s);

    // Calculates the efficiency, dividing the consumed power by the travelled distance and calculating the average for around 5 seconds
    // Multiplies by 100 before calcutating the averaged and divides after to not be necessary to use double and keep calc with 2 decimal places anyway
    int16_t auxEfficiency = (int16_t)((BMSData.powerConsumption / travelledDistance) * 100.0);
    auxEfficiency = (int16_t)avgEfficiency.addNCalcAvg(auxEfficiency);
    vehicleInfo.efficiency = auxEfficiency / 100;

    // Updates variables to check changes from last cycle
    lastBatteryRemainingCapacity = BMSData.batteryRemainingCapacity;
    travelledDistance = vehicleInfo.tripKm;
}

// Converts the cell voltages of BMS data from double to uint8_t
// Voltage of each cell. Value 0 = 2V. Incrementing of 0.01V each bit increment.
uint8_t cellVoltageToSaveInFlash(double cellVoltage)
{
    // If the cell voltage is less than 2V, returns 0
    if (cellVoltage < 2.0)
    {
        return 0;
    }
    // If the cell voltage is bigger than 4.55V, returns 255. It means the end of scale
    else if (cellVoltage > 4.55)
    {
        return 255;
    }
    // If it's between scale, converts
    else
    {
        return (uint8_t)((cellVoltage - 2.0) * 100.0);
    }
}

// Analyzes the packet 1 received from the Ant_BMS_CAN via CAN
void BMSPack1Analysis(uint8_t *CANDataArray)
{

    BMSData.chargingMosfetStatus = (CANDataArray[0] >> 2) && 0x01;
    BMSData.dischargingMosfetStatus = (CANDataArray[0] >> 3) && 0x001;
    BMSData.bmsSwitch = (CANDataArray[0] >> 4) && 0x0001;
    BMSData.connectionCheck = BMSData.bmsSwitch;
    BMSData.current = ((double)uint8To_Uint16(CANDataArray[3], CANDataArray[2]) - 5000) / 10.0;
    BMSData.SOC = CANDataArray[1];
    // BMSData.BatteryLowSOC = (CANDataArray[0]>>4)&& 0x004;
    //  Calculates the efficiency after receiving the battery capacity informations
    calcEfficiency();

    BMSData.voltage = ((double)uint8To_Uint16(CANDataArray[5], CANDataArray[4])) / 10.0;
    BMSData.batteryPhysicalCapacity = 100;                           // Not sure
    BMSData.batteryRemainingCapacity = (double)(BMSData.SOC) * 0.98; //  With a marging 5percent
}

// Analyzes the packet 2 received from the Ant_BMS_CAN via CAN
void BMSPack2Analysis(uint8_t *CANDataArray)
{
    BMSData.cellHighestVoltageNumber = CANDataArray[0];
    BMSData.cellHighestVoltageValue = ((double)uint8To_Uint16(CANDataArray[1], CANDataArray[0])) / 1000.0;
    BMSData.cellLowestVoltageNumber = CANDataArray[3];
    BMSData.cellLowestVoltageValue = ((double)uint8To_Uint16(CANDataArray[3], CANDataArray[2])) / 1000.0;
    BMSData.averageCellsVoltage = ((double)uint8To_Uint16(CANDataArray[6], CANDataArray[7])) / 1000.0;
}

// Analyzes the packet 3 received from the Ant_BMS_CAN via CAN
void BMSPack3Analysis(uint8_t *CANDataArray)
{
    BMSData.cellsVoltage[0] = ((double)uint8To_Uint16(CANDataArray[0], CANDataArray[1])) / 1000.0;
    BMSData.cellsVoltage[1] = ((double)uint8To_Uint16(CANDataArray[2], CANDataArray[3])) / 1000.0;
    BMSData.cellsVoltage[2] = ((double)uint8To_Uint16(CANDataArray[4], CANDataArray[5])) / 1000.0;
    BMSData.cellsVoltage[3] = ((double)uint8To_Uint16(CANDataArray[6], CANDataArray[7])) / 1000.0;
}

// Analyzes the packet 4 received from the Ant_BMS_CAN via CAN
void BMSPack4Analysis(uint8_t *CANDataArray)
{
    BMSData.cellsVoltage[4] = ((double)uint8To_Uint16(CANDataArray[0], CANDataArray[1])) / 1000.0;
    BMSData.cellsVoltage[5] = ((double)uint8To_Uint16(CANDataArray[2], CANDataArray[3])) / 1000.0;
    BMSData.cellsVoltage[6] = ((double)uint8To_Uint16(CANDataArray[4], CANDataArray[5])) / 1000.0;
    BMSData.cellsVoltage[7] = ((double)uint8To_Uint16(CANDataArray[6], CANDataArray[7])) / 1000.0;
}

// Analyzes the packet 5 received from the Ant_BMS_CAN via CAN
void BMSPack5Analysis(uint8_t *CANDataArray)
{
    BMSData.cellsVoltage[8] = ((double)uint8To_Uint16(CANDataArray[0], CANDataArray[1])) / 1000.0;
    BMSData.cellsVoltage[9] = ((double)uint8To_Uint16(CANDataArray[2], CANDataArray[3])) / 1000.0;
    BMSData.cellsVoltage[10] = ((double)uint8To_Uint16(CANDataArray[4], CANDataArray[5])) / 1000.0;
    BMSData.cellsVoltage[11] = ((double)uint8To_Uint16(CANDataArray[6], CANDataArray[7])) / 1000.0;
}

// Analyzes the packet 6 received from the Ant_BMS_CAN via CAN
void BMSPack6Analysis(uint8_t *CANDataArray)
{
    BMSData.cellsVoltage[12] = ((double)uint8To_Uint16(CANDataArray[0], CANDataArray[1])) / 1000.0;
    BMSData.cellsVoltage[13] = ((double)uint8To_Uint16(CANDataArray[2], CANDataArray[3])) / 1000.0;
    BMSData.cellsVoltage[14] = ((double)uint8To_Uint16(CANDataArray[4], CANDataArray[5])) / 1000.0;
    BMSData.cellsVoltage[15] = ((double)uint8To_Uint16(CANDataArray[6], CANDataArray[7])) / 1000.0;
}

// Analyzes the packet 7 received from the Ant_BMS_CAN via CAN
void BMSPack7Analysis(uint8_t *CANDataArray)
{
    BMSData.cellsVoltage[16] = ((double)uint8To_Uint16(CANDataArray[0], CANDataArray[1])) / 1000.0;
    BMSData.cellsVoltage[17] = ((double)uint8To_Uint16(CANDataArray[2], CANDataArray[3])) / 1000.0;
    BMSData.cellsVoltage[18] = ((double)uint8To_Uint16(CANDataArray[4], CANDataArray[5])) / 1000.0;
    BMSData.cellsVoltage[19] = ((double)uint8To_Uint16(CANDataArray[6], CANDataArray[7])) / 1000.0;
}

// Analyzes the packet 8 received from the Ant_BMS_CAN via CAN
void BMSPack8Analysis(uint8_t *CANDataArray)
{
    BMSData.cellsVoltage[20] = ((double)uint8To_Uint16(CANDataArray[0], CANDataArray[1])) / 1000.0;
    BMSData.cellsVoltage[21] = ((double)uint8To_Uint16(CANDataArray[2], CANDataArray[3])) / 1000.0;
    BMSData.cellsVoltage[22] = ((double)uint8To_Uint16(CANDataArray[4], CANDataArray[5])) / 1000.0;
    BMSData.cellsVoltage[23] = ((double)uint8To_Uint16(CANDataArray[6], CANDataArray[7])) / 1000.0;
}

// Analyzes the packet 9 received from the Ant_BMS_CAN via CAN
void BMSPack9Analysis(uint8_t *CANDataArray)
{
    BMSData.cellsVoltage[24] = ((double)uint8To_Uint16(CANDataArray[0], CANDataArray[1])) / 1000.0;
    BMSData.cellsVoltage[25] = ((double)uint8To_Uint16(CANDataArray[2], CANDataArray[3])) / 1000.0;
    BMSData.cellsVoltage[26] = ((double)uint8To_Uint16(CANDataArray[4], CANDataArray[5])) / 1000.0;
    BMSData.cellsVoltage[27] = ((double)uint8To_Uint16(CANDataArray[6], CANDataArray[7])) / 1000.0;
}

// Analyzes the packet 10 received from the Ant_BMS_CAN via CAN
void BMSPack10Analysis(uint8_t *CANDataArray)
{
    BMSData.temperatures[0] = (CANDataArray[0] - 40); // MOS
    BMSData.temperatures[1] = (CANDataArray[1] - 40); //  Balancing
    BMSData.temperatures[5] = (CANDataArray[2] - 40); //   Probe 6
    BMSData.temperatures[4] = (CANDataArray[3] - 40); //   Probe 5
    BMSData.temperatures[3] = (CANDataArray[4] - 40); //   Probe 4
    BMSData.temperatures[2] = (CANDataArray[5] - 40); //   Probe 3
    calcAvgBatteryTemperature();
}

// Analyzes the packet 10 received from the Ant_BMS_CAN via CAN
// This packet is not received from the AntBMS CAN
void BMSPack11Analysis(uint8_t *CANDataArray)
{

    /*
    BMSData.batteryPhysicalCapacity  = (double)(BMSData.SOC)*1.02;
    BMSData.batteryRemainingCapacity = 102-(double)(BMSData.SOC)*1.02;

    */
}

// Analyzes the packet 11 received from the Ant_BMS_CAN via CAN
// This packet is not received from the AntBMS CAN
void BMSPack12Analysis(uint8_t *CANDataArray)
{
    /*BMSData.totalUsedCapacity = ((double)uint8To_Uint32(CANDataArray[0], CANDataArray[1], CANDataArray[2], CANDataArray[3])) / 1000.0;
    BMSData.turnOnTime = uint8To_Uint32(CANDataArray[4], CANDataArray[5], CANDataArray[6], CANDataArray[7]);
    */
}

// Analyzes the packet 12 received from the Ant_BMS_CAN via CAN
// This packet is not received from the AntBMS CAN
void BMSPack13Analysis(uint8_t *CANDataArray)
{
    /*BMSData.temperatures[2] = uint8To_Int16(CANDataArray[0], CANDataArray[1]);
    BMSData.temperatures[3] = uint8To_Int16(CANDataArray[2], CANDataArray[3]);
    BMSData.temperatures[4] = uint8To_Int16(CANDataArray[4], CANDataArray[5]);
    BMSData.temperatures[5] = uint8To_Int16(CANDataArray[6], CANDataArray[7]);
     */
    // Calculates the battery average temperature
}

// Analyzes the packet 13 received from the Ant_BMS_CAN via CAN
// This packet is not received from the AntBMS CAN
void BMSPack14Analysis(uint8_t *CANDataArray)
{
    /*BMSData.chargingMosfetStatus = CANDataArray[4];
    BMSData.dischargingMosfetStatus = CANDataArray[5];
    BMSData.balanceStateSign = CANDataArray[6];
    BMSData.numberOfBatteries = CANDataArray[7];
     */
    // Calculates the FET average temperature
    calcAvgFETTemperature();
}

// Analyzes the packet 14 received from the Ant_BMS_CAN via CAN
// This packet is not received from the AntBMS CAN
void BMSPack15Analysis(uint8_t *CANDataArray)
{
    BMSData.wheelDiameter = uint8To_Uint16(CANDataArray[0], CANDataArray[1]);
    BMSData.weeklyHeartbeat = uint8To_Uint16(CANDataArray[2], CANDataArray[3]);
    BMSData.powerOut = uint8To_Int32(CANDataArray[4], CANDataArray[5], CANDataArray[6], CANDataArray[7]);
}

// Analyzes the packet 16 received from the Ant_BMS_CAN via CAN
// This packet is not received from the AntBMS CAN
void BMSPack16Analysis(uint8_t *CANDataArray)
{

    /*
    BMSData.voltageDS = ((double)uint8To_Uint16(CANDataArray[0], CANDataArray[1])) / 10.0;
    BMSData.dischargingMOSFETDriveVoltage = ((double)uint8To_Uint16(CANDataArray[2], CANDataArray[3])) / 10.0;
    BMSData.chargingMOSFETDriveVoltage = ((double)uint8To_Uint16(CANDataArray[4], CANDataArray[5])) / 10.0;
    BMSData.initialComparatorVoltage = uint8To_Uint16(CANDataArray[6], CANDataArray[7]);
    */
}

// Analyzes the packet 17 received from the Ant_BMS_CAN via CAN
// This packet is not received from the AntBMS CAN
void BMSPack17Analysis(uint8_t *CANDataArray)
{
    /*BMSData.equalizationControl = uint8To_Uint32(CANDataArray[0], CANDataArray[1], CANDataArray[2], CANDataArray[3]);
    BMSData.systemLog = uint8To_Uint16(CANDataArray[4], CANDataArray[5]);
    */
}