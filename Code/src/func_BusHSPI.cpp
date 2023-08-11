/******************************************************************************
 * func_BusI2C.cpp
 * This file has functions that uses the HSPI Bus, used in the task_BusHSPI
 * This bus is used by the inertial sensor and flash memory
 *****************************************************************************/

#include "func_BusHSPI.h"

// Populates the struct infoToSave
void prepareInfoToSave()
{
    // Variables for this function
    uint8_t aux; // Used to help in some places

    infoToSave.packetCode = 0; // TO BE IMPLEMENTED

    infoToSave.second = timeNow.second;
    infoToSave.minute = timeNow.minute;
    infoToSave.hour = timeNow.hour;
    infoToSave.dayW = timeNow.dayW;
    infoToSave.dayM = timeNow.dayM;
    infoToSave.month = timeNow.month;
    infoToSave.year = timeNow.year;

    infoToSave.boardTemperature = vehicleInfo.boardTemperature;
    infoToSave.boardSupplyVoltage = (uint8_t)(vehicleInfo.boardSupplyVoltage * 10.0);
    infoToSave.odometerKm = vehicleInfo.odometerKm;
    infoToSave.tripKm = (uint16_t)vehicleInfo.tripKm;
    infoToSave.speedKmh = (uint16_t)vehicleInfo.speedKmh;
    infoToSave.maximumSpeed = (uint16_t)vehicleInfo.customMaxSpeed;
    infoToSave.rpm = vehicleInfo.rpm;

    infoToSave.efficiency = vehicleInfo.efficiency;

    aux = 0;
    bitWrite(aux, 0, vehicleInfo.isStored);
    bitWrite(aux, 1, vehicleInfo.isLocked);
    bitWrite(aux, 2, vehicleInfo.isInReverseMode);
    bitWrite(aux, 3, vehicleInfo.isChargerConnected);
    bitWrite(aux, 4, vehicleInfo.isCharging);
    bitWrite(aux, 5, vehicleInfo.isFastChargeOn);
    bitWrite(aux, 6, vehicleInfo.errorsDetected);
    bitWrite(aux, 7, vehicleInfo.ridingModeChangedBecauseOfError);
    infoToSave.vehicleStatuByte1 = aux;

    aux = 0;
    aux = (vehicleInfo.ridingMode & 0b00000111); // The AND operation is to have sure that will be just the 3 bits
    bitWrite(aux, 4, BMSRecv1.checkConn());
    bitWrite(aux, 5, MCUAPTRecv1.checkConn());
    bitWrite(aux, 6, MCUAPTRecv2.checkConn());
    bitWrite(aux, 7, MCUData.ECUHighThrottleProt);
    infoToSave.vehicleStatuByte2 = aux;

    infoToSave.numActiveErrors = vehicleInfo.numActiveErrors;
    infoToSave.sumActiveErrors = vehicleInfo.sumActiveErrors;
    infoToSave.roll = (int16_t)vehicleInfo.roll;
    infoToSave.pitch = (int16_t)vehicleInfo.pitch;
    infoToSave.frontTirePressure = (uint8_t)vehicleInfo.frontTirePressure;
    infoToSave.frontTireTemperature = vehicleInfo.frontTireTemperature;
    infoToSave.rearTirePressure = (uint8_t)vehicleInfo.rearTirePressure;
    infoToSave.rearTireTemperature = vehicleInfo.rearTireTemperature;
    infoToSave.inputsI2C = inputs.inputsI2CDebug;
    infoToSave.outputsI2C = outputs.outputsI2CDebug;
    infoToSave.BMSVoltage = (uint16_t)(BMSData.voltage * 10.0);

    // Convert the cell voltages
    // Value 0 = 3V. Incrementing of 0.01V each bit increment.
    for (uint8_t x = 0; x < 32; x++)
    {
        infoToSave.BMSCellVoltages[x] = cellVoltageToSaveInFlash(BMSData.cellsVoltage[x]);
    }

    infoToSave.BMSCurrent = (int16_t)(BMSData.current * 10.0);

    for (uint8_t x = 0; x < 6; x++)
    {
        infoToSave.BMSTemperatures[x] = (int8_t)BMSData.temperatures[x];
    }

    infoToSave.BMSSoc = BMSData.SOC;
    infoToSave.BMSBatteryPhysicalCapacity = (uint8_t)BMSData.batteryPhysicalCapacity;
    infoToSave.BMSBatteryRemainingCapacity = (uint32_t)(BMSData.batteryRemainingCapacity * 1000000.0);
    infoToSave.BMSTurnOnTime = BMSData.turnOnTime;
    infoToSave.BMSCellHighestVoltageNumber = BMSData.cellHighestVoltageNumber;
    infoToSave.BMSCellHighestVoltageValue = cellVoltageToSaveInFlash(BMSData.cellHighestVoltageValue);
    infoToSave.BMCCellLowestVoltageNumber = BMSData.cellLowestVoltageNumber;
    infoToSave.BMSCellLowestVoltageValue = cellVoltageToSaveInFlash(BMSData.cellLowestVoltageValue);
    infoToSave.BMSNumberOfBatteries = BMSData.numberOfBatteries;
    infoToSave.BMSChargingMosfetStatus = BMSData.chargingMosfetStatus;
    infoToSave.BMSDischargingMosfetStatus = BMSData.dischargingMosfetStatus;
    infoToSave.BMSBalanceStateSign = BMSData.balanceStateSign;
    infoToSave.MCUVoltage = (uint16_t)(MCUData.voltage * 10.0);
    infoToSave.MCUBusCurrent = (int16_t)(MCUData.busCurrent * 10.0);
    infoToSave.MCURpm = MCUData.rpm;
    infoToSave.MCUMotorTemperature = MCUData.motorTemperature;
    infoToSave.MCUControllerTemperature = MCUData.controllerTemperature;
    infoToSave.MCUThrottle = MCUData.throttle;
    infoToSave.MCULowByteError = MCUData.lowByteError;
    infoToSave.MCUHighByteError = MCUData.highByteError;

    aux = 0;
    bitWrite(aux, 0, bitRead(MCUData.statusOutDebug, 5));
    bitWrite(aux, 1, 0);
    bitWrite(aux, 2, bitRead(MCUData.statusOutDebug, 7));
    bitWrite(aux, 3, bitRead(MCUData.lowByteStatus, 5));
    bitWrite(aux, 4, bitRead(MCUData.lowByteStatus, 6));
    bitWrite(aux, 5, bitRead(MCUData.lowByteStatus, 7));
    aux += (MCUData.stateDebug << 6);
    infoToSave.MCUStatusByte = aux;

    infoToSave.MCUOutCurrent = MCUData.currentOutDebug;
    infoToSave.MCUMaxRegen = MCUData.regenOutDebug;
    infoToSave.maximumThrottle = MCUData.minThrottleDebug;
    infoToSave.minimumThrottle = MCUData.maxThrottleDebug;

    infoToSave.chargerVoltage = (uint16_t)(chargerData.voltage * 10.0);
    infoToSave.chargerCurrent = (int16_t)(chargerData.current * 10.0);
    infoToSave.chargerStatusByte = chargerData.statusByte;
    infoToSave.maxChargerVoltage = (uint16_t)(chargerData.voltageOutput * 10.0);
    infoToSave.maxChargerCurrent = (int16_t)(chargerData.currentOutput * 10.0);
    infoToSave.millis = millis();

    aux = 0;
    aux = (rtc_get_reset_reason(0) - 1) << 4;
    aux += (rtc_get_reset_reason(1) - 1);
    infoToSave.resetReason = aux;
}