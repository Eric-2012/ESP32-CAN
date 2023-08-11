/******************************************************************************
 * class_CANSend.h
 * Header for class_CANSend.cpp
 *****************************************************************************/
#ifndef __CLASS_CANSEND_H__
#define __CLASS_CANSEND_H__

#include <Arduino.h>

#include "global_Constants.h"
#include "global_Variables.h"

// Type used to store a pointer to a function
typedef void (*CANPreparationFunction)();

class CANSend
{
private:
    // CAN ID Packet
    uint32_t id;

    // Function called to prepare the data frame
    CANPreparationFunction function;

    // Interval between sendings (ms)
    uint32_t interval;

    // Last time when the packet was sent (ms)
    uint32_t lastMillisPacket;

    // Packet to be sent
    uint8_t data[8];

    // Save the data before send, for debugging
    uint8_t dataDebug[8];

    // CAN Interface. 0 (external) or 1 (embedded)
    uint8_t interface;

public:
    // Class constructors
    CANSend();
    ~CANSend();

    // Configures the object
    void confToCanExt(uint32_t _id, CANPreparationFunction _function, uint32_t _interval);
    void confToCanEmb(uint32_t _id, CANPreparationFunction _function, uint32_t _interval);

    // Prepare the packet and send it
    bool prepareNSend();

    // Resets the data array
    void resetData();

    // Write the values of the data array
    void writeByte(uint8_t byte, uint8_t value);

    // Get the byte that is being sent
    uint8_t getByte(uint8_t byte);
};
#endif