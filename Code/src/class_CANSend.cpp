/******************************************************************************
 * class_CANSend.cpp
 * This file has the common functions to be used when CAN data is being sent
 *****************************************************************************/

#include "class_CANSend.h"

// Class constructors
CANSend::CANSend(){};
CANSend::~CANSend(){};

// Configure the object to CANExt
void CANSend::confToCanExt(uint32_t _id, CANPreparationFunction _function, uint32_t _interval)
{
    // Save the ID
    id = _id;

    // Saves the CAN Interface
    interface = 0;

    // Save the pointer to the function that will prepare the data
    function = _function;

    // Save the interval
    interval = _interval;
}

// Configure the object to CANEmb
void CANSend::confToCanEmb(uint32_t _id, CANPreparationFunction _function, uint32_t _interval)
{
    // Save the ID
    id = _id;

    // Saves the CAN Interface
    interface = 1;

    // Save the pointer to the function that will prepare the data
    function = _function;

    // Save the interval
    interval = _interval;
}

// Checks if it is a package of interest and analyze it if it is
bool CANSend::prepareNSend()
{
    // Return variable
    bool CANSendStatus = false;

    // If it's time to send the packet
    uint32_t currentTime = millis();
    if (currentTime - lastMillisPacket > interval)
    {
        // Call the function that prepares the CAN Packet to be sent
        function();

        // Copy the array to be sent to the debug variable
        memcpy(dataDebug, data, 8);

        // If the object is using external CAN
        if (interface == 0)
        {
            // Use CAN
            CANExt.poll();

            // Preparing CAN packet
            CANExtTransmitFrame.rtr = false;
            CANExtTransmitFrame.ext = true;
            CANExtTransmitFrame.id = id;
            CANExtTransmitFrame.len = 8;
            memcpy(CANExtTransmitFrame.data, data, 8);

            // Send packet
            CANSendStatus = CANExt.tryToSend(CANExtTransmitFrame);
        }
        // If the object is using embedded CAN
        else if (interface == 1)
        {
            // To be implemented using library ESPCAN
            // Look the tester project to know how to use
        }

        // Prepares for next cycle
        lastMillisPacket = currentTime;
    }

    return CANSendStatus;
}

// Resets the data array
void CANSend::resetData()
{
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
    data[4] = 0;
    data[5] = 0;
    data[6] = 0;
    data[7] = 0;
}

// Write the values of the data array
void CANSend::writeByte(uint8_t byte, uint8_t value)
{
    data[byte] = value;
}

// Get the byte that is being sent
uint8_t CANSend::getByte(uint8_t byte)
{
    return data[byte];
}