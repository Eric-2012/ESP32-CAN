/******************************************************************************
 * func_DwinPanelDisplay.cpp
 * This file has functions to use the DWIN Display
 * PS: The functions to update in the display just will update if the new data
 * is different from the previous OR if the "force" parameter is TRUE.
 *****************************************************************************/

#include "class_ScreenUART.h"

DwinPanelDisplay::DwinPanelDisplay(uint8_t Header_H, uint8_t Header_L) : DwinDisplayUartProtocol(Header_H, Header_L){};
DwinPanelDisplay::~DwinPanelDisplay() {}

// Disable touch screen
void DwinPanelDisplay::disableTouch()
{
	DwinDisplayUartProtocol::transforFunction(REAL_BYTE_DATA_LEGTH, SET_DATA_REGISTER, screenCmdDisableTouch, 0x00);
}

// Set the homepage
void DwinPanelDisplay::setHomePage(uint8_t newHomePage)
{
	homePage = newHomePage;
}

// Get the homepage
uint8_t DwinPanelDisplay::getHomePage()
{
	return homePage;
}

// Set the current page
void DwinPanelDisplay::setCurrentPage(uint8_t newCurrentPage)
{
	currentPage = newCurrentPage;
}

// Get the current page
uint8_t DwinPanelDisplay::getCurrentPage()
{
	return currentPage;
}

// Update display if new page is different from the current
void DwinPanelDisplay::updateDisplayPage()
{
	if (getCurrentPage() != previousPage)
	{
		previousPage = getCurrentPage();
		changePage(currentPage);
	}
}

// Change page
void DwinPanelDisplay::changePage(uint8_t pageNum)
{
	DwinDisplayUartProtocol::transforFunction(REAL_INT_DATA_LEGTH, SET_DATA_REGISTER, screenCmdChangePage, 0x00, pageNum);
}

// Shows text in the top left corner
void DwinPanelDisplay::writeTextTopLeft(char *text, bool force)
{
	String textAux = text;
	uint8_t limit = 14;
	char message[14];
	static char previous[14];

	// Limit the text size if bigger
	textAux.remove(limit);
	textAux.toCharArray(message, limit);

	if ((strcmp(previous, message) != 0) || (force))
	{
		strcpy(previous, message);
		DwinDisplayUartProtocol::transforData(SET_DATA_VARIABLE_REGISTER, addrTextTopLeft, message, limit);

		// Shows debug message if the flag is active
		if (serialDebugFlags.get(0))
		{
			Serial.println("writeTextTopLeft");
		}
	}
}

// Shows the BMS temperature in text
void DwinPanelDisplay::writeTextBMSTemp(int16_t pTemperature, bool force)
{
	static int16_t previous;

	if ((previous != pTemperature) || (force))
	{
		previous = pTemperature;
		DwinDisplayUartProtocol::transforData(INT_DATA_LENGTH, SET_DATA_VARIABLE_REGISTER, addrTextBMSTemp, pTemperature);

		// Shows debug message if the flag is active
		if (serialDebugFlags.get(0))
		{
			Serial.println("writeTextBMSTemp");
		}
	}
}

// Shows the SOC Icon
void DwinPanelDisplay::showGraphSOC(uint16_t pSOC, bool force)
{
	static uint8_t previous;
	uint8_t blocks = 0;

	if (pSOC > 90)
	{
		blocks = 10;
	}
	else if (pSOC > 81)
	{
		blocks = 9;
	}
	else if (pSOC > 72)
	{
		blocks = 8;
	}
	else if (pSOC > 63)
	{
		blocks = 7;
	}
	else if (pSOC > 54)
	{
		blocks = 6;
	}
	else if (pSOC > 45)
	{
		blocks = 5;
	}
	else if (pSOC > 36)
	{
		blocks = 4;
	}
	else if (pSOC > 27)
	{
		blocks = 3;
	}
	else if (pSOC > 22)
	{
		blocks = 2;
	}
	else if (pSOC > 11)
	{
		blocks = 1;
	}
	else
	{
		blocks = 0;
	}

	if ((previous != blocks) || (force))
	{
		previous = blocks;
		DwinDisplayUartProtocol::transforData(INT_DATA_LENGTH, SET_DATA_VARIABLE_REGISTER, addrGraphSoc, (uint16_t)(blocks & 0xFF));

		// Shows debug message if the flag is active
		if (serialDebugFlags.get(0))
		{
			Serial.println("showGraphSOC");
		}
	}
}

