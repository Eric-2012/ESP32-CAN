/******************************************************************************
 * func_RTC.cpp
 * This file has the functions set the time in the RTC Device, read the time and
 * read the temperature from it.
 *****************************************************************************/

#include "func_RTC.h"

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
    return ((val / 10 * 16) + (val % 10));
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
    return ((val / 16 * 10) + (val % 16));
}

// Set the time if necessary and update the variables board temperature and time
void updateRTC()
{
    // Check if it is necessary to update the time
    if (flagTimeToUpdate)
    {
        // Sets time and date to DS3231
        I2C0.beginTransmission(addrI2C_RTC);
        I2C0.write(0);                             // set next input to start at the seconds register
        I2C0.write(decToBcd(timeToUpdate.second)); // set seconds
        I2C0.write(decToBcd(timeToUpdate.minute)); // set minutes
        I2C0.write(decToBcd(timeToUpdate.hour));   // set hours
        I2C0.write(decToBcd(timeToUpdate.dayW));   // set day of week (1=Sunday, 7=Saturday)
        I2C0.write(decToBcd(timeToUpdate.dayM));   // set day (1 to 31)
        I2C0.write(decToBcd(timeToUpdate.month));  // set month
        I2C0.write(decToBcd(timeToUpdate.year));   // set year (0 to 99)
        I2C0.endTransmission();
        flagTimeToUpdate = false;
    }

    // Update the struct timeNow with the actual time
    I2C0.beginTransmission(addrI2C_RTC);
    I2C0.write(0x00); // set DS3231 register pointer to 00h
    I2C0.endTransmission(false);
    I2C0.requestFrom(addrI2C_RTC, 7);
    timeNow.second = bcdToDec(I2C0.read() & 0x7f);
    timeNow.minute = bcdToDec(I2C0.read());
    timeNow.hour = bcdToDec(I2C0.read() & 0x3f);
    timeNow.dayW = bcdToDec(I2C0.read());
    timeNow.dayM = bcdToDec(I2C0.read());
    timeNow.month = bcdToDec(I2C0.read());
    timeNow.year = bcdToDec(I2C0.read());

    // Get temperature
    I2C0.beginTransmission(addrI2C_RTC);
    I2C0.write(0x11); // set DS3231 register pointer to 11h
    I2C0.endTransmission(false);
    I2C0.requestFrom(addrI2C_RTC, 1);
    vehicleInfo.boardTemperature = I2C0.read(); // Get int part of the temperature, the decimal part is not important
}

// Put the new time to be applied in the structure timeToUpdate, then put the variable flagTimeToUpdate to true to indicates that there is an update scheduled
void setTime(uint8_t second, uint8_t minute, uint8_t hour, uint8_t dayW, uint8_t dayM, uint8_t month, uint8_t year)
{
    timeToUpdate.second = second;
    timeToUpdate.minute = minute;
    timeToUpdate.hour = hour;
    timeToUpdate.dayW = dayW;
    timeToUpdate.dayM = dayM;
    timeToUpdate.month = month;
    timeToUpdate.year = year;
    flagTimeToUpdate = true;
}

// Display actual time in the serial port
void displayTime()
{
    char timeNowString[50];
    String dayWNames[] = {"...", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

    // Format the time structure
    snprintf(timeNowString, sizeof(timeNowString), "%02u:%02u:%02u: %02u/%02u/%02u",
             timeNow.hour, timeNow.minute, timeNow.second, timeNow.dayM, timeNow.month, timeNow.year);
    Serial.println(timeNowString);
}