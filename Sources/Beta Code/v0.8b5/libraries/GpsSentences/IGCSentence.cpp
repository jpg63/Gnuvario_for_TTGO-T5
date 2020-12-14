/* GPSSentences -- Generate some standard GPS sentences 
 *
 * Copyright 2016-2019 Baptiste PELLEGRIN
 *           2020      Jean-philippe GOI
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

/* 
 *********************************************************************************
 *                                                                               *
 *                           IGCSENTENCE                                         *
 *                                                                               *
 *  version    Date     Description                                              *
 *    1.0                                                                        *
 *    1.1      25/11/20   modification format date IGC                           *
 *                                                                               *
 *********************************************************************************
 */

#include <IGCSentence.h>

#include <Arduino.h>
#include <DebugConfig.h>
#include <eepromHAL.h>

/**************/
/* IGC header */
/**************/

const char IGCHeader00[] PROGMEM = "AXXX ";
// vario model name
//const char IGCHeader02[] PROGMEM = "\r\nHFDTE";
//const char IGCHeader03[] PROGMEM = "010100";
const char IGCHeader02[] PROGMEM = "\r\nHFDTEDATE:";
const char IGCHeader03[] PROGMEM = "010100,00";
const char IGCHeader04[] PROGMEM = "\r\nHFPLTPILOTINCHARGE: ";
//pilot name
const char IGCHeader06[] PROGMEM = "\r\nHFGTYGLIDERTYPE: ";
//glider type
const char IGCHeader08[] PROGMEM = "\r\nHFDTM100GPSDATUM: WGS-1984";
const char IGCHeader09[] PROGMEM = "\r\nHFFTYFRTYPE: ";
// vario model name
const char IGCHeader11[] PROGMEM = "\r\n";

const char* headerStrings[] = {	IGCHeader00,
																NULL,
																IGCHeader02,
																IGCHeader03,
																IGCHeader04,
																NULL,
																IGCHeader06,
																NULL,
																IGCHeader08,
																IGCHeader09,
																NULL,
																IGCHeader11};



static void EEPROMUpdate(int address, uint8_t value) {

  if( EEPROMHAL.read(address) != value ) {
    EEPROMHAL.write(address, value);
  }
}
  

