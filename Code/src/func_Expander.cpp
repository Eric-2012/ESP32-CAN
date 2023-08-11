/******************************************************************************
 * func_Expander.cpp
 * This file has functions to configure the I2C Expanders, one as output and
 * one as input, and the functions to control inputs and outputs.
 * PS: In the tests, weird things were happening if all the config registers
 * were not sent.
 *****************************************************************************/

#include "func_Expander.h"

// Configure the I2C expanders
void configExpanders()
{
	// Configure expander for outputs
	I2C0.beginTransmission(addrI2C_Outputs);
	I2C0.write(0x00);		// Address 0x00
	I2C0.write(0b00000000); // IODIRA - PORTA Direction (0 as output, 1 as input)
	I2C0.write(0b00000000); // IODIRB - PORTB Direction (0 as output, 1 as input)
	I2C0.write(0b00000000); // IPOLA - PORTA Polarization (0 register is the same as pin, 1 is inverted)
	I2C0.write(0b00000000); // IPOLB - PORTB Polarization (0 register is the same as pin, 1 is inverted)
	I2C0.write(0b00000000); // GPINTENA - PORTA On Change interrupt activation (0 as disabled, 1 as activated)
	I2C0.write(0b00000000); // GPINTENB - PORTB On Change interrupt activation (0 as disabled, 1 as activated)
	I2C0.write(0b00000000); // DEFVALA - PORTA Default value for interruption
	I2C0.write(0b00000000); // DEFVALB - PORTB Default value for interruption
	I2C0.write(0b00000000); // INTCONA - PORTA Interruption behavior (0 every time on change, 1 just when change and it is different from default value)
	I2C0.write(0b00000000); // INTCONB - PORTB Interruption behavior (0 every time on change, 1 just when change and it is different from default value)
	I2C0.write(0b00000000); // IOCONA - Check datasheet
	I2C0.write(0b00000000); // IOCONB - Check datasheet
	I2C0.write(0b00000000); // GPPUA - PORTA PullUp resistor (0 without, 1 as with)
	I2C0.write(0b00000000); // GPPUB - PORTB PullUp resistor (0 without, 1 as with)
	I2C0.endTransmission();
	// Make sure that the GPIOs will start in 0
	I2C0.beginTransmission(addrI2C_Outputs);
	I2C0.write(0x12);		// Address 0x12
	I2C0.write(0b00000000); // GPIOA
	I2C0.write(0b00000000); // GPIOB
	I2C0.write(0b00000000); // OLATA
	I2C0.write(0b00000000); // OLATB
	I2C0.endTransmission();

	// Configure expander for inputs
	I2C0.beginTransmission(addrI2C_Inputs);
	I2C0.write(0x00);		// Address 0x00
	I2C0.write(0b11111111); // IODIRA - PORTA Direction (0 as output, 1 as input)
	I2C0.write(0b11111111); // IODIRB - PORTB Direction (0 as output, 1 as input)
	I2C0.write(0b00000000); // IPOLA - PORTA Polarization (0 register is the same as pin, 1 is inverted)
	I2C0.write(0b00000000); // IPOLB - PORTB Polarization (0 register is the same as pin, 1 is inverted)
	I2C0.write(0b00000000); // GPINTENA - PORTA On Change interrupt activation (0 as disabled, 1 as activated)
	I2C0.write(0b00000000); // GPINTENB - PORTB On Change interrupt activation (0 as disabled, 1 as activated)
	I2C0.write(0b00000000); // DEFVALA - PORTA Default value for interruption
	I2C0.write(0b00000000); // DEFVALB - PORTB Default value for interruption
	I2C0.write(0b00000000); // INTCONA - PORTA Interruption behavior (0 every time on change, 1 just when change and it is different from default value)
	I2C0.write(0b00000000); // INTCONB - PORTB Interruption behavior (0 every time on change, 1 just when change and it is different from default value)
	I2C0.write(0b00000000); // IOCONA - Check datasheet
	I2C0.write(0b00000000); // IOCONB - Check datasheet
	I2C0.write(0b00000000); // GPPUA - PORTA PullUp resistor (0 without, 1 as with)
	I2C0.write(0b00001000); // GPPUB - PORTB PullUp resistor (0 without, 1 as with)
	I2C0.endTransmission();
	// Make sure that the GPIOs will start in 0
	I2C0.beginTransmission(addrI2C_Inputs);
	I2C0.write(0x12);		// Address 0x12
	I2C0.write(0b00000000); // GPIOA
	I2C0.write(0b00000000); // GPIOB
	I2C0.write(0b00000000); // OLATA
	I2C0.write(0b00000000); // OLATB
	I2C0.endTransmission();
}

