#include "VarioImuStd.h"
#include <HardwareConfig.h>
#include <Arduino.h>

#ifndef TWOWIRESCHEDULER

VarioImuStd::VarioImuStd() {}

VarioImuStd::init()
{
#ifdef MS5611_DEBUG
    SerialPort.println("Initialize MS5611 Sensor");
#endif //MS5611_DEBUG

#if defined(VARIO_SDA_PIN) && defined(VARIO_SCL_PIN)
    while (!ms5611.begin(VARIO_SDA_PIN, VARIO_SCL_PIN))
#else
    while (!ms5611.begin())
#endif
    {
        SerialPort.println("Could not find a valid MS5611 sensor, check wiring!");
#if defined(ESP32)
        ESP_LOGE(TAG, "Erreur capteur MS5611 introuvable");
#endif //EPS32
        delay(500);
    }

    // Get reference pressure for relative altitude
    referencePressure = ms5611.readPressure();

    SerialPort.print("Oversampling: ");
    SerialPort.println(ms5611.getOversampling());

    vertaccel.init();

#ifdef HAVE_ACCELEROMETER
#ifdef ACCEL_DEBUG
    SerialPort.println("configuring device.");
#endif //ACCEL_DEBUG

#ifdef ACCEL_DEBUG
    SerialPort.println("configured 9Axis I2C MPU9250");
#endif //ACCEL_DEBUG

    // Call imu.begin() to verify communication and initialize
#if defined(VARIO_SDA_PIN) && defined(VARIO_SCL_PIN)
    if (imu.begin(VARIO_SDA_PIN, VARIO_SCL_PIN) != INV_SUCCESS)
#else
    if (imu.begin() != INV_SUCCESS)
#endif

    {
        while (1)
        {
            SerialPort.println("Unable to communicate with MPU-9250");
            SerialPort.println("device error");
            ESP_LOGE(TAG, "Erreur capteur MPU9250 introuvable");
            while (1)
                ;
        }
    }

    // Use setSensors to turn on or off MPU-9250 sensors.
    // Any of the following defines can be combined:
    // INV_XYZ_GYRO, INV_XYZ_ACCEL, INV_XYZ_COMPASS,
    // INV_X_GYRO, INV_Y_GYRO, or INV_Z_GYRO
    // Enable all sensors:
    imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);

    /*  // Use setGyroFSR() and setAccelFSR() to configure the
  // gyroscope and accelerometer full scale ranges.
  // Gyro options are +/- 250, 500, 1000, or 2000 dps
  imu.setGyroFSR(2000); // Set gyro to 2000 dps
  // Accel options are +/- 2, 4, 8, or 16 g
  imu.setAccelFSR(8); // Set accel to +/-2g
  // Note: the MPU-9250's magnetometer FSR is set at 
  // +/- 4912 uT (micro-tesla's)

  // setLPF() can be used to set the digital low-pass filter
  // of the accelerometer and gyroscope.
  // Can be any of the following: 188, 98, 42, 20, 10, 5
  // (values are in Hz).
  imu.setLPF(5); // Set LPF corner frequency to 5Hz*/

    // The sample rate of the accel/gyro can be set using
    // setSampleRate. Acceptable values range from 4Hz to 1kHz
    imu.setSampleRate(100); // Set sample rate to 10Hz

    // Likewise, the compass (magnetometer) sample rate can be
    // set using the setCompassSampleRate() function.
    // This value can range between: 1-100Hz
    imu.setCompassSampleRate(100); // Set mag rate to 10Hz

    /*  imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT | // Enable 6-axis quat
               DMP_FEATURE_GYRO_CAL, // Use gyro calibration
              10); // Set DMP FIFO rate to 10 Hz
  // DMP_FEATURE_LP_QUAT can also be used. It uses the 
  // accelerometer in low-power mode to estimate quat's.
  // DMP_FEATURE_LP_QUAT and 6X_LP_QUAT are mutually exclusive*/

    imu.dmpBegin(DMP_FEATURE_SEND_RAW_ACCEL |    // Send accelerometer data
                                                 //                 DMP_FEATURE_SEND_RAW_GYRO  | // Send raw gyroscope values to FIFO
                     DMP_FEATURE_GYRO_CAL |      // Calibrate the gyro data
                     DMP_FEATURE_SEND_CAL_GYRO | // Send calibrated gyro data
                     DMP_FEATURE_6X_LP_QUAT,     // Calculate quat's with accel/gyro
                 100);                           // Set update rate to 10Hz.

#endif //HAVE_ACCELEROMETER
}

double VarioImuStd::getAlti(){
  return ms5611.readPressure();
}

//**********************************
bool VarioImuTwoWire::havePressure(void)
//**********************************
{
	return(true);
}

#endif