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

#include "VarioData.h"
#include "VarioDataTwoWireScheduler.h"
#include <HardwareConfig.h>
#include <DebugConfig.h>

#ifdef DATA_DEBUG
#define ARDUINOTRACE_ENABLE 1
#else
#define ARDUINOTRACE_ENABLE 0
#endif

#define ARDUINOTRACE_SERIAL SerialPort
#include <ArduinoTrace.h>

#include "VarioHardwareManager.h"

#include <Utility.h>

#include <sdcardHAL.h>
#include <VarioSettings.h>

#include <VarioLog.h>

#ifdef HAVE_WIFI
#include <VarioWifiServer.h>
#endif //HAVE_WIFI

#include <GPSSentence.h>

#if defined(HAVE_SDCARD) && defined(HAVE_GPS)
#include <AglManager.h>
#endif //HAVE_SDCARD && HAVE_GPS

/**********************/
/* SDCARD objects     */
/**********************/

int8_t sdcardState = SDCARD_STATE_INITIAL;

VarioSettings 	GnuSettings;
//Kalmanvert 			kalmanvert;

//****************************************************************************************************************************
VarioData::VarioData()
//****************************************************************************************************************************
{
}

void VarioData::init(uint8_t version, uint8_t sub_version, uint8_t beta_code, String devname)
{
	
	Version 			= version;
	Sub_Version		= sub_version;
	Beta_Code			= beta_code;
	Devname				= devname;
#ifdef PROG_DEBUG
    ///  while (!SerialPort) { ;}
    char tmpbuffer[100];
    sprintf(tmpbuffer, "GNUVARIO compiled on %s", __DATE__); // at %s", __DATE__, __TIME__);
    SerialPort.println(tmpbuffer);
    sprintf(tmpbuffer, "VERSION %i.%i - %s", Version, Sub_Version, Devname);
    SerialPort.println(tmpbuffer);
    if (Beta_Code > 0)
    {
        SerialPort.print("Beta ");
        SerialPort.println(Beta_Code);
    }
    SerialPort.flush();
#endif //PRO_DEBBUG

#if defined(ESP32)
    if (Beta_Code > 0)
    {
        ESP_LOGI("GNUVARIO", "GnuVario-E version %d.%d Beta %d.", Version, Sub_Version, Beta_Code);
    }
    else
    {
        ESP_LOGI("GNUVARIO", "GnuVario-E version %d.%d.", Version, Sub_Version);
        //  ESP_LOGE(TAG, "Failed to initialize the card (%d). Make sure SD card lines have pull-up resistors in place.", ret);
    }
#endif //ESP32
}

//*******************************************
void VarioData::intKalman(double firstAlti)
//*******************************************
{
    kalmanvert.init(firstAlti,
                    0.0,
                    POSITION_MEASURE_STANDARD_DEVIATION,
                    ACCELERATION_MEASURE_STANDARD_DEVIATION,
                    millis());
										
#ifdef KALMAN_DEBUG
  SerialPort.println("kalman init");
#endif //KALMAN_DEBUG

  TRACE();
  SDUMP("Test INT MPU");
}


//*******************************************
void VarioData::getData()
//*******************************************
{
    dataClass.getData(tmpAlti, tmpTemp, tmpAccel);

#ifdef DATA_DEBUG
    SerialPort.print("Alti : ");
    SerialPort.println(tmpAlti);
    SerialPort.print("Temperature : ");
    SerialPort.println(tmpTemp);
    SerialPort.print("Accel : ");
    SerialPort.println(tmpAccel);
#endif //DATA_DEBUG

    kalmanvert.update(tmpAlti,
                      tmpAccel,
                      millis());
}

//*******************************************
double VarioData::getVelocity()
//*******************************************
{
    return kalmanvert.getVelocity();
}

//*******************************************
double VarioData::getCalibratedPosition(){
//*******************************************
     return kalmanvert.getCalibratedPosition();
}


