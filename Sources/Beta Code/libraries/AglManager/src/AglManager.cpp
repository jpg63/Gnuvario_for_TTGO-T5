#include "AglManager.h"

AglManager::AglManager()
{
    hgtReader = new HGTReader("/HGT/");
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
    Serial.print("ComputeHeight ");
    Serial.print("Alti : ");
    Serial.print(currentAlti);
    Serial.print(", Lat : ");
    Serial.print(currentLat);
    Serial.print(", Long :");
    Serial.println(currentLong);
    if(currentAlti != -1 && currentLat != -1 && currentLong != -1
                        && currentLat != 0 && currentLong != 0)    
    {
        int groundLevel = hgtReader->getGroundLevel(currentLat/10, currentLong/10);
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