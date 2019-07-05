#ifndef UTILITY_H
#define UTILITY_H

#include <Arduino.h>
#include <VarioSettings.h>

//#include <EnergySaving.h>


/******************************************************/
/******************************************************/

void indicateUncalibratedAccelerometer();
void indicatePowerDown();
void indicateFaultMS5611();
void indicateFaultMPU9250();
void indicateFaultSDCARD();
void signalBeep(double freq, unsigned long duration, int count);
int8_t percentBat(double targetVoltage);

extern VarioSettings GnuSettings;
//extern Beeper beeper;

#endif
