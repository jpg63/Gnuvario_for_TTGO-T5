/*********************************************************************************/
/*                                                                               */
/*                           Libraries wifiServer_SD                             */
/*                                                                               */
/*  version    Date     Description                                              */
/*    1.0                                                                        */
/*    1.1    08/10/19   Ajout listDirectory                                      */
/*                                                                               */
/*********************************************************************************/


#include <Arduino.h>
#include <HardwareConfig.h>
#include <DebugConfig.h>

#if defined(WEBSERVER_SDCARD)

#include <wifiServer_SD.h>

#ifdef HAVE_SDCARD
#include <sdcardHAL.h>
#endif

#include <SPI.h>

#include <VarioSettings.h>

#include <Update.h>

#include <ArduinoJson.h>

extern VarioSettings GnuSettings;

#define TAG "server"

bool   SD_present = false;
char   ServerVersion[20]; // = "3.4";

#ifdef _HARDWARECONFIG_H_
IPAddress local_IP(LOCAL_IP); // Set your server's fixed IP address here
IPAddress gateway(GATEWAY);    // Set your network Gateway usually your Router base address
IPAddress subnet(SUBNET);   // Set your network sub-network mask here
IPAddress dns(DNS);           // Set your network DNS usually your Router base address
#else
IPAddress local_IP(192, 168, 1, 150); // Set your server's fixed IP address here
IPAddress gateway(192, 168, 1, 1);    // Set your network Gateway usually your Router base address
IPAddress subnet(255, 255, 255, 0);   // Set your network sub-network mask here
IPAddress dns(192,168,1,1);           // Set your network DNS usually your Router base address	
#endif

char ssid_1[50];
char password_1[50];

char ssid_2[50];
char password_2[50];

char ssid_3[50];
char password_3[50];

char ssid_4[50];
char password_4[50];

File uploadFile; 

#include "CSS.h"

//************************************************************
// DISPLAY SCREEN
//************************************************************
#define ENABLE_DISPLAY_WEBSERVER

#ifdef ENABLE_DISPLAY_WEBSERVER
#include <varioscreenGxEPD.h>
#endif

static bool hasSD = false;

/***********************************/
void returnOK() {
/***********************************/
  server.send(200, "text/plain", "");
}

/***********************************/
void returnFail(String msg) {
/***********************************/
  server.send(500, "text/plain", msg + "\r\n");
}

/***********************************/
bool loadFromSdCard(String path) {
/***********************************/
  String dataType = "text/plain";
	
  String basePath = "www";

  path = basePath + path;
	
  if (path.endsWith("/")) {
    path += "index.htm";
  }

  if (path.endsWith(".src")) {
    path = path.substring(0, path.lastIndexOf("."));
  } else if (path.endsWith(".htm")) {
    dataType = "text/html";
  } else if (path.endsWith(".css")) {
    dataType = "text/css";
  } else if (path.endsWith(".js")) {
    dataType = "application/javascript";
  } else if (path.endsWith(".png")) {
    dataType = "image/png";
  } else if (path.endsWith(".gif")) {
    dataType = "image/gif";
  } else if (path.endsWith(".jpg")) {
    dataType = "image/jpeg";
  } else if (path.endsWith(".ico")) {
    dataType = "image/x-icon";
  } else if (path.endsWith(".xml")) {
    dataType = "text/xml";
  } else if (path.endsWith(".pdf")) {
    dataType = "application/pdf";
  } else if (path.endsWith(".zip")) {
    dataType = "application/zip";
	} else if (path.endsWith(".svg")) {
    dataType = "image/svg+xml";
  }

#ifdef HAVE_SDCARD

  File dataFile = SDHAL.open(path.c_str());
  if (dataFile.isDirectory()) {
    path += "/index.htm";
    dataType = "text/html";
    dataFile = SDHAL.open(path.c_str());
  }

  if (!dataFile) {
    return false;
  }

  if (server.hasArg("download")) {
    dataType = "application/octet-stream";
  }

  if (server.streamFile(dataFile, dataType) != dataFile.size()) {
#ifdef WIFI_DEBUG
    SerialPort.println("Sent less data than expected!");
#endif
  }

  dataFile.close();
  return true;
#else
	return false;
#endif
}