// Shows the motor temperature icon
void DwinPanelDisplay::showGraphMotorTemperature(int16_t pTemperature, bool force)
{
	static uint8_t previous;
	uint8_t blocks = 0;

	if (pTemperature < 40)
	{
		blocks = 0;
	}
	else if (pTemperature < 55)
	{
		blocks = 1;
	}
	else if (pTemperature < 65)
	{
		blocks = 2;
	}
	else if (pTemperature < 75)
	{
		blocks = 3;
	}
	else if (pTemperature < 85)
	{
		blocks = 4;
	}
	else if (pTemperature < 95)
	{
		blocks = 5;
	}
	else if (pTemperature < 105)
	{
		blocks = 6;
	}
	else if (pTemperature < 115)
	{
		blocks = 7;
	}
	else if (pTemperature < 120)
	{
		blocks = 8;
	}
	else if (pTemperature < 120)
	{
		blocks = 9;
	}
	else
	{
		blocks = 10;
	}

	if ((previous != blocks) || (force))
	{
		previous = blocks;
		DwinDisplayUartProtocol::transforData(INT_DATA_LENGTH, SET_DATA_VARIABLE_REGISTER, addrGraphMotorTemp, (uint16_t)(blocks & 0xFF));

		// Shows debug message if the flag is active
		if (serialDebugFlags.get(0))
		{
			Serial.println("showGraphMotorTemperature");
		}
	}
}

// Shows the current (if >= 0) and regen (if < 0)
void DwinPanelDisplay::showGraphCurrent(int16_t pCurrent, bool force)
{
	static int16_t previous;

	if ((previous != pCurrent) || (force))
	{
		previous = pCurrent;

		// Current
		if (pCurrent >= 0)
		{
			DwinDisplayUartProtocol::transforData(INT_DATA_LENGTH, SET_DATA_VARIABLE_REGISTER, addrGraphRegenCurrent, 0);
			if (pCurrent > 140)
			{
				DwinDisplayUartProtocol::transforData(INT_DATA_LENGTH, SET_DATA_VARIABLE_REGISTER, addrGraphCurrent, 70);
			}
			else
			{
				DwinDisplayUartProtocol::transforData(INT_DATA_LENGTH, SET_DATA_VARIABLE_REGISTER, addrGraphCurrent, (pCurrent / 2));
			}
		}
		// Regen
		if (pCurrent < 0)
		{
			DwinDisplayUartProtocol::transforData(INT_DATA_LENGTH, SET_DATA_VARIABLE_REGISTER, addrGraphCurrent, 0);
			if (pCurrent < -20)
			{
				DwinDisplayUartProtocol::transforData(INT_DATA_LENGTH, SET_DATA_VARIABLE_REGISTER, addrGraphRegenCurrent, 10);
			}
			else
			{
				DwinDisplayUartProtocol::transforData(INT_DATA_LENGTH, SET_DATA_VARIABLE_REGISTER, addrGraphRegenCurrent, (-pCurrent / 2));
			}
		}

		// Shows debug message if the flag is active
		if (serialDebugFlags.get(0))
		{
			Serial.println("showGraphCurrent");
		}
	}
}

// Shows the speed
void DwinPanelDisplay::writeTextSpeed(uint16_t pSpeed, bool force)
{
	static uint16_t previous;

	if ((previous != pSpeed) || (force))
	{
		previous = pSpeed;
		DwinDisplayUartProtocol::transforData(INT_DATA_LENGTH, SET_DATA_VARIABLE_REGISTER, addrTextSpeed, pSpeed);

		// Shows debug message if the flag is active
		if (serialDebugFlags.get(0))
		{
			Serial.println("writeTextSpeed");
		}
	}
}

