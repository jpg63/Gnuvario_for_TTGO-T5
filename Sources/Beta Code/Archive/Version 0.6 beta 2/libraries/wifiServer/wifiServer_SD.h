/*********************************************************************************/
/*                                                                               */
/*                           Libraries wifiServer_SD                             */
/*                                                                               */
/*  version    Date     Description                                              */
/*    1.0                                                                        */
/*    1.1    08/10/19   Ajout listDirectory                                      */
/*                                                                               */
/*********************************************************************************/

#ifndef wifiServer_SD_h
#define wifiServer_SD_h

#include <HardwareConfig.h>

#if defined (WEBSERVER_SDCARD)

#include <Arduino.h>

#ifdef ESP8266
  #include <ESP8266WiFi.h>       // Built-in
  #include <ESP8266WiFiMulti.h>  // Built-in
  #include <ESP8266WebServer.h>  // Built-in
  #include <ESP8266mDNS.h>
#else
  #include <WiFi.h>              // Built-in
  #include <WiFiMulti.h>         // Built-in
//  #include <ESP32WebServer.h>    // https://github.com/Pedroalbuquerque/ESP32WebServer download and place in your Libraries folder
//	#include <WiFiClient.h>
	#include <WebServer.h>
	#include <ESPmDNS.h>
//  #include "FS.h"
#endif

/*#ifdef HAVE_SDCARD
#include <sdcardHAL.h>
#endif //HAVE_SDCARD*/

//#define ServerVersion "1.0"
extern char   ServerVersion[20];
extern String webpage;
extern bool   SD_present;

extern char ssid_1[50];
extern char password_1[50];

extern char ssid_2[50];
extern char password_2[50];

extern char ssid_3[50];
extern char password_3[50];

extern char ssid_4[50];
extern char password_4[50];


#ifdef ESP8266
  extern ESP8266WiFiMulti wifiMulti; 
  extern ESP8266WebServer server;
#else
  extern WiFiMulti wifiMulti;
//  extern ESP32WebServer server;
	extern WebServer server;
#endif

class WifiServer {

  public:

	  boolean begin(void);
		void disableWifi(void);
		void disableBT(void);
		void disableAll(void);
		void connect(void);
		void start(void);
		void handleClient(void);
};

void returnOK();
void returnFail(String msg);
bool loadFromSdCard(String path);
void handleFileUpload();
void deleteRecursive(String path);
void handleDelete();
void handleCreate();
void listFlights();
void printDirectory();
void handleNotFound();
void myHandleFileUpload();
//void listDirectory(File dir, int numTabs);

extern WifiServer wifiServer; 

#endif //WEBSERVER_SDCARD

#endif
	