/* varioStat -- 
 *
 * Copyright 2019 Jean-philippe GOI
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

/*********************************************************************************/
/*                                                                               */
/*                           VarioStat                                           */
/*                                                                               */
/*  version    Date     Description                                              */
/*    1.0      06/07/19                                                          */
/*                                                                               */
/*********************************************************************************/

#include <variostat.h>
#include <Arduino.h>
#include <eepromHAL.h>
#include <toneHAL.h>
#include <DebugConfig.h>

// Write any data structure or variable to EEPROM
static int EEPROMAnythingWrite(int pos, char *character, int length)
{
#ifdef EEPROM_DEBUG	
	SerialPort.print("length : ");
	SerialPort.println(length);
  SerialPort.println("EEPROMAnythingWrite : ");	
	for (int i = 0; i < length; i++) {
		SerialPort.printf("%d",int(character[i]));	
  }
  SerialPort.println("");
#endif //EEPROM_DEBUG
	
  for (int i = 0; i < length; i++)
  {
		char tmpChar = *character;

	  if (EEPROMHAL.read(pos + i) != *character)  {
      EEPROMHAL.write(pos + i, *character);
	  }
    character++;
  }

  EEPROMHAL.commit();
  return pos + length;
}
 
// Read any data structure or variable from EEPROM
static int EEPROMAnythingRead(int pos, char *character, int length)
{
#ifdef EEPROM_DEBUG	
  SerialPort.print("EEPROMAnythingRead : ");	
#endif //EEPROM_DEBUG
	
  for (int i = 0; i < length; i++)
  {
		char tmpchar = EEPROMHAL.read(pos + i);
    *character = tmpchar;
#ifdef EEPROM_DEBUG	
		SerialPort.print(tmpchar);	
#endif //EEPROM_DEBUG
		
    character++;
  }
	
#ifdef EEPROM_DEBUG	
		SerialPort.println("");	
#endif //EEPROM_DEBUG
	
  return pos + length;
}

void VarioStat::Begin(void) {
  Timer        = millis();
  MaxAlti      = 0;
  MaxSpeed     = 0;
  MaxVario     = 0;
  MinVario     = 0;
	firsttime    = true;
	EnableRecord = false;
  //Enable();
}

void VarioStat::Enable() {
  EnableRecord = true;
}

void VarioStat::Disable() {
  EnableRecord = false;
}

void VarioStat::SetAlti(double alti) {
  if (alti > MaxAlti) MaxAlti = alti;
}

void VarioStat::SetSpeed(double speed) {
  if (speed > MaxSpeed) MaxSpeed = speed;
}

void VarioStat::SetVario(double vario) {
  if (vario > MaxVario) MaxVario = vario;
  if (vario < MinVario) MinVario = vario;
}

void VarioStat::SetDuration(int8_t* duree) {
  duree[0] = duree[0];	
  duree[1] = duree[1];	
  duree[2] = duree[2];	
}

void VarioStat::SetTime(int8_t* time) {
	if (firsttime && EnableRecord) {
    time[0] = time[0];	
    time[1] = time[1];	
    time[2] = time[2];	
		firsttime = false;
	}
}

void VarioStat::SetDate(uint8_t* dateValue) {
	for (int i=0; i<8; i++) {
    date[i] = dateValue[i];	
	}
}
   
double VarioStat::GetAlti() {
  return MaxAlti;
}

double VarioStat::GetVarioMin() {
  return MinVario;
}

double VarioStat::GetVarioMax() {
  return MaxVario;
}

double VarioStat::GetSpeed() {
  return MaxSpeed;
}
   
void VarioStat::GetDuration(int8_t* dureeValue) {
  for (int i=0; i<3; i++) {
		if (duree[i] > 99) dureeValue[i] = 0;
		else							dureeValue[i] = duree[i];
	}	
}
  
void VarioStat::GetTime(int8_t* timeValue) {
  for (int i=0; i<3; i++) {
		if (time[i] > 99) timeValue[i] = 0;
		else							timeValue[i] = time[i];
	}	
}

void VarioStat::GetDate(uint8_t* dateValue) {
  for (int i=0; i<3; i++) {
		if (date[i] > 99) dateValue[i] = 0;
		else							dateValue[i] = date[i];
	}
}
	
void VarioStat::Display() {
//  Serial.println("Display");	
  ReadEeprom();
}

void VarioStat::ForceWrite() {
//  Serial.println("Display");	
  WriteEeprom();
}

bool VarioStat::Handle() {
  if ((EnableRecord) && (millis()-Timer) > 10000) {
	  Timer = millis();
	  WriteEeprom();
	  return true;
  }
  return false;
}