/***********************************/
void handleFileUpload() {
/***********************************/
  if (server.uri() != "/edit") {
    return;
  }
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    if (SDHAL.exists((char *)upload.filename.c_str())) {
      SDHAL.remove((char *)upload.filename.c_str());
    }
    uploadFile = SDHAL.open(upload.filename.c_str(), FILE_WRITE);
#ifdef WIFI_DEBUG
    SerialPort.print("Upload: START, filename: "); SerialPort.println(upload.filename);
#endif
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (uploadFile) {
      uploadFile.write(upload.buf, upload.currentSize);
    }
#ifdef WIFI_DEBUG
    SerialPort.print("Upload: WRITE, Bytes: "); SerialPort.println(upload.currentSize);
#endif
  } else if (upload.status == UPLOAD_FILE_END) {
    if (uploadFile) {
      uploadFile.close();
    }
#ifdef WIFI_DEBUG
    SerialPort.print("Upload: END, Size: "); SerialPort.println(upload.totalSize);
#endif
  }
}

/***********************************/
void deleteRecursive(String path) {
/***********************************/
  File fileSD = SDHAL.open((char *)path.c_str());
  if (!fileSD.isDirectory()) {
    fileSD.close();
    SDHAL.remove((char *)path.c_str());
    return;
  }

  fileSD.rewindDirectory();
  while (true) {
    File entry = fileSD.openNextFile();
    if (!entry) {
      break;
    }
    String entryPath = path + "/" + entry.name();
    if (entry.isDirectory()) {
      entry.close();
      deleteRecursive(entryPath);
    } else {
      entry.close();
      SDHAL.remove((char *)entryPath.c_str());
    }
    yield();
  }

  SDHAL.rmdir((char *)path.c_str());
  fileSD.close();
}

/***********************************/
void handleDelete() {
/***********************************/
  if (server.args() == 0) {
    return returnFail("BAD ARGS");
  }
  String path = server.arg(0);
  if (path == "/" || !SDHAL.exists((char *)path.c_str())) {
    returnFail("BAD PATH");
    return;
  }
  deleteRecursive(path);
  returnOK();
}

/***********************************/
void handleCreate() {
/***********************************/
  if (server.args() == 0) {
    return returnFail("BAD ARGS");
  }
  String path = server.arg(0);
  if (path == "/" || SDHAL.exists((char *)path.c_str())) {
    returnFail("BAD PATH");
    return;
  }

  if (path.indexOf('.') > 0) {
    File fileSD = SDHAL.open((char *)path.c_str(), FILE_WRITE);
    if (fileSD) {
      fileSD.write("\0");
      fileSD.close();
    }
  } else {
    SDHAL.mkdir((char *)path.c_str());
  }
  returnOK();
}

/***********************************/
void handleNotFound() {
/***********************************/
  if (hasSD && loadFromSdCard(server.uri())) {
    return;
  }
  String message = "SDCARD Not Detected\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " NAME:" + server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  SerialPort.print(message);
}

/***********************************/
void myHandleFileUpload() {
/***********************************/	
  if (server.uri() != "/upload") {
    return;
  }

	HTTPUpload& upload = server.upload();
	if (upload.status == UPLOAD_FILE_START) {
		if (SDHAL.exists((char *)upload.filename.c_str())) {
			SDHAL.remove((char *)upload.filename.c_str());
		}
		uploadFile = SDHAL.open(upload.filename.c_str(), FILE_WRITE);
#ifdef WIFI_DEBUG
		SerialPort.print("Upload: START, filename: "); SerialPort.println(upload.filename);
#endif
	} else if (upload.status == UPLOAD_FILE_WRITE) {
		if (uploadFile) {
			uploadFile.write(upload.buf, upload.currentSize);
		}
#ifdef WIFI_DEBUG
		SerialPort.print("Upload: WRITE, Bytes: "); SerialPort.println(upload.currentSize);
#endif
	} else if (upload.status == UPLOAD_FILE_END) {
		if (uploadFile) {
			uploadFile.close();
		}
#ifdef WIFI_DEBUG
		SerialPort.print("Upload: END, Size: "); SerialPort.println(upload.totalSize);
#endif
	}

	 returnOK();

}