bool IGCHeader::saveParams(const char* model, const char* pilot, const char* glider) {

  /******************************/
  /* build and check the header */
  /******************************/
  
  /* build the list of string */
	
#ifdef IGC_DEBUG
  SerialPort.print("model : ");
  SerialPort.print(model);
  SerialPort.print(" - ");
  SerialPort.println(strlen(model));
#endif //PROG_DEBUG

	dataModelName = (char *)malloc(strlen(model)+1);
	strcpy(dataModelName, model);

  headerStrings[1] = dataModelName;
	
#ifdef IGC_DEBUG
  SerialPort.print("dataModelName : ");
  SerialPort.print(dataModelName);
  SerialPort.print(" - ");
  SerialPort.println(strlen(dataModelName));
#endif //PROG_DEBUG
		
#ifdef IGC_DEBUG
  SerialPort.print("pilot : ");
  SerialPort.print(pilot);
  SerialPort.print(" - ");
  SerialPort.println(strlen(pilot));
#endif //PROG_DEBUG

	dataPilotName = (char *)malloc(strlen(pilot)+1);
	strcpy(dataPilotName, pilot);
		
  headerStrings[5] = dataPilotName;

#ifdef IGC_DEBUG
  SerialPort.print("dataPilotName : ");
  SerialPort.print(dataPilotName);
  SerialPort.print(" - ");
  SerialPort.println(strlen(dataPilotName));
#endif //PROG_DEBUG

#ifdef IGC_DEBUG
  SerialPort.print("glider : ");
  SerialPort.print(glider);
  SerialPort.print(" - ");
  SerialPort.println(strlen(glider));
#endif //PROG_DEBUG

//	char* dataGliderName;
	dataGliderName = (char *)malloc(strlen(glider)+1);
	strcpy(dataGliderName, glider);

  headerStrings[7] = dataGliderName;
	
#ifdef IGC_DEBUG
  SerialPort.print("dataGliderName : ");
  SerialPort.print(dataGliderName);
  SerialPort.print(" - ");
  SerialPort.println(strlen(dataGliderName));
#endif //PROG_DEBUG
	
  headerStrings[10] = model;

  /* compute the address of date */
  uint16_t datePos;
  
  uint16_t pos = 0;
  uint16_t stringPos = 0;
  uint16_t stringIdx = 0;

  while( stringIdx < 3 ) {

    /* next string ? */
    if( pgm_read_byte_near(headerStrings[stringIdx] + stringPos) == '\0' ) {
      stringIdx++;
      stringPos = 0;
    }

    /* else count */
    else {
      pos++;
      stringPos++;
    }
  }

  datePos = pos;

  /* compute total size */
  uint16_t totalSize;
  
  while( stringIdx < HEADER_STRING_COUNT ) {
    
    /* next string ? */
    if( pgm_read_byte_near(headerStrings[stringIdx] + stringPos) == '\0' ) {
      stringIdx++;
      stringPos = 0;
    }

    /* else count */
    else {
      pos++;
      stringPos++;
    }
  }

  totalSize = pos;
  pos += 6; //tag + save totalSize + save datePos 
  if( pos > IGC_SENTENCE_HEADER_MAX_SIZE ) {
    return false;
  }

  /******************/
  /* save to EEPROM */
  /******************/
  int eepromAddress = IGC_SENTENCE_HEADER_EEPROM_ADDRESS;

  uint16_t val[3];
  uint8_t* valBuffer = (uint8_t*)((void*)val);

  /* save needed values */
  val[0] = IGC_SENTENCE_EEPROM_TAG;
	
#ifdef IGC_DEBUG
  SerialPort.print("taille : ");
  SerialPort.println(totalSize);
#endif //IGC_DEBUG

  val[1] = totalSize;
	
#ifdef IGC_DEBUG
  SerialPort.print("datePös : ");
  SerialPort.println(datePos);
#endif //IGC_DEBUG
	
  val[2] = datePos;

  /* save to eeprom */
  for( int i = 0; i<6; i++) {
    EEPROMUpdate(eepromAddress, valBuffer[i]);
    eepromAddress++;
  }
  
  /* write the header */
  stringPos = 0;
  stringIdx = 0;
  
  while( stringIdx < HEADER_STRING_COUNT ) {

    uint8_t c = pgm_read_byte_near(headerStrings[stringIdx] + stringPos);
    
    /* next string ? */
    if( c == '\0' ) {
      stringIdx++;
      stringPos = 0;
      
    }

    /* else save char */
    else {
      EEPROMUpdate(eepromAddress, c);
      eepromAddress++;
      stringPos++;
    }
  }

  return true;
}


int16_t IGCHeader::begin(void) {

  /* read the three value */
  addr = IGC_SENTENCE_HEADER_EEPROM_ADDRESS;
  uint16_t val[3];
  uint8_t* valBuffer = (uint8_t*)((void*)val);

  for( int i=0; i<6; i++) {
    valBuffer[i] = EEPROMHAL.read(addr);
    addr++;
  }

  /* check the tag */
  if( val[0] != IGC_SENTENCE_EEPROM_TAG ) {
    return -1;
  }

  /* save the size of the header */
  size = val[1];

  /* return the position of date */
  return val[2];
}

bool IGCHeader::available(void) {

  return (bool)size;
}


uint8_t IGCHeader::get(void) {

#ifdef IGC_DEBUG
  SerialPort.print("IGCHeader get - addr : ");
  SerialPort.println(addr);
#endif //IGC_DEBUG

  uint8_t c = EEPROMHAL.read(addr);
  addr++;
  size--;

#ifdef IGC_DEBUG
  SerialPort.print("IGCHeader get - char : ");
  SerialPort.println(char(c));
#endif //IGC_DEBUG

  return c;
}
  
    
 


