/******************************************************************************
 * tasks_Bluetooh.cpp
 * This file has the codes for the task that takes care of classic bluetooth and
 * from the task that get data from the TPMS sensor.
 * They are in the same file because to be possible to use both together, it's
 * necessary to follow the right sequence to start both.
 *****************************************************************************/

#include "tasks_Bluetooth.h"
BluetoothSerial SerialBT; // Used to send serial data via bluetooh
BLEScan *pBLEScan;        // Used to scan the BLE beacons

// Callback executed for each BLE beacon found, to detect if it's coming from the TPMS sensor
class BLEDevices : public BLEAdvertisedDeviceCallbacks
{
    void onResult(BLEAdvertisedDevice BLEDevice)
    {
        String data = BLEDevice.toString().c_str();
        if (data.indexOf("858c8986eb1041b2b39e81bf7734b6eb") > 0)
        {
            data = data.substring(data.length() - 10);
            if ((int)strtol(data.substring(8, 10).c_str(), 0, 16) == 200)
            {
                uint16_t TPMSmajor = (((uint8_t)strtol(data.substring(0, 2).c_str(), 0, 16)) << 8) + ((uint8_t)strtol(data.substring(2, 4).c_str(), 0, 16));
                uint16_t TPMSminor = (((uint8_t)strtol(data.substring(4, 6).c_str(), 0, 16)) << 8) + ((uint8_t)strtol(data.substring(6, 8).c_str(), 0, 16));

                uint16_t TPMSid = (((TPMSminor >> 7) & 0x01) * 10000) +   // ID4
                                  (((TPMSmajor & 0xF000) >> 12) * 1000) + // ID3
                                  (((TPMSmajor & 0x0F00) >> 8) * 100) +   // ID2
                                  (((TPMSmajor & 0x00F0) >> 4) * 10) +    // ID1
                                  (TPMSmajor & 0x000F);                   // ID0

                // If the ID is ODD, it means that is from the front tire
                if (TPMSid % 2 != 0)
                {
                    vehicleInfo.frontTirePressure = 0.362595 * ((TPMSminor & 0xFF00) >> 8);
                    vehicleInfo.frontTireTemperature = -40 + (TPMSminor & 0x7F);
                    vehicleInfo.frontTireSensorRSSI = BLEDevice.getRSSI();
                }
                // If the ID is EVEN, it means that is from the rear tire
                else if (TPMSid % 2 == 0)
                {
                    vehicleInfo.rearTirePressure = 0.362595 * ((TPMSminor & 0xFF00) >> 8);
                    vehicleInfo.rearTireTemperature = -40 + (TPMSminor & 0x7F);
                    vehicleInfo.rearTireSensorRSSI = BLEDevice.getRSSI();
                }
            }
        }
    }
};

// Code for the task that takes care of classic Bluetooth
void taskCode_Bluetooth(void *pvParameters)
{
    // Init BLE device
    BLEDevice::init("");

    // Init Bluetooth device
    SerialBT.begin(vehicleInfo.btDeviceName);

    // Configure BLE
    pBLEScan = BLEDevice::getScan(); //create new scan
    pBLEScan->setAdvertisedDeviceCallbacks(new BLEDevices());
    pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99); // less or equal setInterval value

    // Indicated that both bluetooth already started
    vehicleInfo.bluetoothRunning = true;

    while (true)
    {
        // Start execution debug
        task_Bluetooth.initDebugExec();
        
        prepareInfoToSave();
        printDebugViaBluetooth();

        // End execution debug
        task_Bluetooth.endDebugExec(uxTaskGetStackHighWaterMark(NULL));

        // Put the task to sleep
        vTaskDelay(task_Bluetooth.getInterval() / portTICK_PERIOD_MS);
    }
}

// Code for the task that takes care of Buetooth low energy
void taskCode_BLE(void *pvParameters)
{
    // While the BLE device is not initialized by the main bluetooth task, this task keep sleeping
    while (!vehicleInfo.bluetoothRunning)
    {
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }

    while (true)
    {
        // Start execution debug
        task_BLE.initDebugExec();

        BLEScanResults foundDevices = pBLEScan->start(5, false);
        pBLEScan->clearResults(); // delete results fromBLEScan buffer to release memory

        // End execution debug
        task_BLE.endDebugExec(uxTaskGetStackHighWaterMark(NULL));

        // Put the task to sleep
        vTaskDelay(task_BLE.getInterval() / portTICK_PERIOD_MS);
    }
}

