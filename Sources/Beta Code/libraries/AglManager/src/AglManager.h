#ifndef AGL_MANAGER_H
#define AGL_MANAGER_H

#include "HGTReader.h"

class AglManager
{
public:
    AglManager();
    void setAlti(double dAlti);
    void setLatitude(double dLatitude);
    void setLongitude(double dLongitude);
    double getHeight() { return currentHeight; }
		boolean IsOk(void); 
		void init(void);

private:
    void computeHeight();
    HGTReader *hgtReader;
    double currentAlti = -1;
    double currentLong = -1;
    double currentLat = -1;
    double currentHeight;
		boolean Directory_AGL_Exists = false;
};

extern   AglManager aglManager;

#endif //AGL_MANAGER_H