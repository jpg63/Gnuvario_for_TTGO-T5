/*********************************************************************************/
/*                                                                               */
/*                           Libraries eepromHal_ESP32                           */
/*                                                                               */
/*  version    Date     Description                                              */
/*    1.0    01/04/19                                                            */
/*    1.0.1  02/04/19   Ajout fonction init                                      */
/*                                                                               */
/*********************************************************************************/

//#include "eepromHAL.h"
#include "eepromHAL_ESP32.h"
#include <Arduino.h>

#if defined(ESP32)
#include <EEPROM.h>

#if not defined(DEBUG_H)
//Monitor Port 
#define SerialPort Serial

#define EEPROM_DEBUG

#endif

#define EEPROM_SIZE 64

void EepromHal_ESP32::init(void) 
{
	
	#if defined (EEPROM_DEBUG) 
    SerialPort.println("initialise EEPROM"); 
		SerialPort.flush();
#endif //EEPROM_DEBUG

  if (!EEPROM.begin(EEPROM_SIZE))
  {
#if defined (EEPROM_DEBUG) 
    SerialPort.println("failed to initialise EEPROM"); 
		SerialPort.flush();
		delay(1000000);
#endif //EEPROM_DEBUG
  }	
}

uint8_t EepromHal_ESP32::read(int address)
{
  return EEPROM.read(address);
}

void EepromHal_ESP32::update(int address, uint8_t value)
{
	if (address <= EEPROM_SIZE) {
		EEPROM.write(address, value);
		EEPROM.commit();
	}
}

void EepromHal_ESP32::write(int address, uint8_t value)
{
  if (address <= EEPROM_SIZE) EEPROM.write(address, value);
}

bool EepromHal_ESP32::isValid()
{
  return true;
}

void EepromHal_ESP32::commit()
{
	EEPROM.commit();
}
#endif //ESP32