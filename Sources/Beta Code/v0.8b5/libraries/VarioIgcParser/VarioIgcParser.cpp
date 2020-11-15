/* VarioIgcParser -- 
 *
 * Copyright 2020 MichelPa / Jpg63
 * 
 * This file is part of GnuVario-E.
 *
 * ToneHAL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ToneHAL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/* 
 *********************************************************************************
 *                                                                               *
 *                           VarioIgcParser                                      *
 *                                                                               *
 *  version    Date     Description                                              *
 *    1.0                                                                        *
 *    1.1      14/11/20   json doc externe                                       *
 *                                                                               *
 *********************************************************************************
 */

#include <Arduino.h>

#include <HardwareConfig.h>
#include <DebugConfig.h>

#define JSONDOCEXTERN

#ifdef JSONDOCEXTERN
#include <VarioSettings.h>
#endif

#ifdef WIFI_DEBUG
#define ARDUINOTRACE_ENABLE 1
#else
#define ARDUINOTRACE_ENABLE 0
#endif

#define ARDUINOTRACE_SERIAL SerialPort
#include <ArduinoTrace.h>

#include "VarioIgcParser.h"
#include <MD5Builder.h>
#include <ArduinoJson.h>

VarioIgcParser::VarioIgcParser()
{
}

boolean VarioIgcParser::parseFile(String path)
{
    const TickType_t delay = (1) / portTICK_PERIOD_MS;
    File dataFile;
    String buffer;
    char tmpBuffer[100];
    uint8_t tmpBufferPos = 0;
    boolean startHeightSet = false;
    boolean startFlightTimeSet = false;
    minHeight = 9999;
    maxHeight = -9999;
    isParsed = false;

    if (!(dataFile = SDHAL_SD.open(path.c_str(), FILE_READ)))
    {
        return false;
    }
    String tmpFullName = dataFile.name();
    filename = tmpFullName.substring(tmpFullName.lastIndexOf("/") + 1);

    MD5Builder md5b;
    md5b.begin();
    uint16_t size = dataFile.size();
    while (size > 0)
    {
        uint16_t curSize = min(size, (uint16_t)4096);
        md5b.addStream(dataFile, curSize);
        size -= curSize;
        vTaskDelay(delay);
#ifdef WIFI_DEBUG
        SerialPort.print(".");
#endif //WIFI_DEBUG
    }

    md5b.calculate();
    md5 = md5b.toString();
#ifdef WIFI_DEBUG
    SerialPort.println(md5);
#endif //WIFI_DEBUG

    //on ferme pour libérer la mémoire
    dataFile.close();
    if (!(dataFile = SDHAL_SD.open(path.c_str(), FILE_READ)))
    {
        return false;
    }

    //retour au debut du fichier
    dataFile.seek(0);

    while (dataFile.available())
    {

        vTaskDelay(delay);

#ifdef WIFI_DEBUG
        SerialPort.print("-");
#endif //WIFI_DEBUG
        tmpBufferPos = 0;
        while (dataFile.available() && dataFile.peek() != '\n' && tmpBufferPos < 99) // note how this also prevents the buffer from overflowing (49 max to leave space for '\0'!)
        {
            tmpBuffer[tmpBufferPos] = dataFile.read();
            tmpBufferPos++;
        }
        if (tmpBufferPos > 0)
        {
            //on lit aussi le \n qui traine
            dataFile.read();

            tmpBuffer[tmpBufferPos + 1] = '\0';
            buffer = String(tmpBuffer);
            buffer.trim();
#ifdef WIFI_DEBUG
            SerialPort.print(".");
            // SerialPort.print(" buffer : ");
            // SerialPort.println(buffer);
#endif //WIFI_DEBUG
            if (buffer.startsWith("HFDTE"))
            {
                // date de la trace
                flightDate = buffer.substring(5);
                flightDate.trim();
                //inversion pour format americain
                flightDate = "20" + flightDate.substring(4, 6) + "-" + flightDate.substring(2, 4) + "-" + flightDate.substring(0, 2);
#ifdef WIFI_DEBUG
                SerialPort.print("flightDate : ");
                SerialPort.println(flightDate);
#endif //WIFI_DEBUG
            }
            else if (buffer.startsWith("HFPLTPILOTINCHARGE:"))
            {
                //nom du pilote
                pilot = buffer.substring(19);
                pilot.trim();
#ifdef WIFI_DEBUG
                SerialPort.print("pilot : ");
                SerialPort.println(pilot);
#endif //WIFI_DEBUG
            }
            else if (buffer.startsWith("HFGTYGLIDERTYPE:"))
            {
                //nom de la voile
                wing = buffer.substring(16);
                wing.trim();
#ifdef WIFI_DEBUG
                SerialPort.print("wing : ");
                SerialPort.println(wing);
#endif //WIFI_DEBUG
            }
            else if (buffer.startsWith("B"))
            {
                //trame trace
                // B1243314503488N00351234EA0088400927
                // B 12 43 31 4503488N 00351234E A 00884 00927
                //lat lon DDMMmmmN  DDDMMmmmE
                String hms = buffer.substring(1, 7);
                hms = hms.substring(0, 2) + ":" + hms.substring(2, 4) + ":" + hms.substring(4, 6);
                int16_t nPos = buffer.indexOf("N");
                int16_t ePos = buffer.indexOf("E");
                uint8_t aPos = buffer.indexOf("A");
                String lat = "";
                String lon = "";
                if (nPos != -1 && ePos != -1)
                {
                    lat = buffer.substring(7, nPos);
                    lon = buffer.substring(nPos + 1, ePos);
                }
                int16_t height = buffer.substring(aPos + 1, aPos + 6).toInt();

                if (!startHeightSet)
                {
                    startHeightSet = true;
                    startHeight = height;
                    if (nPos != -1 && ePos != -1)
                    {
                        startLat = lat.substring(0, 2).toDouble() + lat.substring(3, 8).toDouble() / 60000;
                        startLon = lon.substring(0, 3).toDouble() + lon.substring(3, 8).toDouble() / 60000;
                    }
                }
                if (!startFlightTimeSet)
                {
                    startFlightTimeSet = true;
                    startFlightTime = hms;
                }
                endFlightTime = hms;
                minHeight = min(height, minHeight);
                maxHeight = max(height, maxHeight);
                endHeight = height;
                if (nPos != -1 && ePos != -1)
                {
                    endLat = lat.substring(0, 2).toDouble() + lat.substring(3, 8).toDouble() / 60000;
                    endLon = lon.substring(0, 3).toDouble() + lon.substring(3, 8).toDouble() / 60000;
                }
            }
        }
    }

    dataFile.close();
#ifdef WIFI_DEBUG
    SerialPort.print("startFlightTime : ");
    SerialPort.println(startFlightTime);
    SerialPort.print("endFlightTime : ");
    SerialPort.println(endFlightTime);
    SerialPort.print("startHeight : ");
    SerialPort.println(startHeight);
    SerialPort.print("minHeight : ");
    SerialPort.println(minHeight);
    SerialPort.print("maxHeight : ");
    SerialPort.println(maxHeight);
    SerialPort.print("endHeight : ");
    SerialPort.println(endHeight);
#endif //WIFI_DEBUG

    isParsed = true;

    return true;
}

