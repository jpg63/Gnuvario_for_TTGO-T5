
#include <HardwareConfig.h>

#ifndef TWOWIRESCHEDULER

#include <MS5611-Ext.h>
#include <Wire.h>
#include <vertaccel2.h>
#include <MPU9250-DMP_SF_EXT.h>

MS5611 ms5611;
double referencePressure;

MPU9250_DMP imu;
Vertaccel vertaccel;

class VarioImuStd
{

public:
    VarioImuStd();
    void init();
    double getAlti();
		bool havePressure(void);
};

#endif