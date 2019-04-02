/*********************************************************************************/
/*                                                                               */
/*                           Libraries ToneHal                                   */
/*                                                                               */
/*  version    Date     Description                                              */
/*    1.0    20/01/19                                                            */
/*    1.1    24/01/19   Réecriture des classes                                   */
/*                      répartition en plusieurs fichiers                        */
/*    1.2    26/01/19   Modifications mineures                                   */
/*    1.3    01/04/19   Ajout ESP32                                              */
/*    1.3.1  02/04/19   Ajout fonction init                                      */
/*                                                                               */
/*********************************************************************************/
#ifndef eepromHAL_h
#define eepromHAL_h

#include <Arduino.h>

#define EEPROMHAL_EXTENDED

class EepromHal  {

  public:

	  virtual void init(void) = 0;

    /**
     * Read an eeprom cell
     * @param address
     * @return value
     */
    virtual uint8_t read(int address) = 0;

    /**
     * Write value to an eeprom cell
     * @param address
     * @param value
     */
    virtual void write(int address, uint8_t value) = 0;

    /**
     * Update a eeprom cell
     * @param address
     * @param value
     */
    virtual void update(int address, uint8_t value) = 0;

    /**
     * Check whether the eeprom data is valid
     * @return true, if eeprom data is valid (has been written at least once), false if not
     */
    virtual bool isValid() = 0;

    /**
     * Write previously made eeprom changes to the underlying flash storage
     * Use this with care: Each and every commit will harm the flash and reduce it's lifetime (like with every flash memory)
     */
    virtual void commit() = 0;

#if defined (EEPROMHAL_EXTENDED)	
	int readInt(int address);
	void writeInt(int address, int value);
	float readFloat(int address);
	void writeFloat(int address, float value);
	void readString(int offset, int bytes, char *buf);
	void writeString(int offset, int bytes, char *buf);
		
#endif	// EEPROMHAL_EXTENDED	
};

#endif
	