// Update inputs and outputs
void updateIOs()
{
	uint8_t I2CError;
	uint8_t aux;
	uint16_t outputsI2C = 0;
	uint16_t inputsI2C = 0;

	// PROTECTION TO NOT USE RD8
	// It might cause a short circuit, check the wire harness before use it
	outputs.rd8 = false;

	// Trigger aux relay K1 for power on the UTC 
	// Allow chraging when the key is off
     outputs.r1 = true;  
     
	// Check output variables and create the data that will be sent to the I2C Expander
	// The output names are the same as shown in the schematic
	bitWrite(outputsI2C, 2, outputs.r1);   
	bitWrite(outputsI2C, 1, outputs.r2);    
	bitWrite(outputsI2C, 15, outputs.rd1);
	bitWrite(outputsI2C, 14, outputs.rd2);
	bitWrite(outputsI2C, 13, outputs.rd3);
	bitWrite(outputsI2C, 3, outputs.rd4);
	bitWrite(outputsI2C, 6, outputs.rd5);
	bitWrite(outputsI2C, 7, outputs.rd6);
	bitWrite(outputsI2C, 0, outputs.rd7);
	bitWrite(outputsI2C, 4, outputs.rd8);
	bitWrite(outputsI2C, 5, outputs.rd9);
	bitWrite(outputsI2C, 12, outputs.led);

	// For saving in flash memory
	outputs.outputsI2CDebug = outputsI2C;

	// Send outputs
	I2C0.beginTransmission(addrI2C_Outputs);
	I2C0.write(0x14);	   // Address 0x14 OLATA
	aux = outputsI2C >> 8; // Get high byte of the outputs variable and send to OLATA
	I2C0.write(aux);
	aux = outputsI2C; // Get low byte of the outputs variable and send to OLATB
	I2C0.write(aux);
	I2CError = I2C0.endTransmission(false);
	if (I2CError != 0)
	{
		Serial.printf("I2C Error: %u\n", I2CError);
	}

	// Get inputs
	I2C0.beginTransmission(addrI2C_Inputs);
	I2C0.write(0x12);
	I2CError = I2C0.endTransmission(false);
	if (I2CError != 0)
	{
		Serial.printf("I2C Error: %u\n", I2CError);
	}
	I2C0.requestFrom(addrI2C_Inputs, 2);
	inputsI2C = (I2C0.read() << 8);
	inputsI2C += I2C0.read();

	// The hardware connection doesn't follow the pin number/bit order
	// This part links the expander pins with the inputs using the same names as shown in the schematic
	inputs.in1 = bitRead(inputsI2C, 7);
	inputs.in2 = bitRead(inputsI2C, 5);
	inputs.in3 = bitRead(inputsI2C, 6);
	inputs.in4 = bitRead(inputsI2C, 4);
	inputs.in5 = bitRead(inputsI2C, 3);
	inputs.in6 = bitRead(inputsI2C, 0);
	inputs.in7 = bitRead(inputsI2C, 2);
	inputs.in8 = bitRead(inputsI2C, 1);
	inputs.in9 = bitRead(inputsI2C, 12);
	inputs.in10 = bitRead(inputsI2C, 14);
	inputs.in11 = bitRead(inputsI2C, 13);
	inputs.in12 = bitRead(inputsI2C, 15);

	// To save in flash memory
	inputs.inputsI2CDebug = inputsI2C;

	/*Serial.print(powerFlag);
	Serial.print(in12);
	Serial.print(in11);
	Serial.print(in10);
	Serial.print(in9);
	Serial.print(in8);
	Serial.print(in7);
	Serial.print(in6);
	Serial.print(in5);
	Serial.print(in4);
	Serial.print(in3);
	Serial.print(in2);
	Serial.println(in1);*/
}