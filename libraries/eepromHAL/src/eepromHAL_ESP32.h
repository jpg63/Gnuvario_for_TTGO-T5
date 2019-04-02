#ifndef eepromHAL_ESP32_h
#define eepromHAL_ESP32_h

#include <Arduino.h>
#include "eepromHAL.h"

#if defined(ESP32)

class EepromHal_ESP32 : public EepromHal {

  public:

	  void init(void);
	
    /**
     * Read an eeprom cell
     * @param address
     * @return value
     */
    uint8_t read(int address);

    /**
     * Write value to an eeprom cell
     * @param address
     * @param value
     */
    void write(int address, uint8_t value);

    /**
     * Update a eeprom cell
     * @param address
     * @param value
     */
    void update(int address, uint8_t value);

    /**
     * Check whether the eeprom data is valid
     * @return true, if eeprom data is valid (has been written at least once), false if not
     */
    bool isValid();

    /**
     * Write previously made eeprom changes to the underlying flash storage
     * Use this with care: Each and every commit will harm the flash and reduce it's lifetime (like with every flash memory)
     */
    void commit();

};


#define EepromHAL EepromHal_ESP32

#endif //ESP32

#endif(
	
	


