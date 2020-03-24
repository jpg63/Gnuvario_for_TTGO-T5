#ifndef VARIO_GPS_H
#define VARIO_GPS_H

#include <HardwareConfig.h>
#include <DebugConfig.h>

#ifdef HAVE_GPS

class VarioGps
{
public:
 //   VarioAlim();
    void init();
};

#endif //HAVE_GPS
#endif //VARIO_GPS_H