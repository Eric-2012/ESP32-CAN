/******************************************************************************
 * task_BusHSPI.cpp
 * This file has the code for the inertial sensor
 *****************************************************************************/

#include "task_BusHSPI.h"

// Variables for communication with the inertial sensor
MPU9250 IMU(HSPIBus, pinHSPI_CS_Acc);
int status;

// Data from the inertial sensor
double accX, accY, accZ;
double gyroX, gyroY, gyroZ;
double magX, magY, magZ;

// Variables used for calculations
Kalman kalmanX, kalmanY, kalmanZ;
double kalAngleX, kalAngleY, kalAngleZ; // Calculated angle using a Kalman filter
double gyroXrate, gyroYrate, gyroZrate;
double roll, pitch, yaw;
double Xh, Yh;
double dt;
uint32_t timer;

// Code for the task that get the data from the inertial sensor and writes to the flash memory
void taskCode_BusHSPI(void *pvParameters)
{
    // Start communication with the inertial sensor
    status = IMU.begin();
    if (status < 0)
    {
        Serial.println("IMU initialization unsuccessful");
        Serial.println("Check IMU wiring or try cycling power");
        Serial.print("Status: ");
        Serial.println(status);
    }

    // Setting the accelerometer full scale range to +/-8G
    IMU.setAccelRange(MPU9250::ACCEL_RANGE_2G);

    // Setting the gyroscope full scale range to +/-500 deg/s
    IMU.setGyroRange(MPU9250::GYRO_RANGE_250DPS);

    // Setting DLPF bandwidth to 20 Hz
    IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_41HZ);

    // Setting SRD to 19 for a 50 Hz update rate
    // IMU.setSrd(19);
    IMU.calibrateMag();

    // Read the sensor
    IMU.readSensor();
    vehicleInfo.accX = accX = IMU.getAccelX_mss();
    vehicleInfo.accY = accY = IMU.getAccelY_mss();
    vehicleInfo.accZ = accZ = IMU.getAccelZ_mss();
    vehicleInfo.gyroX = gyroX = IMU.getGyroX_rads();
    vehicleInfo.gyroY = gyroY = IMU.getGyroY_rads();
    vehicleInfo.gyroZ = gyroZ = IMU.getGyroZ_rads();
    vehicleInfo.magX = magX = IMU.getMagX_uT();
    vehicleInfo.magY = magY = IMU.getMagY_uT();
    vehicleInfo.magZ = magZ = IMU.getMagZ_uT();
    timer = micros(); // Time when the IMU data was read

    // RESTRICT_PITCH
    // http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 25 and eq. 26
    roll = atan2(accY, accZ);
    pitch = atan(-accX / sqrt(accY * accY + accZ * accZ));

    Yh = (magY * cos(roll)) - (magZ * sin(roll));
    Xh = (magX * cos(pitch)) + (magY * sin(roll) * sin(pitch)) + (magZ * cos(roll) * sin(pitch));
    yaw = atan2(Yh, Xh);

    /* RESTRICT_ROLL
    // http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 28 and eq. 29
    // double roll = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
    // double pitch = atan2(-accX, accZ) * RAD_TO_DEG;
    */

    // Set starting angle
    kalmanX.setAngle(roll * RAD_TO_DEG);
    kalmanY.setAngle(pitch * RAD_TO_DEG);
    kalmanZ.setAngle(yaw);

    while (true)
    {
        // Start execution debug
        task_InertialSensor.initDebugExec();

        // Read the sensor
        IMU.readSensor();
        vehicleInfo.accX = accX = IMU.getAccelX_mss();
        vehicleInfo.accY = accY = IMU.getAccelY_mss();
        vehicleInfo.accZ = accZ = IMU.getAccelZ_mss();
        vehicleInfo.gyroX = gyroX = IMU.getGyroX_rads();
        vehicleInfo.gyroY = gyroY = IMU.getGyroY_rads();
        vehicleInfo.gyroZ = gyroZ = IMU.getGyroZ_rads();
        vehicleInfo.magX = magX = IMU.getMagX_uT();
        vehicleInfo.magY = magY = IMU.getMagY_uT();
        vehicleInfo.magZ = magZ = IMU.getMagZ_uT();

        // Calculates elapsed time
        dt = (double)(micros() - timer) / 1000000; // Calculate delta time
        timer = micros();

        // RESTRICT_PITCH
        // http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 25 and eq. 26
        roll = atan2(accY, accZ);
        pitch = atan(-accX / sqrt(accY * accY + accZ * accZ));

        Yh = (magY * cos(roll)) - (magZ * sin(roll));
        Xh = (magX * cos(pitch)) + (magY * sin(roll) * sin(pitch)) + (magZ * cos(roll) * sin(pitch));
        yaw = atan2(Yh, Xh);

        // To the yaw calculation, needs to be radian, so after yaw calc, converts to deg for the rest of things
        roll *= RAD_TO_DEG;
        pitch *= RAD_TO_DEG;

        /* RESTRICT_ROLL
        // http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 28 and eq. 29
        // double roll = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
        // double pitch = atan2(-accX, accZ) * RAD_TO_DEG;
        */

        // Convert to deg/s
        gyroXrate = gyroX / 131.0;
        gyroYrate = gyroY / 131.0;
        gyroZrate = gyroZ / 131.0;

        // RESTRICT_PITCH
        // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
        if ((roll < -90 && kalAngleX > 90) || (roll > 90 && kalAngleX < -90))
        {
            kalmanX.setAngle(roll);
            kalAngleX = roll;
        }
        else
            kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter

        if (abs(kalAngleX) > 90)
            gyroYrate = -gyroYrate; // Invert rate, so it fits the restriced accelerometer reading
        kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt);

        kalAngleZ = kalmanZ.getAngle(yaw, gyroZrate, dt); // Calculate the angle using a Kalman filter

        /* RESTRICT_ROLL
        // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
        // if ((pitch < -90 && kalAngleY > 90) || (pitch > 90 && kalAngleY < -90))
        // {
        //     kalmanY.setAngle(pitch);
        //     kalAngleY = pitch;
        // }
        // else
        //     kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt); // Calculate the angle using a Kalman filter
        // 
        // if (abs(kalAngleY) > 90)
        //     gyroXrate = -gyroXrate;                        // Invert rate, so it fits the restriced accelerometer reading
        // kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter
        */

        // Put in the structure
        vehicleInfo.roll = kalAngleX;
        vehicleInfo.pitch = kalAngleY;

        // End execution debug
        task_InertialSensor.endDebugExec(uxTaskGetStackHighWaterMark(NULL));

        // Put the task to sleep
        vTaskDelay(task_InertialSensor.getInterval() / portTICK_PERIOD_MS);
    }
}