// Shows the trip meter
void DwinPanelDisplay::writeTextTrip(uint16_t pTrip, bool force)
{
	static uint16_t previous;

	if ((previous != pTrip) || (force))
	{
		previous = pTrip;
		DwinDisplayUartProtocol::transforData(INT_DATA_LENGTH, SET_DATA_VARIABLE_REGISTER, addrTextTrip, pTrip);

		// Shows debug message if the flag is active
		if (serialDebugFlags.get(0))
		{
			Serial.println("writeTextTrip");
		}
	}
}

// Shows the odometer
void DwinPanelDisplay::writeTextOdometer(uint32_t pOdometer, bool force)
{
	static uint32_t previous;
	//force = true;
	if ((previous != pOdometer) || (force))
	{
		previous = pOdometer;
		DwinDisplayUartProtocol::transforData(LONG_DATA_LENGTH, SET_DATA_VARIABLE_REGISTER, addrTextOdometer, (uint16_t)(pOdometer >> 16), (uint16_t)(pOdometer & 0xFFFF));//

		// Shows debug message if the flag is active
		if (serialDebugFlags.get(0))
		{
			Serial.println("writeTextOdometer");
		}
	}
}

// Shows the MCU voltage indicator
void DwinPanelDisplay::showGraphMCUVoltage(double pVoltage, bool force)
{
	static uint8_t previous;
	uint8_t blocks = 0;

	if (pVoltage > 112.5)
	{
		blocks = 6;
	}
	else if (pVoltage > 107.0)
	{
		blocks = 5;
	}
	else if (pVoltage > 101.5)
	{
		blocks = 4;
	}
	else if (pVoltage > 96.0)
	{
		blocks = 3;
	}
	else if (pVoltage > 90.5)
	{
		blocks = 2;
	}
	else if (pVoltage > 85.0)
	{
		blocks = 1;
	}
	else
	{
		blocks = 0;
	}

	if ((previous != blocks) || (force))
	{
		previous = blocks;
		DwinDisplayUartProtocol::transforData(INT_DATA_LENGTH, SET_DATA_VARIABLE_REGISTER, addrGraphMCUVoltage, (uint16_t)(blocks & 0xFF));

		// Shows debug message if the flag is active
		if (serialDebugFlags.get(0))
		{
			Serial.println("showGraphMCUVoltage");
		}
	}
}

// 0=Eco, 1=City, 2=Pro, 4=Eco (Red), 5=City (Red), 6=Pro (Red)
void DwinPanelDisplay::showIconRidingMode(uint16_t pRidingMode, bool force)
{
	static uint16_t previous;

	if ((previous != pRidingMode) || (force))
	{
		previous = pRidingMode;
		DwinDisplayUartProtocol::transforData(INT_DATA_LENGTH, SET_DATA_VARIABLE_REGISTER, addrIconRidingMode, pRidingMode);

		// Shows debug message if the flag is active
		if (serialDebugFlags.get(0))
		{
			Serial.println("showIconRidingMode");
		}
	}
}

// Shows the warning signal
void DwinPanelDisplay::showIconWarning(bool show, bool force)
{
	static bool previous;

	if ((previous != show) || (force))
	{
		previous = show;
		DwinDisplayUartProtocol::transforData(INT_DATA_LENGTH, SET_DATA_VARIABLE_REGISTER, addrIconWarning, (uint16_t)(show & 0xFF));

		// Shows debug message if the flag is active
		if (serialDebugFlags.get(0))
		{
			Serial.println("showIconWarning");
		}
	}
}

// Shows warning message centralized
void DwinPanelDisplay::writeTextWarning(char *text, bool force)
{
	const uint8_t limit = 20;
	char warning[limit + 2];
	static char previous[limit + 2];
	String textAux = text;
	String textOut = "";
	uint8_t numSpacesAdd;

	// Limit the text size if bigger
	textAux.remove(limit);

	// Add spaces in left to keep centralized
	// If the lenght is less than the limit
	if (textAux.length() < limit)
	{
		// Calculates how many spaces is necessary
		numSpacesAdd = limit - textAux.length();

		// Divide by two
		numSpacesAdd /= 2;

		// Force one space in the beggining to not be together with the riding mode
		textOut.concat(" ");

		// Add spaces
		for (uint8_t i = 0; i < numSpacesAdd; i++)
		{
			textOut.concat(" ");
		}
	}
	textOut.concat(textAux);

	// Write in the screen
	textOut.toCharArray(warning, limit + 2);
	force = 1;	//wzh20230517 note 
	if ((strcmp(previous, warning) != 0) || (force))
	{
		strcpy(previous, warning);
		DwinDisplayUartProtocol::transforData(SET_DATA_VARIABLE_REGISTER, addrTextWarning, warning, limit + 1);

		// Shows debug message if the flag is active
		if (serialDebugFlags.get(0))
		{
			Serial.println("writeTextWarning");
		}
	}
}

