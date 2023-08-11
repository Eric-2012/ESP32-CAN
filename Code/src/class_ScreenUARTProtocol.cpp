/******************************************************************************
 * func_DwinDisplayUartProtocol.cpp
 * This file has the protocol of the DWIN Display
 *****************************************************************************/

#include "class_ScreenUARTProtocol.h"

DwinDisplayUartProtocol::DwinDisplayUartProtocol(uint8_t Header_H, uint8_t Header_L)
{
	_header_h = Header_H;
	_header_l = Header_L;
}
DwinDisplayUartProtocol::~DwinDisplayUartProtocol() {}

uint16_t CRC_Check(uint8_t *RSdata, uint8_t length)
{
	uint16_t crc = 0xFFFF;
	for (uint8_t i = 3; i < length + 3; i++)
	{
		crc ^= *(RSdata + i);
		for (uint8_t j = 8; j > 0; j--)
		{
			if (crc & 0x0001)
				crc = (crc >> 1) ^ 0xA001;
			else
				crc >>= 1;
		}
	}
	return crc;
}

void DwinDisplayUartProtocol::transforData(uint8_t length, Register command, uint16_t address, uint16_t data)
{
	dwinDisplayProtocol.displayProtocolInt.header[0] = _header_h;
	dwinDisplayProtocol.displayProtocolInt.header[1] = _header_l;
	dwinDisplayProtocol.displayProtocolInt.length = length + CRCLENGTH;
	dwinDisplayProtocol.displayProtocolInt.command = command;
	dwinDisplayProtocol.displayProtocolInt.address[0] = address >> 8;
	dwinDisplayProtocol.displayProtocolInt.address[1] = address & 0xff;
	dwinDisplayProtocol.displayProtocolInt.zeros[0] = 0x00;
	dwinDisplayProtocol.displayProtocolInt.zeros[1] = 0x00;
	dwinDisplayProtocol.displayProtocolInt.data[0] = (uint8_t)((data >> 8) & 0xff);
	dwinDisplayProtocol.displayProtocolInt.data[1] = (uint8_t)(data & 0xff);
	dwinDisplayProtocol.displayProtocolInt.CRC[1] = CRC_Check(dwinDisplayProtocol.array, length) >> 8;
	dwinDisplayProtocol.displayProtocolInt.CRC[0] = CRC_Check(dwinDisplayProtocol.array, length) & 0xff;

	for (uint8_t i = 0; i < length + 3 + CRCLENGTH; i++)
	{
		SerialPanel.write(dwinDisplayProtocol.array[i]);
	}
	SerialPanel.flush();
}

void DwinDisplayUartProtocol::transforData(uint8_t length, Register command, uint16_t address, uint16_t data_h, uint16_t data_l)
{
	dwinDisplayProtocol.displayProtocolLong.header[0] = _header_h;
	dwinDisplayProtocol.displayProtocolLong.header[1] = _header_l;
	dwinDisplayProtocol.displayProtocolLong.length = length + CRCLENGTH;
	dwinDisplayProtocol.displayProtocolLong.command = command;

	dwinDisplayProtocol.displayProtocolLong.address[0] = address >> 8;
	dwinDisplayProtocol.displayProtocolLong.address[1] = address & 0xff;
	dwinDisplayProtocol.displayProtocolLong.zeros[0] = 0x00;
	dwinDisplayProtocol.displayProtocolLong.zeros[1] = 0x00;

	dwinDisplayProtocol.displayProtocolLong.data[0] = (uint8_t)((data_h >> 8) & 0xff);
	dwinDisplayProtocol.displayProtocolLong.data[1] = (uint8_t)(data_h & 0xff);
	dwinDisplayProtocol.displayProtocolLong.data[2] = (uint8_t)((data_l >> 8) & 0xff);
	dwinDisplayProtocol.displayProtocolLong.data[3] = (uint8_t)(data_l & 0xff);

	dwinDisplayProtocol.displayProtocolLong.CRC[1] = CRC_Check(dwinDisplayProtocol.array, length) >> 8;
	dwinDisplayProtocol.displayProtocolLong.CRC[0] = CRC_Check(dwinDisplayProtocol.array, length) & 0xff;

	for (uint8_t i = 0; i < length + 3 + CRCLENGTH; i++)
	{
		SerialPanel.write(dwinDisplayProtocol.array[i]);
	}

	SerialPanel.flush();
}

void DwinDisplayUartProtocol::transforFunction(uint8_t length, Register command, uint8_t address, uint8_t data_h, uint8_t data_l)
{
	dwinDisplayProtocol.displayProtocolFunctionInt.header[0] = _header_h;
	dwinDisplayProtocol.displayProtocolFunctionInt.header[1] = _header_l;
	dwinDisplayProtocol.displayProtocolFunctionInt.length = length + CRCLENGTH;
	dwinDisplayProtocol.displayProtocolFunctionInt.command = command;
	dwinDisplayProtocol.displayProtocolFunctionInt.address = address;
	dwinDisplayProtocol.displayProtocolFunctionInt.data[0] = data_h;
	dwinDisplayProtocol.displayProtocolFunctionInt.data[1] = data_l;
	dwinDisplayProtocol.displayProtocolFunctionInt.CRC[1] = CRC_Check(dwinDisplayProtocol.array, length) >> 8;
	dwinDisplayProtocol.displayProtocolFunctionInt.CRC[0] = CRC_Check(dwinDisplayProtocol.array, length) & 0xff;
	for (uint8_t i = 0; i < length + 3 + CRCLENGTH; i++)
	{
		SerialPanel.write(dwinDisplayProtocol.array[i]);
	}
	SerialPanel.flush();
}

