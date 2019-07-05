#ifndef _HARDWARECONFIG_H_
#define _HARDWARECONFIG_H_

#include <HardwareConfigPRO.h>
#include <HardwareConfigESP32.h>
#include <HardwareConfigMK0.h>

/* HardwareConfig -- 
 *
 * Copyright 2019 Jean-philippe GOI
 * 
 * .
 *
 * ToneHAL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ToneHAL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*********************************************************************************/
/*                                                                               */
/*                           HardwareConfig                                      */
/*                                                                               */
/*  version    Date     Description                                              */
/*    1.0    03/06/19                                                            */
/*    1.0.1  24/06/19   Ajout profil GPS - choix entre 2 versions 							 */
/*                                                                               */
/*********************************************************************************/



/***********************/
/* The voltage divisor */
/***********************/

#define VOLTAGE_DIVISOR_VALUE 1.27
#define VOLTAGE_DIVISOR_REF_VOLTAGE 3.3

/*****************************/
/*  IGC HEADER EEPROM        */
/*****************************/

#define IGC_SENTENCE_HEADER_EEPROM_ADDRESS 0x30 
#define IGC_SENTENCE_HEADER_MAX_SIZE (0x200 - 0x30)
#define IGC_SENTENCE_EEPROM_TAG 2568

/*****************************/
/*  EEPROM SOUND             */
/*****************************/

/* eeprom sound setting adresses */
#define SOUND_EEPROM_ADDR 0x210
#define SOUND_EEPROM_TAG 9806

/*****************************/
/*  EEPROM STAT              */
/*****************************/
#define FLY_STAT_HEADER_EEPROM_ADDRESS 0x230 
#define FLY_STAT_EEPROM_TAG 2068

/*****************************/
/* Baro parameters           */
/*****************************/

/* You can set the calibration coefficients if known */
#define MS5611_STATIC_ADDRESS 0x77
//#define MS5611_STATIC_CALIBRATION {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}

/* We advice using the ms5611 barometer */
/* but if you want to use the BMP280 you can enable it here */
//#define HAVE_BMP280
#define BMP280_STATIC_ADDRESS 0x76
//#define BMP280_STATIC_CALIBRATION {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}

/**********************/
/*  MPU parameters    */
/**********************/

/* If you embed an accelerometer set the model here. */
/* Possible values are :                             */
/*   MPU6050, MPU6500, MPU9150, MPU9250              */
#define MPU9250
#define MPU_STATIC_ADDRESS 0x68

/*#define VERTACCEL_GYRO_CAL_EEPROM_ADDR 0x00
#define VERTACCEL_GYRO_CAL_EEPROM_TAG 0xf4e2
#define VERTACCEL_ACCEL_CAL_EEPROM_ADDR 0x0C
#define VERTACCEL_ACCEL_CAL_EEPROM_TAG 0xee54
#define VERTACCEL_MAG_CAL_EEPROM_ADDR 0x14
#define VERTACCEL_MAG_CAL_EEPROM_TAG 0x49f2
#define VERTACCEL_MAG_PROJ_EEPROM_ADDR 0x1C
#define VERTACCEL_MAG_PROJ_EEPROM_TAG 0x67fa*/

#define VERTACCEL_STATIC_CALIBRATION

#define VERTACCEL_GYRO_CAL_BIAS {0x00, 0x00, 0x29, 0xdf, 0xff, 0xff, 0x9f, 0x4a, 0xff, 0xff, 0xb5, 0xe9}
#define VERTACCEL_ACCEL_CAL_BIAS {-3416, 9992, -31522}
#define VERTACCEL_ACCEL_CAL_SCALE -165
#define VERTACCEL_MAG_CAL_BIAS {-5097, 4319, 22227}
#define VERTACCEL_MAG_CAL_PROJ_SCALE 9247
#define VERTACCEL_ACCEL_CAL_BIAS_MULTIPLIER 7
#define VERTACCEL_MAG_CAL_BIAS_MULTIPLIER 5