/***********************************/
void listFlights() {
/***********************************/
  String path;
  path = "/vols";
  File dir = SDHAL.open((char *)path.c_str());
  path = String();
  if (!dir.isDirectory()) {
    dir.close();
    return returnFail("NOT DIR");
  }
  dir.rewindDirectory();
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/json", "");
  WiFiClient client = server.client();

  server.sendContent("[");
  for (int cnt = 0; true; ++cnt) {
    File entry = dir.openNextFile();
    if (!entry) {
      break;
    }

    String output;
    if (cnt > 0) {
      output = ',';
    }

    String fsize = "";
    if (!entry.isDirectory()){
      int bytes = entry.size();

      if (bytes < 1024)                     fsize = String(bytes)+" B";
      else if(bytes < (1024 * 1024))        fsize = String(bytes/1024.0,3)+" KB";
      else if(bytes < (1024 * 1024 * 1024)) fsize = String(bytes/1024.0/1024.0,3)+" MB";
      else                                  fsize = String(bytes/1024.0/1024.0/1024.0,3)+" GB";
    } else {
      fsize = "na";
    }

    output += "{\"type\":\"";
    output += (entry.isDirectory()) ? "dir" : "file";
    output += "\",\"name\":\"";
    output += entry.name();
    output += "\",\"size\":\"";
    output += fsize;
    output += "\"";
    output += "}";
    server.sendContent(output);
    entry.close();
  }
  server.sendContent("]");
  dir.close();
}

/***********************************/
void printDirectory() {
/***********************************/
  if (!server.hasArg("dir")) {
    return returnFail("BAD ARGS");
  }
  String path = server.arg("dir");
  if (path != "/" && !SDHAL.exists((char *)path.c_str())) {
    return returnFail("BAD PATH");
  }
  File dir = SDHAL.open((char *)path.c_str());
  path = String();
  if (!dir.isDirectory()) {
    dir.close();
    return returnFail("NOT DIR");
  }
  dir.rewindDirectory();
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/json", "");
  WiFiClient client = server.client();

  server.sendContent("[");
  for (int cnt = 0; true; ++cnt) {
    File entry = dir.openNextFile();
    if (!entry) {
      break;
    }

    String output;
    if (cnt > 0) {
      output = ',';
    }

    output += "{\"type\":\"";
    output += (entry.isDirectory()) ? "dir" : "file";
    output += "\",\"name\":\"";
    output += entry.name();
    output += "\"";
    output += "}";
    server.sendContent(output);
    entry.close();
  }
  server.sendContent("]");
  dir.close();
}

/***********************************/
void listDirectory(File dir, int numTabs) {
/***********************************/
//	DynamicJsonBuffer jsonBuffer;
//	DynamicJsonDocument doc(1024);
//  JsonArray& array = jsonBuffer.createArray();

	dir.rewindDirectory();
	
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      SerialPort.print('\t');
    }
    SerialPort.print(entry.name());
    if (entry.isDirectory()) {
      SerialPort.println("/");
      listDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      SerialPort.print("\t\t");
      SerialPort.println(entry.size(), DEC);
    }
		
/*    JsonObject& object = jsonBuffer.createObject();

    object["type"] = (entry.isDirectory()) ? "dir" : "file";
    object["name"] = String(entry.name());
    array.add(object);	*/	
		
    entry.close();
  }
}
 