void DwinDisplayUartProtocol::transforFunction(uint8_t length, Register command, uint8_t address, uint8_t data)
{
	dwinDisplayProtocol.displayProtocolFunctionByte.header[0] = _header_h;
	dwinDisplayProtocol.displayProtocolFunctionByte.header[1] = _header_l;
	dwinDisplayProtocol.displayProtocolFunctionByte.length = length + CRCLENGTH;
	dwinDisplayProtocol.displayProtocolFunctionByte.command = command;
	dwinDisplayProtocol.displayProtocolFunctionByte.address = address;
	dwinDisplayProtocol.displayProtocolFunctionByte.data = data;
	dwinDisplayProtocol.displayProtocolFunctionByte.CRC[1] = CRC_Check(dwinDisplayProtocol.array, length) >> 8;
	dwinDisplayProtocol.displayProtocolFunctionByte.CRC[0] = CRC_Check(dwinDisplayProtocol.array, length) & 0xff;
	for (uint8_t i = 0; i < length + 3 + CRCLENGTH; i++)
	{
		SerialPanel.write(dwinDisplayProtocol.array[i]);
	}
	SerialPanel.flush();
}

void DwinDisplayUartProtocol::transforData(Register command, uint16_t address, char *data, uint8_t data_length)
{
	dwinDisplayProtocol.displayProtocolString.Header[0] = _header_h;
	dwinDisplayProtocol.displayProtocolString.Header[1] = _header_l;
	dwinDisplayProtocol.displayProtocolString.Lenth = 5 + 90 + CRCLENGTH;
	dwinDisplayProtocol.displayProtocolString.command = command;
	dwinDisplayProtocol.displayProtocolString.address[0] = address >> 8;
	dwinDisplayProtocol.displayProtocolString.address[1] = address & 0xff;
	dwinDisplayProtocol.displayProtocolString.zeros[0] = 0x00;
	dwinDisplayProtocol.displayProtocolString.zeros[1] = 0x00;
	for (uint8_t i = 0; i < data_length; i++)
	{
	 	dwinDisplayProtocol.displayProtocolString.data[i] = (int8_t)data[i];
	}

	dwinDisplayProtocol.displayProtocolString.data[88] = 0xff;
	dwinDisplayProtocol.displayProtocolString.data[89] = 0xff;
	_crc_temp = CRC_Check(dwinDisplayProtocol.array, dwinDisplayProtocol.displayProtocolString.Lenth - 2);
	dwinDisplayProtocol.displayProtocolString.CRC[1] = _crc_temp >> 8;
	dwinDisplayProtocol.displayProtocolString.CRC[0] = _crc_temp & 0xff;

	for (uint8_t i = 0; i < dwinDisplayProtocol.displayProtocolString.Lenth + 3; i++)
	{
		SerialPanel.write(dwinDisplayProtocol.array[i]);
	}
	SerialPanel.flush();
}

void DwinDisplayUartProtocol::transforDataDualAddressByte(Register command, uint16_t address, char *data, uint8_t data_length)
{
	dwinDisplayProtocol.displayProtocolStringDualByteAddress.header[0] = _header_h;
	dwinDisplayProtocol.displayProtocolStringDualByteAddress.header[1] = _header_l;
	dwinDisplayProtocol.displayProtocolStringDualByteAddress.length = 5 + 90 + CRCLENGTH;
	dwinDisplayProtocol.displayProtocolStringDualByteAddress.command = command;
	dwinDisplayProtocol.displayProtocolStringDualByteAddress.address = address;
	dwinDisplayProtocol.displayProtocolStringDualByteAddress.zeros[0] = 0x00;
	dwinDisplayProtocol.displayProtocolStringDualByteAddress.zeros[1] = 0x00;
	for (uint8_t i = 0; i < data_length; i++)
	{
		dwinDisplayProtocol.displayProtocolStringDualByteAddress.data[i] = (int8_t)data[i];
	}

	dwinDisplayProtocol.displayProtocolStringDualByteAddress.data[88] = 0xff;
	dwinDisplayProtocol.displayProtocolStringDualByteAddress.data[89] = 0xff;
	_crc_temp = CRC_Check(dwinDisplayProtocol.array, dwinDisplayProtocol.displayProtocolStringDualByteAddress.length - 2);
	dwinDisplayProtocol.displayProtocolStringDualByteAddress.CRC[1] = _crc_temp >> 8;
	dwinDisplayProtocol.displayProtocolStringDualByteAddress.CRC[0] = _crc_temp & 0xff;

	for (uint8_t i = 0; i < dwinDisplayProtocol.displayProtocolStringDualByteAddress.length + 3; i++)
	{
		SerialPanel.write(dwinDisplayProtocol.array[i]);
	}
	SerialPanel.flush();
}