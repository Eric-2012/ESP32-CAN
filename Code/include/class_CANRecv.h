/******************************************************************************
 * class_CANRecv.h
 * Header for class_CANRecv.cpp
 *****************************************************************************/
#ifndef __CLASS_CANRECV_H__
#define __CLASS_CANRECV_H__

#include <Arduino.h>

#include "global_Constants.h"
#include "global_Variables.h"

// Type used to store a pointer to a function
typedef void (*CANAnalysisFunction)(uint8_t *CANDataArray);

class CANRecv
{
private:
    // CAN ID Packet
    uint32_t id;

    // Function called to analyze the data frame
    CANAnalysisFunction function;

    // Max. time expected between packets (ms)
    uint32_t maxTime;

    // Last time when the packet was received (ms)
    uint32_t lastMillisPacket;

    // 8 bytes received through the packet
    uint8_t data[8];

    // CAN Interface. 0 (external) or 1 (embedded)
    uint8_t interface;

    // Status of connection
    // It's forced to true each time that a packet is received and forced to false in a timeout check on the function checkNAnalyze.
    bool connected = false;

public:
    // Class constructors
    CANRecv();
    ~CANRecv();

    // Configures the object
    void confToCanExt(uint32_t _id, CANAnalysisFunction _function, uint32_t _maxTime);
    void confToCanEmb(uint32_t _id, CANAnalysisFunction _function, uint32_t _maxTime);

    // Checks if it is a package of interest and analyze it if it is
    bool checkNAnalyze();

    // Checks if there is a timeout error
    // Return true if it's connected, false if not
    bool checkConn();
};
#endif