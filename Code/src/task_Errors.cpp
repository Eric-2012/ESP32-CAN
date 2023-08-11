/******************************************************************************
 * task_Errors.cpp
 * This file has the code for task Errors that will check all status messages
 * and log the error codes in just one place
 *****************************************************************************/

#include "task_Errors.h"

// Code for the task Errors
void taskCode_Errors(void *pvParameters)
{
    while (true)
    {
        // Start execution debug
        task_Errors.initDebugExec();

        /* BMS Charging MOSFET ============================ */
        // E001	FET off (This variable is also 0 if the BMS has a communication issue, so the connectionCheck is checked)
        updateErrorArray(1, (BMSData.chargingMosfetStatus == 0) && (BMSData.connectionCheck));

        // E002	Overvoltage protection
        updateErrorArray(2, (BMSData.chargingMosfetStatus == 2));

        // E003	Overcurrent protection
        updateErrorArray(3, (BMSData.chargingMosfetStatus == 3));

        // E004	Pack overvoltage
        updateErrorArray(4, (BMSData.chargingMosfetStatus == 5));

        // E005	Battery over temperature
        updateErrorArray(5, (BMSData.chargingMosfetStatus == 6));

        // E006	FET over temperature
        updateErrorArray(6, (BMSData.chargingMosfetStatus == 7));

        // E007	Abnormal current
        updateErrorArray(7, (BMSData.chargingMosfetStatus == 8));

        // E008	Pack out of balance
        updateErrorArray(8, (BMSData.chargingMosfetStatus == 9));

        // E009	Motherboard overheat
        updateErrorArray(9, (BMSData.chargingMosfetStatus == 10));

        // E010	FET error
        updateErrorArray(10, (BMSData.chargingMosfetStatus == 13));

        /* BMS Discharging MOSFET ========================= */
        // E011	FET off (This variable is also 0 if the BMS has a communication issue, so the connectionCheck is checked)
        updateErrorArray(11, (BMSData.dischargingMosfetStatus == 0) && (BMSData.connectionCheck));

        // E012	Overvoltage protection
        updateErrorArray(12, (BMSData.dischargingMosfetStatus == 2));

        // E013	Overcurrent protection
        updateErrorArray(13, (BMSData.dischargingMosfetStatus == 3));

        // E014	Pack under voltage
        updateErrorArray(14, (BMSData.dischargingMosfetStatus == 5));

        // E015	Battery over temperature
        updateErrorArray(15, (BMSData.dischargingMosfetStatus == 6));

        // E016	FET over temperature
        updateErrorArray(16, (BMSData.dischargingMosfetStatus == 7));

        // E017	Abnormal current
        updateErrorArray(17, (BMSData.dischargingMosfetStatus == 8));

        // E018	Pack out of balance
        updateErrorArray(18, (BMSData.dischargingMosfetStatus == 9));

        // E019	Motherboard overheat
        updateErrorArray(19, (BMSData.dischargingMosfetStatus == 10));

        // E020	Short circuit protection
        updateErrorArray(20, (BMSData.dischargingMosfetStatus == 12));

        // E021	FET error
        updateErrorArray(21, (BMSData.dischargingMosfetStatus == 13));

        /* BMS Battery balance ============================ */
        // E022	Pack imbalanced
        updateErrorArray(22, (BMSData.balanceStateSign == 1));

        // E023	Pack charging imbalanced
        updateErrorArray(23, (BMSData.balanceStateSign == 2));

        // E024	Balance over temperature
        updateErrorArray(24, (BMSData.balanceStateSign == 3));

        // E025	Motherboard overheat
        updateErrorArray(25, (BMSData.balanceStateSign == 10));

        /* Charger ======================================== */
        // E100	Communication error
        updateErrorArray(100, bitRead(chargerData.statusByte, 4));

        // E101	Hardware failure
        updateErrorArray(101, bitRead(chargerData.statusByte, 0));

        // E102	Over temperature
        updateErrorArray(102, bitRead(chargerData.statusByte, 1));

        // E103	Low input voltage
        updateErrorArray(103, bitRead(chargerData.statusByte, 2));

        // E104	Safety shutdown
        updateErrorArray(104, bitRead(chargerData.statusByte, 3));

        /* MCU ============================================ */
        // E200	Main contactor failure
        updateErrorArray(200, bitRead(MCUData.lowByteError, 2));

        // E201	Motor thermal shutdown
        updateErrorArray(201, bitRead(MCUData.lowByteError, 3));

        // E202	Motor thermal limit
        updateErrorArray(202, bitRead(MCUData.lowByteError, 4));

        // E203	Controller thermal shutdown
        updateErrorArray(203, bitRead(MCUData.lowByteError, 5));

        // E204	Controller thermal limit
        updateErrorArray(204, bitRead(MCUData.lowByteError, 6));

        // E205	Accelerator failure
        updateErrorArray(205, bitRead(MCUData.lowByteError, 7));

        // E206	Throttle high protection
        updateErrorArray(206, bitRead(MCUData.highByteError, 0));

        // E207	Undervoltage shutdown
        updateErrorArray(207, bitRead(MCUData.highByteError, 1));

        // E208	Limp mode
        updateErrorArray(208, bitRead(MCUData.highByteError, 2));

        // E209	50% speed limit
        updateErrorArray(209, bitRead(MCUData.highByteError, 3));

        // E210	Overvoltage protection
        updateErrorArray(210, bitRead(MCUData.highByteError, 4));

        // E211	Overcurrent protection
        updateErrorArray(211, bitRead(MCUData.highByteError, 5));

        // E212	Motor stalled
        updateErrorArray(212, bitRead(MCUData.highByteError, 6));

        // E213	Hall sensor failure
        updateErrorArray(213, bitRead(MCUData.highByteError, 7));

        /* Vehicle errors ================================= */
        // E300	BMS communication timeout
        // If the BMS data is coming via UART, the error needs to be from !BMSData.connectionCheck
        if (vehicleParam.BMSType == 0)
            updateErrorArray(300, !BMSData.connectionCheck);

        // If the BMS data is coming via CAN, the error needs to be from the CAN objects
        // Check just for the first pack for timeout error
        if (vehicleParam.BMSType == 1)
            updateErrorArray(300, !BMSRecv1.checkConn()); 

        // E301	Charger communication timeout (Maybe useless)
        updateErrorArray(301, 0);

        // E302	MCU communication timeout
        updateErrorArray(302, (!MCUAPTRecv1.checkConn()) || (!MCUAPTRecv2.checkConn()));

        // E303	HMI communication timeout (Maybe useless)
        updateErrorArray(303, 0);

        // E304	Kickstand down
        updateErrorArray(304, (inputs.kickstand && !vehicleParam.ignoreKillswitchKickstand));

        // E305	Killswitch active
        updateErrorArray(305, (inputs.killswitch && !vehicleParam.ignoreKillswitchKickstand));

        // E306	Front tire warning
        updateErrorArray(306, 0);

        // E307	Rear tire warning
        updateErrorArray(307, 0);

        // E308	Front tire sensor battery error
        updateErrorArray(308, 0);

        // E309	Rear tire sensor battery error
        updateErrorArray(309, 0);

        // E310	Vehicle crashed
        updateErrorArray(310, 0);

        // E311	Vehicle down
        updateErrorArray(311, 0);

        // E312 Throttle high protection
        updateErrorArray(312, MCUData.ECUHighThrottleProt);

        // End of cycle
        updateErrorArray(0, 0, true);

        // End execution debug
        task_Errors.endDebugExec(uxTaskGetStackHighWaterMark(NULL));

        // Put the task to sleep
        vTaskDelay(task_Errors.getInterval() / portTICK_PERIOD_MS);
    }
}