String VarioIgcParser::getPilot()
{
    if (isParsed)
    {
        return pilot;
    }
    else
    {
        return "";
    }
}

String VarioIgcParser::getWing()
{
    if (isParsed)
    {
        return wing;
    }
    else
    {
        return "";
    }
}

String VarioIgcParser::getFlightDate()
{
    if (isParsed)
    {
        return flightDate;
    }
    else
    {
        return "";
    }
}

String VarioIgcParser::getStartFlightTime()
{
    if (isParsed)
    {
        return startFlightTime;
    }
    else
    {
        return "";
    }
}

String VarioIgcParser::getEndFlightTime()
{
    if (isParsed)
    {
        return endFlightTime;
    }
    else
    {
        return "";
    }
}

int16_t VarioIgcParser::getStartHeight()
{
    if (isParsed)
    {
        return startHeight;
    }
    else
    {
        return 0;
    }
}

int16_t VarioIgcParser::getEndHeight()
{
    if (isParsed)
    {
        return endHeight;
    }
    else
    {
        return 0;
    }
}

int16_t VarioIgcParser::getMinHeight()
{
    if (isParsed)
    {
        return minHeight;
    }
    else
    {
        return 0;
    }
}

int16_t VarioIgcParser::getMaxHeight()
{
    if (isParsed)
    {
        return maxHeight;
    }
    else
    {
        return 0;
    }
}

String VarioIgcParser::getMD5()
{
    if (isParsed)
    {
        return md5;
    }
    else
    {
        return "";
    }
}

String VarioIgcParser::getFilename()
{
    if (isParsed)
    {
        return filename;
    }
    else
    {
        return "";
    }
}

double_t VarioIgcParser::getStartLat()
{
    if (isParsed)
    {
        return startLat;
    }
    else
    {
        return 0;
    }
}
double_t VarioIgcParser::getStartLon()
{
    if (isParsed)
    {
        return startLon;
    }
    else
    {
        return 0;
    }
}
double_t VarioIgcParser::getEndLat()
{
    if (isParsed)
    {
        return endLat;
    }
    else
    {
        return 0;
    }
}
double_t VarioIgcParser::getEndLon()
{
    if (isParsed)
    {
        return endLon;
    }
    else
    {
        return 0;
    }
}

String VarioIgcParser::getJson()
{
    String output;
		
#ifndef JSONDOCEXTERN
    DynamicJsonDocument doc(1024);
		#define JSONDOC doc
#else
		#define JSONDOC GnuSettings.doc	
		// Clearing Buffer
		JSONDOC.clear();
#endif
		
    if (isParsed)
    {
        JSONDOC["md5"] 							= md5;
        JSONDOC["pilot"] 						= pilot;
        JSONDOC["wing"] 						= wing;
        JSONDOC["flightDate"] 			= flightDate;
        JSONDOC["startFlightTime"] 	= startFlightTime;
        JSONDOC["endFlightTime"] 		= endFlightTime;
        JSONDOC["startHeight"] 			= startHeight;
        JSONDOC["endHeight"] 				= endHeight;
        JSONDOC["minHeight"] 				= minHeight;
        JSONDOC["maxHeight"] 				= maxHeight;
        JSONDOC["filename"] 				= filename;
        JSONDOC["startLat"] 				= startLat;
        JSONDOC["startLon"] 				= startLon;
        JSONDOC["endLat"] 					= endLat;
        JSONDOC["endLon"] 					= endLon;
    }
    else
    {
        JSONDOC.to<JsonObject>();
    }
    serializeJson(JSONDOC, output);
    return output;
}
