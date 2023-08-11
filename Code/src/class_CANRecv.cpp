/******************************************************************************
 * class_CANRecv.cpp
 * This file has the common functions to be used when CAN data is being received
 *****************************************************************************/

#include "class_CANRecv.h"

// Class constructors
CANRecv::CANRecv(){};
CANRecv::~CANRecv(){};

// Configure the object to CANExt
void CANRecv::confToCanExt(uint32_t _id, CANAnalysisFunction _function, uint32_t _maxTime)
{
    // Save the ID
    id = _id;

    // Saves the CAN Interface
    interface = 0;

    // Save the pointer to the function that will analyze the data
    function = _function;

    // Save the maxTime
    maxTime = _maxTime;
}

// Configure the object to CANEmb
void CANRecv::confToCanEmb(uint32_t _id, CANAnalysisFunction _function, uint32_t _maxTime)
{
    // Save the ID
    id = _id;

    // Saves the CAN Interface
    interface = 1;

    // Save the pointer to the function that will analyze the data
    function = _function;

    // Save the maxTime
    maxTime = _maxTime;
}

// Checks if it is a package of interest and analyze it if it is
// Returns true is the id matches
bool CANRecv::checkNAnalyze()
{
    // Return variable
    bool aux = false;

    // If the object is using external CAN
    if (interface == 0)
    {
        // Checks the ID of the received packet
        if (CANExtReceiveFrame.id == id)
        {
            if ((id == addrMCUAPTRecv1) | (id == addrMCUAPTRecv2))
            {
                CANExt.setFiltersOnTheFly(ACAN2515Mask(), NULL, 0);
            }

            // Copy the data to an internal array
            memcpy(data, CANExtReceiveFrame.data, 8);

            // Call the function to analyze the packet
            function(&data[0]);

            // Save the time when the last packet was received
             lastMillisPacket = millis();

            // Force the connected as true
            connected = true;

            // Flags that the packet was received by the object, it means that it was the ID that it was waiting
            aux = true;
        }
        else
        {
            if ((id == addrMCUAPTRecv1) | (id == addrMCUAPTRecv2))
            {
                ACAN2515Mask rxm0 = extended2515Mask(0xFFFFFFFF);
                const ACAN2515AcceptanceFilter filters[] = {
                    {extended2515Filter(id), NULL},
                    // {extended2515Filter(addrHMISend1), NULL},
                    // {extended2515Filter(addrHMISend2), NULL},
                    // {extended2515Filter(addrHMISend3), NULL},
                    // {extended2515Filter(addrHMISend4), NULL}
                };
                CANExt.setFiltersOnTheFly(rxm0, filters, 1);
                // Serial.println(id);
            }
        }
    }
    // If the object is using embedded CAN
    else if (interface == 1)
    {
        // To be implemented using library ESPCAN
        // Look the tester project to know how to use
    }

    // If aux is false, it means that the ID was not for this object, in this case, looks to the timing to check if there is a timeout issue
    if (!aux)
    {
        // Check if the time difference between now and the last packet is bigger than the max time, if yes, put connected as false
        if (millis() - lastMillisPacket >= maxTime)
        {
            connected = false;
        }
    }
    return aux;
}

// Checks if there is a timeout error
// Return true if it's connected, false if not
bool CANRecv::checkConn()
{
    return connected;
}