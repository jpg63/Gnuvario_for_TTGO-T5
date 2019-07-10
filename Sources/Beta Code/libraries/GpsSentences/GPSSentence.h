/* GPSSentences -- Generate some standard GPS sentences 
 *
 * Copyright 2016-2019 Baptiste PELLEGRIN
 * 
 * This file is part of GNUVario.
 *
 * GNUVario is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GNUVario is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef GPS_SENTENCE_H
#define GPS_SENTENCE_H

#include <Arduino.h>
#include <DebugConfig.h>
#include <HardwareConfig.h>
#include <IGCSentence.h>
#include <NmeaParser.h>
#include <sdcardHAL.h>
#include <kalmanvert.h>


/*!!! the first character of begin() is part of the sentence !!!*/
class GPSSentence {

 public:
  uint8_t begin(double baroAlti);
	void writePosition(kalmanvert kalmanvert);
	void writeGGA(void);
	uint8_t* CreateIgcFile();

 private:

};
  
#ifdef HAVE_SDCARD
	
#define SDCARD_STATE_INITIAL 0
#define SDCARD_STATE_INITIALIZED 1
#define SDCARD_STATE_READY 2
#define SDCARD_STATE_ERROR -1

extern int8_t sdcardState;
extern File fileIgc;
extern IGCHeader   header;
extern IGCSentence igc;
extern GPSSentence igcSD;
#endif //HAVE_SDCARD	

/***************/
/* gps objects */
/***************/
#ifdef HAVE_GPS

extern NmeaParser nmeaParser;
#endif //HAVE_GPS

#endif
