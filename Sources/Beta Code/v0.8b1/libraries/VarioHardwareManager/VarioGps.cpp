#include <Arduino.h>
#include <HardwareConfig.h>
#include <DebugConfig.h>

#ifdef HAVE_GPS

#include "VarioGps.h"

#include <SerialNmea.h>
#include <NmeaParser.h>
#include <LxnavSentence.h>
#include <LK8Sentence.h>
#include <IGCSentence.h>
#include <GPSSentence.h>

void VarioGps::init()
{
  serialNmea.begin(9600, true);
#ifdef GPS_DEBUG
  SerialPort.println("SerialNmea init");
#endif //GPS_DEBUG
}

#endif //HAVE_GPS