/* Display integrated climb rate or instantaneous values if disabled     */
/* If enabled set the integration time in ms.                            */
/* ! Climb rate integration time must not be more than glide ratio one ! */
//#define VARIOMETER_DISPLAY_INTEGRATED_CLIMB_RATE
#define VARIOMETER_CLIMB_RATE_INTEGRATION_TIME 2000
#define VARIOMETER_INTEGRATED_CLIMB_RATE_DISPLAY_FREQ 2.0

/* Glide ratio display parameters  */
/* Integration time in ms.         */
#define VARIOMETER_GLIDE_RATIO_INTEGRATION_TIME 15000


/* Set the GPS precision needed to use the GPS altitude value  */
/* to calibrate the barometric altitude.                       */
/*      !!! the best possible precision is 100 !!!             */ 
#define VARIOMETER_GPS_ALTI_CALIBRATION_PRECISION_THRESHOLD 350


/*****************************/
/* SDCard/Bluetooth behavior */
/*****************************/

/* What type of barometric altitude is sent :           */
/* -> Based on international standard atmosphere        */
/* -> Calibrated with GPS altitude                      */
//#define VARIOMETER_SDCARD_SEND_CALIBRATED_ALTITUDE
//#define VARIOMETER_BLUETOOTH_SEND_CALIBRATED_ALTITUDE

/* What type of vario NMEA sentence is sent by bluetooth. */
/* Possible values are :                                  */
/*  - VARIOMETER_SENT_LXNAV_SENTENCE                      */
/*  - VARIOMETER_SENT_LK8000_SENTENCE                     */
#define VARIOMETER_SENT_LXNAV_SENTENCE

/* When there is no GPS to sync variometer bluetooth sentences */
/* set the delay between sendings in milliseconds.             */ 
#define VARIOMETER_SENTENCE_DELAY 2000

/*****************************/
/*           SDCard          */
/*****************************/

#define LF16_FILE_NAME_NUMBER_SIZE 2
#define LF16_FILE_NAME_NUMBER_LIMIT 100


/******************************/
/*             GPS            */
/******************************/

/* The GPS period in ms                             */
/* use the gps-time-analysis sketch to determine it */
#define GPS_PERIOD 997.5  //default 1000

/* The GPS RMC and GGA sentences parameters */
/* Check the SD card ouput or bluetooth output of gps-time-analysis */
#define NMEA_TAG_SIZE 5


// GPS MODEL
#define ATGM336H
//#define NEO_6M

#if defined(ATGM336H)
//ATGM336H

#define NMEA_RMC_TAG "GNRMC"
#define NMEA_GGA_TAG "GNGGA"

/* precision = 10^(number of digit after dot in gps ouput) */
/* check the gps-time-analysis output to check the precision */
/* RMC speed is at 8th position */
/* GGA time is at 2th position */
/* GGA Alti is at 10th position */
/* be carefull, time precision is an int */
#define NMEA_RMC_SPEED_PRECISION 100.0
#define NMEA_GGA_TIME_PRECISION 1000
#define NMEA_GGA_ALTI_PRECISION 10.0

#else if defined(NEO_6M)

//NEO-6M
#define NMEA_RMC_TAG "GPRMC"
#define NMEA_GGA_TAG "GPGGA"

#define NMEA_RMC_SPEED_PRECISION 1000.0
#define NMEA_GGA_TIME_PRECISION 100
#define NMEA_GGA_ALTI_PRECISION 10.0

#endif 

/*----------------------------*/
/*          HARDWARE          */
/*      Vario parameters      */
/*                            */
/*----------------------------*/

/* Comment or uncomment according to  */
/* what you embed in the variometer   */ 
#define HAVE_SPEAKER
#define HAVE_ACCELEROMETER
#define HAVE_SCREEN
#define HAVE_GPS
#define HAVE_SDCARD
//#define HAVE_BLUETOOTH
#define HAVE_VOLTAGE_DIVISOR
//#define HAVE_AUDIO_AMPLI	
#define HAVE_POWER_ALIM	
#define HAVE_BUTTON

#endif