void VarioStat::ReadEeprom() {
  int eepromAddress = FLY_STAT_HEADER_EEPROM_ADDRESS;
  int val_int = 0;
   
  // Integer read from EEPROM
//  eepromAddress = EEPROMAnythingRead(eepromAddress, reinterpret_cast<char*>(&val_int), sizeof(val_int));
    eepromAddress = EEPROMHAL.readInt(eepromAddress, &val_int);
  
	
#ifdef EEPROM_DEBUG	
  SerialPort.print("ReadEprom TAG :  ");	
  SerialPort.println(val_int); 
	SerialPort.print("FLY_STAT_EEPROM_TAG : ");
	SerialPort.println(FLY_STAT_EEPROM_TAG);
#endif //EEPROM_DEBUG
 
  if (val_int == FLY_STAT_EEPROM_TAG) {  
	
#ifdef EEPROM_DEBUG	
		SerialPort.println("TAG OK ");	
#endif //EEPROM_DEBUG
	
    // Float read to EEPROM
    float   val_float = 0;
    eepromAddress = EEPROMAnythingRead(eepromAddress, reinterpret_cast<char*>(&val_float), sizeof(val_float));
	  MaxAlti = val_float;

    eepromAddress = EEPROMAnythingRead(eepromAddress, reinterpret_cast<char*>(&val_float), sizeof(val_float));
	  MaxSpeed = val_float;

    eepromAddress = EEPROMAnythingRead(eepromAddress, reinterpret_cast<char*>(&val_float), sizeof(val_float));
	  MinVario = val_float;

    eepromAddress = EEPROMAnythingRead(eepromAddress, reinterpret_cast<char*>(&val_float), sizeof(val_float));
	  MaxVario = val_float;
	
    // Read array from EEPROM
    EEPROMAnythingRead(eepromAddress, reinterpret_cast<char*>(&duree), sizeof(duree));   

    // Read array from EEPROM
    EEPROMAnythingRead(eepromAddress, reinterpret_cast<char*>(&time), sizeof(time));   

    // Read array from EEPROM
    EEPROMAnythingRead(eepromAddress, reinterpret_cast<char*>(&date), sizeof(date));   

  }
  else {
    MaxAlti    = 1200;
    MaxSpeed   = 52;
    MaxVario   = 3.2;
    MinVario   = -1.8;
    duree[0]   = 0;
    duree[1]   = 0;
    duree[2]   = 0;	  
		date[0]    = 0;
		date[1]    = 1;
		date[2]    = 1;
		date[3]    = 80;
		time[0]    = 0;
		time[1]    = 0;
		time[2]    = 0;
  }
}

void VarioStat::WriteEeprom() {
	
	  /******************/
  /* save to EEPROM */
  /******************/
  int eepromAddress = FLY_STAT_HEADER_EEPROM_ADDRESS;

#ifdef EEPROM_DEBUG	
  SerialPort.println("WriteEeprom");	
#endif //EEPROM_DEBUG

  // int to EEPROM
  int val_int = FLY_STAT_EEPROM_TAG;
#ifdef EEPROM_DEBUG	
  SerialPort.print("FLY_STAT_EEPROM_TAG : ");	
	SerialPort.println(val_int);
	SerialPort.print("Siseof : ");
	SerialPort.println(sizeof(val_int));
#endif //EEPROM_DEBUG
	
//  eepromAddress = EEPROMAnythingWrite(eepromAddress, reinterpret_cast<char*>(&val_int), sizeof(val_int)); 
  eepromAddress = EEPROMHAL.writeInt(eepromAddress, val_int);

  // Float to EEPROM
  float   val_float = MaxAlti;
  eepromAddress = EEPROMAnythingWrite(eepromAddress, reinterpret_cast<char*>(&val_float), sizeof(val_float)); 

#ifdef EEPROM_DEBUG	
  SerialPort.print("Max Alti : ");	
	SerialPort.println(val_float);
#endif //EEPROM_DEBUG
 
  val_float = MaxSpeed;
  eepromAddress = EEPROMAnythingWrite(eepromAddress, reinterpret_cast<char*>(&val_float), sizeof(val_float)); 
#ifdef EEPROM_DEBUG	
  SerialPort.print("Max Speed : ");	
	SerialPort.println(val_float);
#endif //EEPROM_DEBUG

  val_float = MinVario;
  eepromAddress = EEPROMAnythingWrite(eepromAddress, reinterpret_cast<char*>(&val_float), sizeof(val_float)); 
#ifdef EEPROM_DEBUG	
  SerialPort.print("Min vario : ");	
	SerialPort.println(val_float);
#endif //EEPROM_DEBUG

  val_float = MaxVario;
  eepromAddress = EEPROMAnythingWrite(eepromAddress, reinterpret_cast<char*>(&val_float), sizeof(val_float)); 
#ifdef EEPROM_DEBUG	
  SerialPort.print("Max Vario : ");	
	SerialPort.println(val_float);
#endif //EEPROM_DEBUG

  eepromAddress = EEPROMAnythingWrite(eepromAddress, reinterpret_cast<char*>(&duree), sizeof(duree));   

  eepromAddress = EEPROMAnythingWrite(eepromAddress, reinterpret_cast<char*>(&time), sizeof(time)); 

  eepromAddress = EEPROMAnythingWrite(eepromAddress, reinterpret_cast<char*>(&date), sizeof(date));     
  
/*  toneAC(900);
  delay(1000);
  toneAC(0);*/

  EEPROMHAL.commit();

}

/**
 * Read a single 16 bits integer
 *
uint16_t EEPROMClassEx::readInt(int address)
{
	if (!isReadOk(address+sizeof(uint16_t))) return 0;
	return eeprom_read_word((uint16_t *) address);
}


/*
 void EEwriteFloat(int addr, float f) {
    unsigned char *buf = (unsigned char*)(&f);
    for ( int i = 0 ; i < sizeof(f) ; i++ ) {
        EEPROM.write(addr+i, buf[i]);
    }
}
 
float EEreadFloat(int addr) {
    float f;
    unsigned char *buf = (unsigned char*)(&f);
    for ( int i = 0 ; i < sizeof(f) ; i++ ) {
         buf[i] = EEPROM.read(addr+i);
    }
    return f;
}     
*/    
  
  
