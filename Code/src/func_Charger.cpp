/******************************************************************************
 * func_Charger.cpp
 * This file has the code for the functions that analizes the Charger data
 * and prepares the outputs for it
 *****************************************************************************/

#include "func_Charger.h"

// Analyzes the charger package
void chargerPackAnalysis(uint8_t *CANDataArray)
{
    chargerData.voltage = ((double)uint8To_Uint16(CANDataArray[0], CANDataArray[1])) / 10.0;
    chargerData.current = ((double)uint8To_Int16(CANDataArray[2], CANDataArray[3])) / 10.0;
    chargerData.statusByte = CANDataArray[4];
}

// Prepares the CAN packet to send via can to the Charger
void prepareCANChargerOut()
{
    // Resets charger CAN Packet
    chargerSend.resetData();

    // Voltage and current to charge
    chargerData.voltageOutput = vehicleParam.maximumChargeVoltage;

    // If the fast charge is ON, it puts the maximum. If it's off, puts half
    chargerData.currentOutput = 0;
    if (inputs.fastChargeSwitch)
    {
        chargerData.currentOutput = vehicleParam.maximumChargeCurrent;
    }
    else
    {
        chargerData.currentOutput = vehicleParam.maximumChargeCurrent / 2;
    }

    // Multiply by 10 and split the voltage in high and low byte
    uint16_t aux = chargerData.voltageOutput * 10.0;
    chargerSend.writeByte(chargerSend_byteMaxVoltageHighByte, (aux >> 8));
    chargerSend.writeByte(chargerSend_byteMaxVoltageLowByte, (aux & 0xFF));

    // Multiply by 10 and split the current in high and low byte
    aux = chargerData.currentOutput * 10;
    chargerSend.writeByte(chargerSend_byteMaxCurrentHighByte, (aux >> 8));
    chargerSend.writeByte(chargerSend_byteMaxCurrentLowByte, (aux & 0xFF));
}