/********************/
/* IGC sentence "B" */
/********************/
uint8_t IGCSentence::begin(double baroAlti) {

  /* clear variables */
  outc = 0;
  commaCount = 1;  //TAG not feeded
  digitCount = IGC_SENTENCE_TIME_SIZE; //we start immediately with time

  /* no negative alti */
  uint16_t intAlti;
  if( baroAlti >= 0.0 ) {
    intAlti = (uint16_t)baroAlti;
  } else {
    intAlti = 0;
  }

#ifdef IGC_DEBUG
  SerialPort.print("IGCSentence begin : ");
#endif //IGC_DEBUG

  /* fill in reverse order */
  for( int i = 0; i<IGC_SENTENCE_ALTI_SIZE; i++ ) {
    digitBuffer[i] = '0' + (intAlti % 10);
    intAlti /= 10;
#ifdef IGC_DEBUG
  SerialPort.print(digitBuffer[i]);
#endif //IGC_DEBUG

  }

#ifdef IGC_DEBUG
  SerialPort.println();
#endif //IGC_DEBUG

  return 'B';
}

void IGCSentence::feed(uint8_t c) {

  outc = 0;
  
  /* when receive commma, reinit vars */
  if( c == ',' ) {
    commaCount++;
        
    if( commaCount == 2 ) {
      digitCount = IGC_SENTENCE_LAT_SIZE; //latitude
    }

    else if( commaCount == 3 || commaCount == 5 ) {
      digitCount = IGC_SENTENCE_CARDINAL_SIZE; //N,S,E,O
    }

    else if( commaCount == 4 ) {
      digitCount = IGC_SENTENCE_LONG_SIZE; //longitude
    }

    else if( commaCount == 7 ) {
      outc = 'A'; //we have alti
    }

    else if( commaCount == 8 || commaCount == 10 ) {
      outc = digitBuffer[IGC_SENTENCE_ALTI_SIZE - 1]; //first alti digit
      digitCount = IGC_SENTENCE_ALTI_SIZE - 2;  //next digits (used as pointers)
    }

    else if( commaCount == 9 ) {
      digitCount = IGC_SENTENCE_ALTI_SIZE - 1; //we read gps alti in reverse order
    }

    else if( commaCount == 11 ) {
      outc = '\r';
    }

    return;
  }

  /*********************/
  /* there is two case */
  /*********************/
  
  /* -> we are reading GPS alti */
  if( commaCount == 9 ) {
    if( digitCount < IGC_SENTENCE_ALTI_SIZE ) {
      /* if dot was not found get the digit */
      if( c != '.' ) {
				digitBuffer[digitCount] = c;
				digitCount--;
      }

      /* build alti digits */
      else {
				uint8_t i = 0;
	
				/* translate digits */
				digitCount++;
				while( digitCount < IGC_SENTENCE_ALTI_SIZE ) {
					digitBuffer[i] = digitBuffer[digitCount];
					i++;
					digitCount++;
				}

				/* fill with 0 */
				while( i < IGC_SENTENCE_ALTI_SIZE ) {
					digitBuffer[i] = '0';
					i++;
				}
      }
    }
  }
  
  /* -> we are outputting digits */
  else {
    if( commaCount < 8 && c != '.' && digitCount ) {
			digitCount--;
			outc = c;
    }
  }
}
 
bool IGCSentence::available() {

  return (bool)outc;
}

uint8_t IGCSentence::get() {

  /* get last char */
  uint8_t retChar = outc;
  outc = 0;

  /* check if we need to send next char immediately  */
  if( digitCount >= 0 ) {
    if( commaCount == 8 || commaCount == 10 ) {  //we outputing alti 
      outc = digitBuffer[digitCount];
      digitCount--;
    }
  }

  else {
    digitCount = 0; //don't stay with digitCount = -1 
  }

  /* new line */
  if( retChar == '\r' ) {
    outc = '\n';
  }

  /* send */
  return retChar;
}

    

  
