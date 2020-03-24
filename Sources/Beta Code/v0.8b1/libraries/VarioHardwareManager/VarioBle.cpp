#include <Arduino.h>
#include <HardwareConfig.h>
#include <DebugConfig.h>

#ifdef HAVE_BLUETOOTH

#include "VarioBle.h"

#include "SimpleBLE.h"
#include <VarioSettings.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

bool VarioBle::init()
{
  if (GnuSettings.VARIOMETER_ENABLE_BT)
  {
#ifdef BT_DEBUG
    SerialPort.setDebugOutput(true);
    //    pinMode(0, INPUT_PULLUP);
    SerialPort.print("ESP32 SDK: ");
    SerialPort.println(ESP.getSdkVersion());
#endif //BT_DEBUG
    ble.begin("GnuVario-E");
		
		return true;
  }

  return false;
}

#endif //HAVE_BLUETOOTH