/***********************************/
void listDirectoryJson(File dir, int numTabs) {
/***********************************/
//	DynamicJsonBuffer jsonBuffer;
	DynamicJsonDocument doc(1024);
//  JsonArray& array = jsonBuffer.createArray();
	JsonArray array = doc.to<JsonArray>();
	
	dir.rewindDirectory();
	
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }

    JsonObject object = doc.createNestedObject();
    object["type"] = (entry.isDirectory()) ? "dir" : "file";
    object["name"] = String(entry.name());
    array.add(object);
		
    if (entry.isDirectory()) {
      listDirectoryJson(entry, numTabs + 1);
    }
		
    entry.close();
  }
	
  String response = "";
	serializeJson(doc, response);
	SerialPort.println("");
	SerialPort.println(response);
}
 
 /*
 JsonObject obj1 = doc.createNestedObject();
obj1["key"] = "a1";
obj1["value"] = analogRead(A1);

// Convert the JsonDocument to an array
JsonArray arr = doc.to<JsonArray>();
 
arr[0] = 666;
arr[1] = 667; 
 
 void printDirectory(AsyncWebServerRequest *request) {
  if( ! request->hasParam("dir")) return returnFail(request, "BAD ARGS");
  String path = request->arg("dir");
  if(path != "/" && !SD.exists((char *)path.c_str())) return returnFail(request, "BAD PATH");
  File dir = SD.open((char *)path.c_str());
  path = String();
  if(!dir.isDirectory()){
    dir.close();
    return returnFail(request, "NOT DIR");
  }
  DynamicJsonBuffer jsonBuffer;
  JsonArray& array = jsonBuffer.createArray();
  dir.rewindDirectory();
  File entry;
  while(entry = dir.openNextFile()){
    JsonObject& object = jsonBuffer.createObject();
    object["type"] = (entry.isDirectory()) ? "dir" : "file";
    object["name"] = String(entry.name());
    array.add(object);
    entry.close();
  }  
  String response = "";
  array.printTo(response);
  request->send(200, "application/json", response);
  dir.close();
}
 
 
 https://arduinojson.org/v6/doc/serialization/
 */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
boolean WifiServer::begin(void) {
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	GnuSettings.VARIOMETER_SSID_1.toCharArray(ssid_1,sizeof(ssid_1));
	GnuSettings.VARIOMETER_PASSWORD_1.toCharArray(password_1,sizeof(password_1));

#ifdef WIFI_DEBUG
  SerialPort.print("ssid_1 : ");
  SerialPort.println(ssid_1);
  SerialPort.print("password_1 : ");
  SerialPort.println(password_1);
#endif //WIFI_DEBUG

	GnuSettings.VARIOMETER_SSID_2.toCharArray(ssid_2,sizeof(ssid_2));
	GnuSettings.VARIOMETER_PASSWORD_2.toCharArray(password_2,sizeof(password_2));

#ifdef WIFI_DEBUG
  SerialPort.print("ssid_2 : ");
  SerialPort.println(ssid_2);
  SerialPort.print("password_2 : ");
  SerialPort.println(password_2);
#endif //WIFI_DEBUG

	GnuSettings.VARIOMETER_SSID_3.toCharArray(ssid_3,sizeof(ssid_3));
	GnuSettings.VARIOMETER_PASSWORD_3.toCharArray(password_3,sizeof(password_3));
	
#ifdef WIFI_DEBUG
  SerialPort.print("ssid_3 : ");
  SerialPort.println(ssid_3);
  SerialPort.print("password_3 : ");
  SerialPort.println(password_3);
#endif //WIFI_DEBUG

	GnuSettings.VARIOMETER_SSID_4.toCharArray(ssid_4,sizeof(ssid_4));
	GnuSettings.VARIOMETER_PASSWORD_4.toCharArray(password_4,sizeof(password_4));

#ifdef WIFI_DEBUG
  SerialPort.print("ssid_4 : ");
  SerialPort.println(ssid_4);
  SerialPort.print("password_4 : ");
  SerialPort.println(password_4);
#endif //WIFI_DEBUG

  File dataFile = SDHAL.open("www/index.htm");
  if (dataFile) {
//    if (SDHAL.exists("www/index.html")) {
#ifdef WIFI_DEBUG
    SerialPort.println("SD Card initialized.");
#endif //WIFI_DEBUG

    hasSD = true;
  	dataFile.close();
  }
  
  dataFile = SDHAL.open("/");

  dataFile.rewindDirectory();
  listDirectory(dataFile, 0);
  listDirectoryJson(dataFile, 0);

	if (hasSD != true) {
#ifdef WIFI_DEBUG
		SerialPort.println("Erreur SDCARD");
#endif
		return false;
	}
	
#ifdef WIFI_DEBUG
  SerialPort.println("done!");
#endif
  	
	return true;
};

