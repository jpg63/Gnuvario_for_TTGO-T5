#include <HardwareConfig.h>
#include <DebugConfig.h>

#include "AglManager.h"

#ifdef AGL_DEBUG
#define ARDUINOTRACE_ENABLE 1
#else
#define ARDUINOTRACE_ENABLE 0
#endif

#define ARDUINOTRACE_SERIAL SerialPort
#include <ArduinoTrace.h>

#include <VarioLog.h>

#define AGL_Directory "/AGL"

AglManager::AglManager()
{   
	String tmpStr = AGL_Directory;
	tmpStr += "/";
	hgtReader = new HGTReader(tmpStr);
}

void AglManager::init(void) 
{
	char tmpFileName[15] = AGL_Directory;
 
	if (SDHAL_SD.exists(tmpFileName)) Directory_AGL_Exists = true;
  else														  Directory_AGL_Exists = false;

#ifdef AGL_DEBUG
	SerialPort.print("INIT AGL : Directory exists : ");
	SerialPort.println(Directory_AGL_Exists);
#endif //AGL_DEBUG
}

void AglManager::setAlti(double dAlti)
{
    if (currentAlti != dAlti)
    {
        currentAlti = dAlti;
        computeHeight();
    }
}

void AglManager::setLatitude(double dLatitude)
{
    if (currentLat != dLatitude)
    {
        currentLat = dLatitude;
        computeHeight();
    }
}

void AglManager::setLongitude(double dLongitude)
{
    if (currentLong != dLongitude)
    {
        currentLong = dLongitude;
        computeHeight();
    }
}

void AglManager::computeHeight()
{
#ifdef AGL_DEBUG
	SerialPort.print("ComputeHeight ");
	SerialPort.print("Alti : ");
	SerialPort.print(currentAlti);
	SerialPort.print(", Lat : ");
	SerialPort.print(currentLat);
	SerialPort.print(", Long :");
	SerialPort.println(currentLong);
#endif //AGL_DEBUG

  if (IsOk()) {
    if(currentAlti != -1 && currentLat != -1 && currentLong != -1
                        && currentLat != 0 && currentLong != 0)    
    {
        int groundLevel = hgtReader->getGroundLevel(currentLat, currentLong);
        if(groundLevel != NO_FILE_FOR_POS)
        {
            currentHeight = currentAlti - groundLevel;
        }
        else
        {
            currentHeight = currentAlti;
        }        
    }
    else
    {
        currentHeight = currentAlti;
    }
	}
	else 
	{	
        currentHeight = currentAlti;
  }
#ifdef AGL_DEBUG
	SerialPort.print("AGL Height ");
	SerialPort.println(currentHeight);
#endif //AGL_DEBUG
}

boolean AglManager::IsOk(void) 
{ 

#ifdef AGL_DEBUG
	SerialPort.print("AGL Directory exists : ");
	SerialPort.println(Directory_AGL_Exists);
#endif //AGL_DEBUG

  return Directory_AGL_Exists; 
}

/*
ComputeHeight Alti : 444.84, Lat : 45.46, Long :3.12
getGroundLevel(4.546228,0.311662)
N04W000.HGT
Filename : N04W000.HGT
Path : AGL/N04W000.HGT
Open KO
Echec ouverture
*/