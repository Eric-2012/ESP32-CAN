/******************************************************************************
 * func_DwinDisplayUartProtocol.h
 * Header for func_DwinDisplayUartProtocol.cpp
 *****************************************************************************/
#ifndef __CLASS_DWINDISPLAYUARTPROTOCOL_H__
#define __CLASS_DWINDISPLAYUARTPROTOCOL_H__

#include "Arduino.h"
#include "global_Constants.h"
#include "global_Variables.h"

#define CRCLENGTH 2

class DwinDisplayUartProtocol
{
protected:
	uint8_t _header_h;
	uint8_t _header_l;

private:
	uint16_t _crc_temp;

public:
	String input_string;
	//input_string.reserve(13); // makes the string length 12 bytes long
	const uint8_t screenCmdChangePage = 0x03;
	const uint8_t screenCmdDisableTouch = 0x0B;
	const uint8_t screenCmdHandshake = 0x00;
	//const uint8_t brightness = 0x01;
	enum Register
	{
		SET_DATA_REGISTER = 0x80,
		GET_DATA_REGISTER,
		SET_DATA_VARIABLE_REGISTER,
		GET_DATA_VARIABLE_REGISTER,
		SET_DATA_CURVE
	};

	struct DisplayProtocolInt
	{
		uint8_t header[2];
		uint8_t length;
		uint8_t command;
		uint8_t address[2];
		uint8_t zeros[2];
		uint8_t data[2];
		uint8_t CRC[2];
	};

	struct DisplayProtocolLong
	{
		uint8_t header[2];
		uint8_t length;
		uint8_t command;
		uint8_t address[2];
		uint8_t zeros[2];
		uint8_t data[4];
		uint8_t CRC[2];
	};

	struct DisplayProtocolFunctionInt
	{
		uint8_t header[2];
		uint8_t length;
		uint8_t command;
		uint8_t address;
		uint8_t data[2];
		uint8_t CRC[2];
	};

	struct DisplayProtocolFunctionByte
	{
		uint8_t header[2];
		uint8_t length;
		uint8_t command;
		uint8_t address;
		uint8_t data;
		uint8_t CRC[2];
	};

	struct DisplayProtocolCallBack
	{
		uint8_t header[2];
		uint8_t length;
		uint8_t command;
		uint8_t address[2];
		uint8_t data_length;
		uint8_t Data[8];
		uint8_t CRC[2];
	};

	struct DisplayProtocolString
	{
		uint8_t Header[2];
		uint8_t Lenth;
		uint8_t command;
		uint8_t address[2];
		uint8_t zeros[2];
		uint8_t data[90];
		uint8_t CRC[2];
	};

	struct DisplayProtocolStringDualByteAddress
	{
		uint8_t header[2];
		uint8_t length;
		uint8_t command;
		uint16_t address;
		uint8_t zeros[2];
		uint8_t data[90]; //27
		uint8_t CRC[2];
	};

	union
	{
		DisplayProtocolInt displayProtocolInt;
		DisplayProtocolLong displayProtocolLong;
		DisplayProtocolFunctionInt displayProtocolFunctionInt;
		DisplayProtocolFunctionByte displayProtocolFunctionByte;
		DisplayProtocolCallBack displayProtocolCallBack;
		DisplayProtocolString displayProtocolString;
		DisplayProtocolStringDualByteAddress displayProtocolStringDualByteAddress;
		uint8_t array[100]; //14
	} dwinDisplayProtocol;

public:
	DwinDisplayUartProtocol(uint8_t Header_H, uint8_t Header_L);
	~DwinDisplayUartProtocol();
	void transforData(uint8_t length, Register command, uint16_t address, uint16_t data);
	void transforData(uint8_t length, Register command, uint16_t address, uint16_t data_h, uint16_t data_l);
	void transforData(Register command, uint16_t address, char *data, uint8_t data_length);
	void transforDataDualAddressByte(Register command, uint16_t address, char *data, uint8_t data_length);
	void transforFunction(uint8_t length, Register command, uint8_t address, uint8_t data_h, uint8_t data_l);
	void transforFunction(uint8_t length, Register command, uint8_t address, uint8_t data);

private:
};
#endif