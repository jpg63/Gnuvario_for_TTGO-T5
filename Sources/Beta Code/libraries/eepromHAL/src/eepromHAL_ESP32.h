/*********************************************************************************/
/*                                                                               */
/*                        Libraries EEpromHAL_ESP32                              */
/*                                                                               */
/*  version    Date     Description                                              */
/*    1.0    20/01/19                                                            */
/*    1.1    24/01/19   Réecriture des classes                                   */
/*                      répartition en plusieurs fichiers                        */
/*    1.2    26/01/19   Modifications mineures                                   */
/*    1.3    01/04/19   Ajout ESP32                                              */
/*    1.3.1  02/04/19   Ajout fonction init                                      */
/*    1.3.2  07/05/19   Ajout #include <DebugConfig.h> et EepromHAL EEPROMHAL;   */
/*    1.3.3  09/07/19   Ajout init(int size)                                     */
/*                                                                               */
/*********************************************************************************/


#ifndef eepromHAL_ESP32_h
#define eepromHAL_ESP32_h

#include <Arduino.h>
#include "eepromHAL.h"

#if defined(ESP32)

class EepromHal_ESP32 : public EepromHal {

  public:

	  void init(void);

	  void init(int size);
	
	
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
extern EepromHAL EEPROMHAL;

#endif //ESP32

#endif(
	
	


