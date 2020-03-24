/* VarioTwoWire -- 
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
 *                          VarioTwoWire                                         *
 *                                                                               *
 *  version    Date     Description                                              *
 *    1.0    22/03/20                                                            *
 *                                                                               *
 *********************************************************************************
 */
 
#include <Arduino.h> 
#include <HardwareConfig.h>
#include "VarioImuTwoWire.h"

#ifdef TWOWIRESCHEDULER


#ifdef HAVE_BMP280
Bmp280 TWScheduler::bmp280;
#else
Ms5611 TWScheduler::ms5611;
#endif

#ifdef HAVE_ACCELEROMETER
Vertaccel TWScheduler::vertaccel;
#endif //HAVE_ACCELEROMETER


//**********************************
VarioImuTwoWire::VarioImuTwoWire()
//**********************************
{
}

//**********************************
void VarioImuTwoWire::init()
//**********************************
{
    /**************************/
    /* init Two Wires devices */
    /**************************/
    //!!!
#ifdef HAVE_ACCELEROMETER
    intTW.begin();
    twScheduler.init();
    //  vertaccel.init();

#endif //HAVE_ACCELEROMETER
}

//**********************************
double VarioImuTwoWire::getAlti()
//**********************************
{
    return twScheduler.getAlti();
}

//**********************************
bool VarioImuTwoWire::havePressure(void)
//**********************************
{
	return(twScheduler.havePressure());
}

#endif