// Shows the battery icon
void DwinPanelDisplay::showIconBattery(bool show, bool force)
{
	static bool previous;

	if ((previous != show) || (force))
	{
		previous = show;
		DwinDisplayUartProtocol::transforData(INT_DATA_LENGTH, SET_DATA_VARIABLE_REGISTER, addrIconBattery, (uint16_t)(show & 0xFF));

		// Shows debug message if the flag is active
		if (serialDebugFlags.get(0))
		{
			Serial.println("showIconBattery");
		}
	}
}

// Shows the OBD Icon
void DwinPanelDisplay::showIconOBD(bool show, bool force)
{
	static bool previous;

	if ((previous != show) || (force))
	{
		previous = show;
		DwinDisplayUartProtocol::transforData(INT_DATA_LENGTH, SET_DATA_VARIABLE_REGISTER, addrIconOBD, (uint16_t)(show & 0xFF));

		// Shows debug message if the flag is active
		if (serialDebugFlags.get(0))
		{
			Serial.println("showIconOBD");
		}
	}
}

// Shows turn left icon
void DwinPanelDisplay::showIconTurnLeft(bool show, bool force)
{
	static bool previous;

	if ((previous != show) || (force))
	{
		previous = show;
		DwinDisplayUartProtocol::transforData(INT_DATA_LENGTH, SET_DATA_VARIABLE_REGISTER, addrIconTurnLeft, (uint16_t)(show & 0xFF));

		// Shows debug message if the flag is active
		if (serialDebugFlags.get(0))
		{
			Serial.println("showIconTurnLeft");
		}
	}
}

// Shows turn right icon
void DwinPanelDisplay::showIconTurnRight(bool show, bool force)
{
	static bool previous;

	if ((previous != show) || (force))
	{
		previous = show;
		DwinDisplayUartProtocol::transforData(INT_DATA_LENGTH, SET_DATA_VARIABLE_REGISTER, addrIconTurnRight, (uint16_t)(show & 0xFF));

		// Shows debug message if the flag is active
		if (serialDebugFlags.get(0))
		{
			Serial.println("showIconTurnRight");
		}
	}
}

// Show Highbeam Icon
void DwinPanelDisplay::showIconHighbeam(bool show, bool force)
{
	static bool previous;

	if ((previous != show) || (force))
	{
		previous = show;
		DwinDisplayUartProtocol::transforData(INT_DATA_LENGTH, SET_DATA_VARIABLE_REGISTER, addrIconHighbeam, (uint16_t)(show & 0xFF));

		// Shows debug message if the flag is active
		if (serialDebugFlags.get(0))
		{
			Serial.println("showIconHighbeam");
		}
	}
}

// Shows the charging icon
void DwinPanelDisplay::showIconCharging(bool show, bool force)
{
	static bool previous;

	if ((previous != show) || (force))
	{
		previous = show;
		DwinDisplayUartProtocol::transforData(INT_DATA_LENGTH, SET_DATA_VARIABLE_REGISTER, addrIconCharging, (uint16_t)(show & 0xFF));

		// Shows debug message if the flag is active
		if (serialDebugFlags.get(0))
		{
			Serial.println("showIconCharging");
		}
	}
}

// Shows the SOC in the charging screen
void DwinPanelDisplay::writeTextSOCCharging(uint16_t pSOC, bool force)
{
	static uint16_t previous;

	if ((previous != pSOC) || (force))
	{
		previous = pSOC;
		DwinDisplayUartProtocol::transforData(INT_DATA_LENGTH, SET_DATA_VARIABLE_REGISTER, addrTextSOCCharging, pSOC);

		// Shows debug message if the flag is active
		if (serialDebugFlags.get(0))
		{
			Serial.println("writeTextSOCCharging");
		}
	}
}