// Prints the struct infoToSave via bluetooth
void printDebugViaBluetooth()
{
    String datalog = ";";
    datalog.concat(infoToSave.packetCode);
    datalog.concat(";");
    datalog.concat(infoToSave.second);
    datalog.concat(";");
    datalog.concat(infoToSave.minute);
    datalog.concat(";");
    datalog.concat(infoToSave.hour);
    datalog.concat(";");
    datalog.concat(infoToSave.dayW);
    datalog.concat(";");
    datalog.concat(infoToSave.dayM);
    datalog.concat(";");
    datalog.concat(infoToSave.month);
    datalog.concat(";");
    datalog.concat(infoToSave.year);
    datalog.concat(";");
    datalog.concat(infoToSave.boardTemperature);
    datalog.concat(";");
    datalog.concat(infoToSave.boardSupplyVoltage);
    datalog.concat(";");
    datalog.concat(infoToSave.odometerKm);
    datalog.concat(";");
    datalog.concat(infoToSave.tripKm);
    datalog.concat(";");
    datalog.concat(infoToSave.speedKmh);
    datalog.concat(";");
    datalog.concat(infoToSave.maximumSpeed);
    datalog.concat(";");
    datalog.concat(infoToSave.rpm);
    datalog.concat(";");
    datalog.concat(infoToSave.efficiency);
    datalog.concat(";");
    datalog.concat(infoToSave.vehicleStatuByte1);
    datalog.concat(";");
    datalog.concat(infoToSave.vehicleStatuByte2);
    datalog.concat(";");
    datalog.concat(infoToSave.numActiveErrors);
    datalog.concat(";");
    datalog.concat(infoToSave.sumActiveErrors);
    datalog.concat(";");
    datalog.concat(infoToSave.roll);
    datalog.concat(";");
    datalog.concat(infoToSave.pitch);
    datalog.concat(";");
    datalog.concat(infoToSave.frontTirePressure);
    datalog.concat(";");
    datalog.concat(infoToSave.frontTireTemperature);
    datalog.concat(";");
    datalog.concat(infoToSave.rearTirePressure);
    datalog.concat(";");
    datalog.concat(infoToSave.rearTireTemperature);
    datalog.concat(";");
    datalog.concat(infoToSave.inputsI2C);
    datalog.concat(";");
    datalog.concat(infoToSave.outputsI2C);
    datalog.concat(";");
    datalog.concat(infoToSave.BMSVoltage);
    datalog.concat(";");

    // Convert the cell voltages
    // Value 0 = 3V. Incrementing of 0.01V each bit increment.
    for (uint8_t x = 0; x < 32; x++)
    {
        datalog.concat(infoToSave.BMSCellVoltages[x]);
        datalog.concat(";");
    }

    datalog.concat(infoToSave.BMSCurrent);
    datalog.concat(";");

    for (uint8_t x = 0; x < 6; x++)
    {
        datalog.concat(infoToSave.BMSTemperatures[x]);
        datalog.concat(";");
    }

    datalog.concat(infoToSave.BMSSoc);
    datalog.concat(";");
    datalog.concat(infoToSave.BMSBatteryPhysicalCapacity);
    datalog.concat(";");
    datalog.concat(infoToSave.BMSBatteryRemainingCapacity);
    datalog.concat(";");
    datalog.concat(infoToSave.BMSTurnOnTime);
    datalog.concat(";");
    datalog.concat(infoToSave.BMSCellHighestVoltageNumber);
    datalog.concat(";");
    datalog.concat(infoToSave.BMSCellHighestVoltageValue);
    datalog.concat(";");
    datalog.concat(infoToSave.BMCCellLowestVoltageNumber);
    datalog.concat(";");
    datalog.concat(infoToSave.BMSCellLowestVoltageValue);
    datalog.concat(";");
    datalog.concat(infoToSave.BMSNumberOfBatteries);
    datalog.concat(";");
    datalog.concat(infoToSave.BMSChargingMosfetStatus);
    datalog.concat(";");
    datalog.concat(infoToSave.BMSDischargingMosfetStatus);
    datalog.concat(";");
    datalog.concat(infoToSave.BMSBalanceStateSign);
    datalog.concat(";");
    datalog.concat(infoToSave.MCUVoltage);
    datalog.concat(";");
    datalog.concat(infoToSave.MCUBusCurrent);
    datalog.concat(";");
    datalog.concat(infoToSave.MCURpm);
    datalog.concat(";");
    datalog.concat(infoToSave.MCUMotorTemperature);
    datalog.concat(";");
    datalog.concat(infoToSave.MCUControllerTemperature);
    datalog.concat(";");
    datalog.concat(infoToSave.MCUThrottle);
    datalog.concat(";");
    datalog.concat(infoToSave.MCULowByteError);
    datalog.concat(";");
    datalog.concat(infoToSave.MCUHighByteError);
    datalog.concat(";");
    datalog.concat(infoToSave.MCUStatusByte);
    datalog.concat(";");
    datalog.concat(infoToSave.MCUOutCurrent);
    datalog.concat(";");
    datalog.concat(infoToSave.MCUMaxRegen);
    datalog.concat(";");
    datalog.concat(infoToSave.maximumThrottle);
    datalog.concat(";");
    datalog.concat(infoToSave.minimumThrottle);
    datalog.concat(";");
    datalog.concat(infoToSave.chargerVoltage);
    datalog.concat(";");
    datalog.concat(infoToSave.chargerCurrent);
    datalog.concat(";");
    datalog.concat(infoToSave.chargerStatusByte);
    datalog.concat(";");
    datalog.concat(infoToSave.maxChargerVoltage);
    datalog.concat(";");
    datalog.concat(infoToSave.maxChargerCurrent);
    datalog.concat(";");
    datalog.concat(infoToSave.millis);
    datalog.concat(";");
    datalog.concat(infoToSave.resetReason);
    datalog.concat(";");

    // Print data to bluetooth
    SerialBT.println(datalog);
}