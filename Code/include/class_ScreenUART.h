/******************************************************************************
 * func_DwinPanelDisplay.h
 * Header for func_DwinPanelDisplay.cpp
 * PS: The functions to update in the display just will update if the new data
 * is different from the previous OR if the "force" parameter is TRUE.
 *****************************************************************************/
#ifndef __CLASS_DWINPANELDISPLAY_H__
#define __CLASS_DWINPANELDISPLAY_H__

#include "Arduino.h"
#include "global_Constants.h"
#include "global_Variables.h"
#include "class_ScreenUARTProtocol.h"

#define REAL_INT_DATA_LEGTH 0x04
#define REAL_BYTE_DATA_LEGTH 0x03

#define INT_DATA_LENGTH 0x07
#define LONG_DATA_LENGTH 0x09

class DwinPanelDisplay : DwinDisplayUartProtocol
{
private:
	// Riding mode screen
	const uint16_t addrTextTopLeft = 0x9A;
	const uint16_t addrTextBMSTemp = 0x0A;
	const uint16_t addrGraphSoc = 0x06;
	const uint16_t addrGraphMotorTemp = 0x0C;
	const uint16_t addrGraphCurrent = 0x0E;
	const uint16_t addrGraphRegenCurrent = 0x08;
	const uint16_t addrTextSpeed = 0x00;
	const uint16_t addrTextTrip = 0x02;
	const uint16_t addrTextOdometer = 0x27;
	const uint16_t addrGraphMCUVoltage = 0x1C;
	const uint16_t addrIconRidingMode = 0x70; // 0=Eco, 1=City, 2=Pro, 4=Eco (Red), 5=City (Red), 6=Pro (Red)
	const uint16_t addrIconWarning = 0x72;
	const uint16_t addrTextWarning = 0xFE;
	const uint16_t addrIconBattery = 0x10;
	const uint16_t addrIconOBD = 0x12;
	const uint16_t addrIconTurnLeft = 0x16; //0x16;
	const uint16_t addrIconTurnRight = 0x18;
	const uint16_t addrIconHighbeam = 0x1A;
	const uint16_t addrIconCharging = 0x25;

	// Charging mode screen
	const uint16_t addrTextSOCCharging = 0xF8;

	uint8_t homePage = 0;
	uint8_t currentPage = 0;
	uint8_t previousPage = 0;

public:
	DwinPanelDisplay(uint8_t Header_H, uint8_t Header_L);
	~DwinPanelDisplay();

	const uint8_t screenRidingKMH = 9;
	const uint8_t screenRidingMPH = 8;
	const uint8_t screenIsCharging = 31;
	const uint8_t screenIsStored = 30;
	const uint8_t screenIsLocked = 32;
	const uint8_t screenTrueSignal = 33;
	const uint8_t screenFalseSignal = 34;

	// Navigation
	void disableTouch();
	void handshake();
	void setHomePage(uint8_t newHomePage);
	uint8_t getHomePage();
	void setCurrentPage(uint8_t newCurrentPage);
	uint8_t getCurrentPage();
	void updateDisplayPage();
	void changePage(uint8_t pageNum);

	// Riding screen
	void writeTextTopLeft(char *text, bool force = false);
	void writeTextBMSTemp(int16_t pTemperature, bool force = false);
	void showGraphSOC(uint16_t pSOC, bool force = false);
	void showGraphMotorTemperature(int16_t pTemperature, bool force = false);
	void showGraphCurrent(int16_t pCurrent, bool force = false);
	void writeTextSpeed(uint16_t pSpeed, bool force = false);
	void writeTextTrip(uint16_t pTrip, bool force = false);
	void writeTextOdometer(uint32_t pOdometer, bool force = false);
	void showGraphMCUVoltage(double pVoltage, bool force = false);
	void showIconRidingMode(uint16_t pRidingMode, bool force = false);
	void showIconWarning(bool show, bool force = false);
	void writeTextWarning(char *text, bool force = false);
	void showIconBattery(bool show, bool force = false);
	void showIconOBD(bool show, bool force = false);
	void showIconTurnLeft(bool show, bool force = false);
	void showIconTurnRight(bool show, bool force = false);
	void showIconHighbeam(bool show, bool force = false);
	void showIconCharging(bool show, bool force = false);

	// Charging screen
	void writeTextSOCCharging(uint16_t pSOC, bool force = false);
};
#endif