//*******************************************
bool VarioData::initSettings(bool Test_SD) 
//*******************************************
{
#ifdef HAVE_SDCARD
	
	if (GnuSettings.initSettings(Test_SD))
  {

#ifdef SDCARD_DEBUG
    SerialPort.println("initialization done.");
    SerialPort.flush();
#endif //SDCARD_DEBUG

#if defined(ESP32)
    ESP_LOGI("SDCARD", "initialization done.");
#endif //EPS32

#ifdef HAVE_WIFI
    SD_present = true;
#endif //HAVE_WIFI
    sdcardState = SDCARD_STATE_INITIALIZED;
    /*    char FileName[15] = "SETTINGS.TXT";
    GnuSettings.readSDSettings(FileName);*/

    GnuSettings.setVersion(Version, Sub_Version, Beta_Code);

    SerialPort.println("Chargement des parametres depuis le fichier params.jso");
    char tmpchar[20] = "params.jso";
    GnuSettings.loadConfigurationVario(tmpchar);

#ifdef SDCARD_DEBUG
    //Debuuging Printing
    SerialPort.print("Pilot Name = ");
    SerialPort.println(GnuSettings.VARIOMETER_PILOT_NAME);
#endif //SDCARD_DEBUG

#ifdef PROG_DEBUG
    //Debuuging Printing
    SerialPort.print("Pilot Name = ");
    SerialPort.println(GnuSettings.VARIOMETER_PILOT_NAME);
#endif //PROG_DEBUG

    char __dataPilotName[GnuSettings.VARIOMETER_PILOT_NAME.length()];
    GnuSettings.VARIOMETER_PILOT_NAME.toCharArray(__dataPilotName, sizeof(__dataPilotName) + 1);

#ifdef PROG_DEBUG
    //Debuuging Printing
    SerialPort.print("__dataPilotName = ");
    SerialPort.print(__dataPilotName);
    SerialPort.print(" - ");
    int tmpint = sizeof(__dataPilotName);
    SerialPort.print(tmpint);
    SerialPort.print(" / ");
    SerialPort.print(GnuSettings.VARIOMETER_PILOT_NAME);
    SerialPort.print(" - ");
    SerialPort.println(GnuSettings.VARIOMETER_PILOT_NAME.length());
#endif //PROG_DEBUG

    char __dataGliderName[GnuSettings.VARIOMETER_GLIDER_NAME.length()];
    GnuSettings.VARIOMETER_GLIDER_NAME.toCharArray(__dataGliderName, sizeof(__dataGliderName) + 1);

#ifdef PROG_DEBUG
    //Debuuging Printing
    SerialPort.print("__dataGliderName = ");
    SerialPort.print(__dataGliderName);
    SerialPort.print(" - ");
    tmpint = sizeof(__dataGliderName);
    SerialPort.print(tmpint);
    SerialPort.print(" / ");
    SerialPort.print(GnuSettings.VARIOMETER_GLIDER_NAME);
    SerialPort.print(" - ");
    SerialPort.println(GnuSettings.VARIOMETER_GLIDER_NAME.length());
#endif //PROG_DEBUG

    header.saveParams(VARIOMETER_MODEL_NAME, __dataPilotName, __dataGliderName);

    boolean ModifValue = false;
    char tmpFileName[15] = "wifi.cfg";

    if (SDHAL_SD.exists(tmpFileName))
    {
      GnuSettings.readSDSettings(tmpFileName, &ModifValue);

      SerialPort.println("");
      SerialPort.println("Lecture du fichier wifi.cfg");

      SerialPort.print("Wifi SSID 1 : ");
      SerialPort.println(GnuSettings.VARIOMETER_SSID_1);

      SerialPort.print("Wifi Password 1 : ");
      SerialPort.println(GnuSettings.VARIOMETER_PASSWORD_1);

      SerialPort.print("Wifi SSID 2 : ");
      SerialPort.println(GnuSettings.VARIOMETER_SSID_2);

      SerialPort.print("Wifi Password 2 : ");
      SerialPort.println(GnuSettings.VARIOMETER_PASSWORD_2);

      SerialPort.print("Wifi SSID 3 : ");
      SerialPort.println(GnuSettings.VARIOMETER_SSID_3);

      SerialPort.print("Wifi Password 3 : ");
      SerialPort.println(GnuSettings.VARIOMETER_PASSWORD_3);

      SerialPort.print("Wifi SSID 4 : ");
      SerialPort.println(GnuSettings.VARIOMETER_SSID_4);

      SerialPort.print("Wifi Password 4 : ");
      SerialPort.println(GnuSettings.VARIOMETER_PASSWORD_4);
    }

    //lecture parametre de configuration

    strcpy(tmpFileName, "variocal.cfg");

    if (SDHAL_SD.exists(tmpFileName))
    {
      GnuSettings.readSDSettings(tmpFileName, &ModifValue);
    }
		return true;
  }
  else
  {
#ifdef HAVE_WIFI
    SD_present = false;
#endif //HAVE_WIFI

#ifdef SDCARD_DEBUG
    SerialPort.println("initialization failed!");
#endif //SDCARD_DEBUG

#if defined(ESP32)
    ESP_LOGE("SDCARD", "initialization failed!");
#endif //EPS32

#ifdef HAVE_SPEAKER
    if (GnuSettings.ALARM_SDCARD)
    {
      indicateFaultSDCARD();
    }
#endif //HAVE_SPEAKER
		return false;
  }
#else //HAVE_SDCARD
#ifdef HAVE_WIFI
  SD_present = false;
#endif //HAVE_WIFI

#ifdef SDCARD_DEBUG
  SerialPort.println("initialization failed!");
#endif //SDCARD_DEBUG

#if defined(ESP32)
  ESP_LOGE("SDCARD", "initialization failed!");
#endif //EPS32

#ifdef HAVE_SPEAKER
  if (GnuSettings.ALARM_SDCARD)
  {
    indicateFaultSDCARD();
  }
	return false;
#endif //HAVE_SPEAKER
#endif //HAVE_SDCARD
}	

//*******************************************
bool VarioData::initLog(void) 
//*******************************************
{
  varioLog.init();
  String tmpStr;
  tmpStr = "FirmWare : " + String(Version) + "." + String(Sub_Version);
  if (Beta_Code > 0)
    tmpStr = tmpStr + "b" + String(Beta_Code);

  INFOLOG(tmpStr);
  TRACELOG(LOG_TYPE_DEBUG, MAIN_DEBUG_LOG);
}


//***********************************************
// INIT AGL
//***********************************************
//*******************************************
bool VarioData::initAGL(void) 
//*******************************************
{
  aglManager.init();
}

//***********************************************
// INIT TIME
//***********************************************
//*******************************************
void VarioData::initTime(void) 
//*******************************************
{
  lastDisplayTimestamp 									= millis();
  lastDisplayTimestamp2 								= millis();
  varioHardwareManager.time_deep_sleep  = lastDisplayTimestamp;
  varioHardwareManager.sleepTimeoutSecs = lastDisplayTimestamp;
  displayLowUpdateState 								= true;
  displayUpdateState 										= true;
}