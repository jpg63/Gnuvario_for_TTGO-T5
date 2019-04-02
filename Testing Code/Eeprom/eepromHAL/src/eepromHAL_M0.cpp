/*********************************************************************************/
/*                                                                               */
/*                           Libraries eepromHal_M0                              */
/*                                                                               */
/*  version    Date     Description                                              */
/*    1.0    05/02/19                                                            */
/*                                                                               */
/*********************************************************************************/

#include "eepromHAL.h"

#include "eepromHAL_M0.h"
#include <Arduino.h>

#if defined(ARDUINO_ARCH_SAMD)
#include "FlashAsEEPROM.h"


#if not defined(DEBUG_H)
//Monitor Port 
#define SerialPort Serial

#define EEPROM_DEBUG

#endif

void EepromHal_zero::init(void) {}

uint8_t EepromHal_zero::read(int address)
{
#if defined(ARDUINO_ARCH_SAMD)
  return EEPROM.read(address);
#endif //ARDUINO_ARCH_SAMD
}

void EepromHal_zero::update(int address, uint8_t value)
{
#if defined(ARDUINO_ARCH_SAMD)
  EEPROM.update(address, value);
#endif //ARDUINO_ARCH_SAMD
}

void EepromHal_zero::write(int address, uint8_t value)
{
#if defined(ARDUINO_ARCH_SAMD)
  EEPROM.write(address, value);
#endif //ARDUINO_ARCH_SAMD
}

bool EepromHal_zero::isValid()
{
#if defined(ARDUINO_ARCH_SAMD)
  return EEPROM.isValid();
#endif //ARDUINO_ARCH_SAMD
}

void EepromHal_zero::commit()
{
#if defined(ARDUINO_ARCH_SAMD)
  EEPROM.commit();
#endif //ARDUINO_ARCH_SAMD
}

#endif //ARDUINO_ARCH_SAMD