/***********************************/
void WifiServer::disableWifi(void) {
/***********************************/
	WiFi.mode(WIFI_OFF);
}

/***********************************/
void WifiServer::disableBT(void) {
/***********************************/
}
		
void WifiServer::disableAll(void) {
	disableWifi();
	disableBT();
}
   
/***********************************/
void WifiServer::connect(void) {
/***********************************/
#ifdef IP_FIXE
 if (!WiFi.config(local_IP, gateway, subnet, dns)) { //WiFi.config(ip, gateway, subnet, dns1, dns2);
    SerialPort.println("WiFi STATION Failed to configure Correctly"); 
  } 
#endif //IP_FIXE	
	
  wifiMulti.addAP(ssid_1, password_1);  // add Wi-Fi networks you want to connect to, it connects strongest to weakest
  wifiMulti.addAP(ssid_2, password_2);  // Adjust the values in the Network tab
  wifiMulti.addAP(ssid_3, password_3);
  wifiMulti.addAP(ssid_4, password_4);  // You don't need 4 entries, this is for example!
  
  SerialPort.println("Connecting ...");
#ifdef ENABLE_DISPLAY_WEBSERVER
	screen.ScreenViewWifi("", "");
#endif //ENABLE_DISPLAY_WEBSERVER
	
	int counter = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250); SerialPort.print('.');
		counter++;
		if (counter > 30) {
			SerialPort.println(F("Error Wifi not found!")); 
			ESP.restart(); 
		}
  }
  SerialPort.println("\nConnected to "+WiFi.SSID()+" Use IP address: "+WiFi.localIP().toString()); // Report which SSID and IP is in use
  // The logical name http://fileserver.local will also access the device if you have 'Bonjour' running or your system supports multicast dns
#ifdef ENABLE_DISPLAY_WEBSERVER
	screen.ScreenViewWifi(WiFi.SSID().substring(0,12), WiFi.localIP().toString());
#endif //ENABLE_DISPLAY_WEBSERVER

  if (MDNS.begin(servername)) {
    MDNS.addService("http", "tcp", 80);
    SerialPort.println("MDNS responder started");
    SerialPort.print("You can now connect to http://");
    SerialPort.print(servername);
    SerialPort.println(".local");
  }
  else {          
    SerialPort.println(F("Error setting up MDNS responder!")); 
    ESP.restart(); 
  } 
}

/***********************************/
void WifiServer::start(void) {
/***********************************/
//  if (SD.begin(SS)) {
	SerialPort.println("SD Card initialized.");
  hasSD = true;
//  }

  server.on("/list", HTTP_GET, printDirectory);
  server.on("/edit", HTTP_DELETE, handleDelete);
  server.on("/edit", HTTP_PUT, handleCreate);
  server.on("/edit", HTTP_POST, []() {
	    returnOK();
  }, handleFileUpload);
  server.on("/flights", HTTP_GET, listFlights);
  server.on("/upload", HTTP_POST, []() {
    returnOK();
  }, myHandleFileUpload);
  server.onNotFound(handleNotFound);

	
  ///////////////////////////// End of Request commands
  server.begin();
  SerialPort.println("HTTP server started");
#ifdef ENABLE_DISPLAY_WEBSERVER
	screen.ScreenViewWifi("START", "");
#endif //ENABLE_DISPLAY_WEBSERVER

}

/***********************************/
void WifiServer::handleClient(void){
/***********************************/
	server.handleClient(); // Listen for client connections
}


WifiServer wifiServer; 

#endif //WEBSERVER_SDCARD

