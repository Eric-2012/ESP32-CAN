/******************************************************************************
 * misc.cpp
 * Main file
 *****************************************************************************/

#include "main.h"

void setup()
{
  /* #region INIT_PINS */
  pinMode(pinReset, OUTPUT);
  pinMode(pinWP_FRAM, OUTPUT);
  pinMode(pinHSPI_CS_Acc, OUTPUT);
  pinMode(pinHSPI_CS_Flash, OUTPUT);
  pinMode(pinVSPI_CS_Can, OUTPUT);

  // Keep FRAM protected against Writing
  digitalWrite(pinWP_FRAM, HIGH);

  // Make sure that the pin for the 12V is used just for analog conversion
  adcAttachPin(pin12VMonitor);

  // Init serial ports (The serial ports for display and BMS are started in their task files)
  // As seen in https://github.com/G6EJD/ESP32-Using-Hardware-Serial-Ports/blob/master/ESP32_Using_Serial2.ino
  Serial.begin(115200);

  // Init I2C Bus
  I2C0.begin(pinSDA, pinSCL, 400000);

  // Init peripherals
  disablePeripheralComponents(); // Put 0 in the reset pin that goes for I2C Expander and CAN chip
  delay(20);
  activatePeripheralComponents(); // Put 1 in the reset pin that goes for I2C Expander and CAN chip
  delay(20);
  configExpanders(); // Initialize the I2C Expanders
  /* #endregion */

  /* #region INIT_VARIABLES */
  vehicleInfo.tripKm = 0;
  vehicleInfo.isInReverseMode = false;

  // Create paremeter objects and read from FRAM
  configParametersObjects();
  readAllParameters();
  /* #endregion */

  // Set the pins that change according to parameters
  // The Embedded CAN is not being used in V2.3.3.2, so it is just if is different
  if (strcmp(vehicleParam.ECUHardwareVersion, "2.3.3.2") != 0)
  {
    pinTXCAN2 = GPIO_NUM_13; // CAN2_TXD - TX from embedded CAN Controller
    pinRXCAN2 = GPIO_NUM_32; // CAN2_RXD - RX from embedded CAN Controller
  }

  // In ECU V2.3.3.2 the HSPI_CS pin is 13, in the V2.3.3.3 is 2
  if (strcmp(vehicleParam.ECUHardwareVersion, "2.3.3.2") == 0)
  {
    pinHSPI_MOSI = 13; // HSPI_MOSI - MOSI for HSPI
  }
  else
  {
    pinHSPI_MOSI = 2; // HSPI_MOSI - MOSI for HSPI
  }

  // Init SPI Buses now, because of the pins change
  HSPIBus.begin(pinHSPI_CLK, pinHSPI_MISO, pinHSPI_MOSI);
  VSPIBus.begin(pinVSPI_CLK, pinVSPI_MISO, pinVSPI_MOSI);

  // Starts the heartbeat ticker each 500ms
  tickHeartbeat.attach_ms(500, heartbeat);

  // If the motorcyle has transmission, it means that the RPM in the wheel is different from the RPM from the MCU
  if (vehicleParam.hasTransmission)
  {
    // So it's necessary to get the RPM from a speed sensor conected to the wheel and analyzed by an interruption
    attachInterrupt(digitalPinToInterrupt(pinSpeedSensor), ifSpeedSensorRose, RISING);

    // But as when the bike stops, there is no interruption, something need to be running from time to time to make sure that the RPM will be 0 when it is stopped
    // Each 500ms, verifies if the last pulse was more than 1 second ago, in this case put 0 as RPM
    // IMPORTANT: It could be used something inside the interruption that happen when a pulse occur, but there it is necessary to be faster as possible, so better to keep outside
    tickForceRPMTo0.attach_ms(500, forceRPMTo0);
  }

  // Prepare the array of errors, already prepared to receive errors from E001 to E999
  // It is also necessary to update the array of error messages in the format EXXX TEXT in the file func_Errors.h
  // and change the constant that has the number of errors in the same file
  errorArray.begin(1000);
  errorArray.clear();

  // In the beggining of the task modeSelection there is a while waiting till receive at least one packet from the BMS and from the MCU, after that the variable "vehicleInfo.passedInitCheckup" is set
  // This is necessary because the mode changes according to the SOC. So if there is no data, the SOC will be 0, starting in ECO mode. And the variable "vehicleInfo.passedInitCheckup" can be used in other places as a "Good to go" flag
  // So to make sure that a packet will be received, the timeout errors from BMS and MCU start triggered
  // It will be clean when a packet is received, then the task modeSelector starts.
  errorArray.set(300, 1);
  errorArray.set(302, 1);

  // There are a lot of Serial.prinln commands used for debug all along the code
  // This array contains the flags to show the debug messages in each session
  serialDebugFlags.begin(32);
  serialDebugFlags.clear();
  serialDebugFlags.set(0, false); // Show display serial debug messages
  serialDebugFlags.set(1, false); // Show BMS serial debug messages

  /* #region AVGCALC Conf */
  avgBMSTemp.confAvgCalc(10, checkVarType(&dummyInt16), 20);       // Configurates avgBMSTemp object and starts the array with 20°C
  avgMotorTemp.confAvgCalc(10, checkVarType(&dummyInt16), 20);     // Configurates avgMotorTemp object and starts the array with 20°C
  avgMCUVoltage.confAvgCalc(200, checkVarType(&dummyUint16));      // Configurates avgMCUVoltage object and starts the array with 0
  avgMCUCurrentOut.confAvgCalc(20, checkVarType(&dummyUint8));     // Configurates avgMCUCurrentOut object and starts the array with 0
  avgECUSupplyVoltage.confAvgCalc(20, checkVarType(&dummyUint16)); // Configurates avgECUSupplyVoltage object and starts the array with 0
  avgEfficiency.confAvgCalc(20, checkVarType(&dummyInt16));        // Configurates avgEfficiency object and starts the array with 0
  /* #endregion */

  /* #region PWRREDUCTION Conf */
  pwrRedFetTemp.confPwrReductionCalc(50, 70, 70, 30, checkVarType(&dummyInt16), 20);     // Configurates pwrRedFetTemp object and starts the array with 20°C
  pwrRedBatTemp.confPwrReductionCalc(50, 65, 70, 30, checkVarType(&dummyInt16), 20);     // Configurates pwrRedBatTemp object and starts the array with 20°C
  pwrRedMotorTemp.confPwrReductionCalc(120, 135, 70, 30, checkVarType(&dummyInt16), 20); // Configurates pwrRedMotorTemp object and starts the array with 20°C
  /* #endregion */

  /* #region INIT_TASKS */
  // == TASKS IN CORE1 ========================================================
  task_BusI2C.runInCore1(taskCode_BusI2C, "BusI2C", 3000, 25, 0);
  task_UpdateIO.runInCore1(taskCode_UpdateIO, "UpdateIO", 1000, 25, 1);
  task_ModeSelector.runInCore1(taskCode_ModeSelector, "ModeSelector", 1000, 25, 2);

  // Just start the screen task if the screen is UART
  if (vehicleParam.screenType == 0) 
    task_ScreenUART.runInCore1(taskCode_ScreenUART, "ScreenUart", 3000, 50, 3);

  task_InertialSensor.runInCore1(taskCode_BusHSPI, "BusHSPI", 3000, 50, 4);

  // == TASKS IN CORE0 ========================================================
  task_Errors.runInCore0(taskCode_Errors, "Errors", 1000, 10, 0);
  task_BusCANExt.runInCore0(taskCode_BusCANExt, "BusCANExt", 3000, 10, 1);

  // Just start the BMS task if the BMS data is coming via UART
  // if (vehicleParam.BMSType == 1)
  // task_BusCANExt.runInCore0(taskCode_BusCANExt, "BusCANExt", 3000, 20, 2);
    // task_BMSUART.runInCore0(taskCode_BMSUART, "BMS", 3000, 20, 2);

  task_Bluetooth.runInCore0(taskCode_Bluetooth, "Bluetooth", 3000, 500, 3);
  task_BLE.runInCore0(taskCode_BLE, "BLE", 1500, 1000, 4);
  /*#endregion */
}

void loop()
{
  // As the functions SerialEventX are not implemented for ESP32, this was is used to replace it
  // Serial0 takes care about commucation with computer
  if (Serial.available())
  {
    serialEvent();
  }
}