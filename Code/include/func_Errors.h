/******************************************************************************
 * func_Errors.h
 * Header for func_Errors.cpp
 *****************************************************************************/
#ifndef __FUNC_ERRORS_H__
#define __FUNC_ERRORS_H__

#include <Arduino.h>

#include "global_Constants.h"
#include "global_Variables.h"

// Number of errors, used to scan the string with the error messages
const uint16_t numberOfErrors = 57;

// Error messages
const String errorMessages[] PROGMEM = {
    // BMS Charging MOSFET
    "E001 BMSCF OFF",
    "E002 BMSCF OVV PROT",
    "E003 BMSCF OVC PROT",
    "E004 BMSCF PACK OVV",
    "E005 BMSCF BATT OVT",
    "E006 BMSCF OVT",
    "E007 BMSCF ABN CURR",
    "E008 BMSCF PACK IMB",
    "E009 BMSCF MOBO OVH",
    "E010 BMSCF ERROR",
    // BMS Discharging MOSFET
    "E011 BMSDF OFF",
    "E012 BMSDF OVV PROT",
    "E013 BMSDF OVC PROT",
    "E014 BMSDF PACK UDV",
    "E015 BMSDF BATT OVT",
    "E016 BMSDF OVT",
    "E017 BMSDF ABN CURR",
    "E018 BMSDF PACK IMB",
    "E019 BMSDF MOBO OVH",
    "E020 BMSDF SC PROT",
    "E021 BMSDF ERROR",
    // BMS Battery balance
    "E022 BMSBAL PACK IMB",
    "E023 BMSBAL CHGR IMB",
    "E024 BMSBAL OVT",
    "E025 BMSBAL MOBO OVH",
    // Charger
    "E100 CHGR COMM ERROR",
    "E101 CHGR HDW FAIL",
    "E102 CHGR OVT",
    "E103 CHGR LOW IV",
    "E104 CHGR SAF SD",
    // MCU
    "E200 MCU CONT FAIL",
    "E201 MOT THRM SD",
    "E202 MOT THRM LIMIT",
    "E203 MCU THRM SD",
    "E204 MCU THRM LIMIT",
    "E205 MCU ACC FAIL",
    "E206 MCU TH PROT",
    "E207 MCU UDV SD",
    "E208 MCU LIMP MODE",
    "E209 MCU 50% SPLIMIT",
    "E210 MCU OVV PROT",
    "E211 MCU OVC PROT",
    "E212 MOT STALL",
    "E213 MCU HS FAIL",
    // Vehicle errors
    "E300 BMS TIMEOUT",
    "E301 CHGR TIMEOUT",
    "E302 MCU TIMEOUT",
    "E303 HMI TIMEOUT",
    "E304 KICKSTAND DOWN",
    "E305 KILLSWITCH ON",
    "E306 FT WARN",
    "E307 RT WARN",
    "E308 FT SEN BT ERROR",
    "E309 RT SEN BT ERROR",
    "E310 VEH CRASH",
    "E311 VEH DOWN",
    "E312 ECU TH PROT"
};

// Update the error array and the variable vehicleInfo.errorsDetected in the end of each check cycle
void updateErrorArray(uint16_t errorCode, bool value, bool endCheckCycle = false);

// Return if the informed error code is true or false
bool getErrorStatus(uint16_t errorCode);

// Check if the bike can run
bool checkIfBikeCanRun();

// Check some of the errors that will limit 50% of power is happening
bool checkErrors50Power();

// Check some of the errors that will keep in city mode is happening
bool checkErrorsKeepCityMode();

// Check some of the errors that will keep in eco mode is happening
bool checkErrorsKeepEcoMode();

#endif