/* VarioHardwareManager -- 
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
 *                           VarioHardwareManager                                *
 *                                                                               *
 *  version    Date     Description                                              *
 *    1.0    22/03/20                                                            *
 *                                                                               *
 *********************************************************************************
 */
 
#include <Arduino.h>

#include <HardwareConfig.h>
#include <DebugConfig.h>

#include "VarioHardwareManager.h"

#include "VarioData.h"

#include <VarioButton.h>

VarioHardwareManager varioHardwareManager;

//**********************************
VarioHardwareManager::VarioHardwareManager()
//**********************************
{
    varioAlim 		= new VarioAlim();
    varioSpeaker 	= new VarioSpeaker();
    varioImu 			= new VarioImu();
		varioGps 			= new VarioGps();
		varioBle 			= new VarioBle();
		

#ifndef HAVE_GPS
		lastVarioSentenceTimestamp = 0;
#endif // !HAVE_GPS		
		
#ifdef HAVE_GPS
		variometerState = VARIOMETER_STATE_INITIAL;
#else
		variometerState = VARIOMETER_STATE_CALIBRATED;
#endif //HAVE_GPS
		
}

void VarioHardwareManager::init()
{

}

//**********************************
void VarioHardwareManager::initAlim()
//**********************************
{
    this->varioAlim->init();
}

//**********************************
void VarioHardwareManager::initSpeaker()
//**********************************
{
    this->varioSpeaker->init();
}

//**********************************
void VarioHardwareManager::initSound()
//**********************************
{
    this->varioSpeaker->initSound();
}

//**********************************
void VarioHardwareManager::initImu()
//**********************************
{
    this->varioImu->init();
}

//**********************************
void VarioHardwareManager::initButton()
//**********************************
{
	#ifdef HAVE_BUTTON
#ifdef BUTTON_DEBUG
  SerialPort.println("initialization bouton");
  SerialPort.flush();
#endif //BUTTON_DEBUG

#if defined(ESP32)
  ESP_LOGI("BUTTON", "initialization button");
#endif //EPS32

  VarioButton.begin();
  ButtonScheduleur.Set_StatePage(STATE_PAGE_INIT);
#endif

}

//**********************************
void VarioHardwareManager::initGps()
//**********************************
{
    this->varioGps->init();
}

//**********************************
bool VarioHardwareManager::initBt()
//**********************************
{
    return(this->varioBle->init());
}

//**********************************
double VarioHardwareManager::getAlti()
//**********************************
{
    return this->varioImu->getAlti();
}

//***********************************
double VarioHardwareManager::firstAlti(void)
//***********************************
{
  return(this->varioImu->firstAlti());
}