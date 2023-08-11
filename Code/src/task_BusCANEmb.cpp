/******************************************************************************
 * task_BusCANEmb.cpp
 * This file has the functions that take care of the embedded CAN Bus
 *****************************************************************************/

#include "task_BusCANEmb.h"

// Code for the task CANBus
void taskCode_BusCANEmb(void *pvParameters)
{
    // Init CAN connection
    CANEmbCgf.speed = CAN_SPEED_250KBPS;
    /*
    WHEN WILL USE EMBEDDED CAN, IT
    CANEmbCgf.tx_pin_id = pinTXCAN2;
    CANEmbCgf.rx_pin_id = pinRXCAN2;
    */
    CANEmbCgf.rx_queue = xQueueCreate(CANEmbRXQueueSize, sizeof(CAN_frame_t));
    ESP32Can.CANInit();

    // Configure the CANRecv objects

    // Configure the CANSend objects

    while (true)
    {
        // Start execution debug
        task_BusCANEmb.initDebugExec();

        // Checks incoming data from the CANBus
        // Use CAN

        // If some CAN data is available
        while (CANExt.available())
        {
            // Receive CAN Data
            CANExt.receive(CANExtReceiveFrame);

            // The function checkNAnalyze return true if the ID matches, so if the return is true the function continue jumps all other parts to get next packet
        }

        // Checks connection times
        CANEmbWatchDog();

        // Send CAN data if the bike passed the initCheckup
        if (vehicleInfo.passedInitCheckup)
        {
        }

        // End execution debug
        task_BusCANEmb.endDebugExec(uxTaskGetStackHighWaterMark(NULL));

        // Put the task to sleep
        vTaskDelay(task_BusCANEmb.getInterval() / portTICK_PERIOD_MS);
    }
}

// Function that monitors CAN connection times
void CANEmbWatchDog()
{
}