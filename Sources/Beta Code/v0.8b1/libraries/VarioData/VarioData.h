/* VarioData -- 
 *
 * Copyright 2020 MichelPa / Jpg63
 * 
 * This file is part of GnuVario-E.
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

/* 
 *********************************************************************************
 *                                                                               *
 *                           VarioData                                           *
 *                                                                               *
 *  version    Date     Description                                              *
 *    1.0    22/03/20                                                            *
 *                                                                               *
 *********************************************************************************
 */
 
#ifndef VARIO_DATA_H
#define VARIO_DATA_H

#include <HardwareConfig.h>
#include <kalmanvert.h>
#include "VarioDataTwoWireScheduler.h"
#include <IntTW.h>
#include <ms5611TW.h>
#include <vertaccel.h>
#include <LightInvensense.h>
#include <TwoWireScheduler.h>
#include <sdcardHAL.h>
//#include <GPSSentence.h>

/*********************/
/*  AGL              */
/*********************/

#if defined(HAVE_SDCARD) && defined(HAVE_GPS)
#include <AglManager.h>
#endif //HAVE_SDCARD && HAVE_GPS

#include <FlightHistory.h>
#include <variostat.h>

#ifdef HAVE_GPS
		constexpr double historyGPSPeriodCountF = (double)(GPS_PERIOD) / 500.0;
		constexpr int8_t historyGPSPeriodCount = (int8_t)(0.5 + historyGPSPeriodCountF);
#endif

class VarioData
{
public:
    VarioData();
    void init(uint8_t version, uint8_t sub_version, uint8_t beta_code, String devname);
		void intKalman(double firstAlti);
		bool initSettings(bool Test_SD); 
		bool initLog(void); 
		bool initAGL(void);
		void initTime(void);

    void getData();
    double getVelocity();
    double getCalibratedPosition();
		int compteurGpsFix = 0;
		double gpsAlti = 0;
		long MaxVoltage = 0;
		unsigned long lastDisplayTimestamp, lastDisplayTimestamp2;

    boolean displayLowUpdateState = true;
    boolean displayUpdateState = true;

    VarioStat flystat;

    Kalmanvert kalmanvert;

/*********************/
/*  AGL              */
/*********************/

#ifdef AGL_MANAGER_H
AglManager aglManager;
#endif

/************************************/
/* glide ratio / average climb rate */
/************************************/

/* two minutes history */
#ifdef HAVE_GPS
		SpeedFlightHistory<500, 120, historyGPSPeriodCount> history;
#else
		FlightHistory<500, 120> history;
#endif

private:
 //   const float POSITION_MEASURE_STANDARD_DEVIATION = 0.1;
		uint8_t Version;
		uint8_t Sub_Version;
		uint8_t Beta_Code;
		String  Devname;

/*#ifdef HAVE_ACCELEROMETER
    const float ACCELERATION_MEASURE_STANDARD_DEVIATION = 0.3;
#else
    const float ACCELERATION_MEASURE_STANDARD_DEVIATION = 0.6;
#endif //HAVE_ACCELEROMETER*/

#ifdef TWOWIRESCHEDULER
    /* init kalman filter with 0.0 accel*/
    double firstAlti; // = twScheduler.getAlti();
    VarioDataTwoWireScheduler dataClass;
#else //TWOWIRESCHEDULER
    VarioDataNotTwoWireScheduler dataClass;
    double firstAlti; // = ms5611.readPressure();
#endif
    double tmpAlti;
    double tmpTemp;
    double tmpAccel;		
};

//extern Kalmanvert kalmanvert;
extern VarioData varioData;

/**********************/
/* SDCARD objects     */
/**********************/
extern int8_t sdcardState;
//extern bool SD_present;
extern VarioSettings GnuSettings;

/*********************/
/*  AGL              */
/*********************/

/*#ifdef AGL_MANAGER_H
extern AglManager aglManager;
#endif*/

#endif //VARIO_DATA_H