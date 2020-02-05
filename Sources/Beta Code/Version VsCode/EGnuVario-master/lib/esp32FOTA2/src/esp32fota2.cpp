/*
   esp32 firmware OTA
   Date: December 2018
   Author: Chris Joyce <https://chrisjoyce911/esp32FOTA>
   Purpose: Perform an OTA update from a bin located on a webserver (HTTP & HTTPS)
*/

/* 
 *********************************************************************************
 *                                                                               *
 *                           esp32fota2                                          *
 *                                                                               *
 *  version    Date     Description                                              *
 *    1.0    18/01/20                                                            *
 *    1.0.1  19/01/20   Ajout execHTTPexist et execHTTPSexist										 *
 *                                                                               *
 *********************************************************************************/

#include <DebugConfig.h>
#include "esp_log.h"

#include <HardwareConfig.h>

#ifdef WIFI_DEBUG
#define ARDUINOTRACE_ENABLE 1
#else
#define ARDUINOTRACE_ENABLE 0
#endif

#define ARDUINOTRACE_SERIAL SerialPort
#include <ArduinoTrace.h>
//#include "myassert.h"

#include "esp32fota2.h"
#include "Arduino.h"
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <Update.h>
#include "ArduinoJson.h"
#include <WiFiClientSecure.h>

#include <varioscreenGxEPD.h>

// This is the root Certificate Authority that signed 
// the server certifcate for server https://github.com 
/*const char* rootCACertificate = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIEIzCCAwugAwIBAgIQT5Qzz46+WowBY93PLJ+8CDANBgkqhkiG9w0BAQsFADBI\n" \
"MRswGQYDVQQDExJFU0VUIFNTTCBGaWx0ZXIgQ0ExHDAaBgNVBAoTE0VTRVQsIHNw\n" \
"b2wuIHMgci4gby4xCzAJBgNVBAYTAlNLMB4XDTE4MDUwODAwMDAwMFoXDTIwMDYw\n" \
"MzEyMDAwMFowgccxHTAbBgNVBA8MFFByaXZhdGUgT3JnYW5pemF0aW9uMRMwEQYL\n" \
"KwYBBAGCNzwCAQMTAlVTMRkwFwYLKwYBBAGCNzwCAQITCERlbGF3YXJlMRAwDgYD\n" \
"VQQFEwc1MTU3NTUwMQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEW\n" \
"MBQGA1UEBxMNU2FuIEZyYW5jaXNjbzEVMBMGA1UEChMMR2l0SHViLCBJbmMuMRMw\n" \
"EQYDVQQDEwpnaXRodWIuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKC\n" \
"AQEAw8pdTS/pX2hGcbERt1bg82rQEqQGZQLLX0iZE4xPrITjMZaAbIjHLy60+sKt\n" \
"EklH33Zln/7g8dnzyFhA03npKmJ18a3kAj3MkcEBQbh/z4s+CUZjSgjkYlXnU+sR\n" \
"lqTeRx2SvACa+RbyCQFjAVfPkx8LkdCDnzha+JwaZL6H3VsTxIIK//Nh/2NGOacg\n" \
"Y2FhDwgc2K/XxeXRlqkXy8Ej/eu+4uuF4VFcjCdm7UEUSQwuSiXi0IMl4KJqqEyk\n" \
"2qHkS8KI39qcQgqGfxoZj+7Ia2Y9I7TGVu0mrwI+NGpRt04zHR11MCjOB5ydbqRu\n" \
"WxEypDJ7q6zwTCFCsryHiFxuqQIDAQABo4GIMIGFMCUGA1UdEQQeMByCCmdpdGh1\n" \
"Yi5jb22CDnd3dy5naXRodWIuY29tMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEF\n" \
"BQcDAjAMBgNVHRMBAf8EAjAAMA4GA1UdDwEB/wQEAwIFoDAfBgNVHSMEGDAWgBRM\n" \
"CcmJdRovgGSqmdi30CucbWA/PzANBgkqhkiG9w0BAQsFAAOCAQEAEcUQF4xJ6WiV\n" \
"71AVvxIQg9GgWzXnpgpJ2ycYD70cynZFZuJ4Uj1Hn4kOEEoSRfKzGJCdQXVTuM4s\n" \
"wb6EET3zrkQdfrct5PkGAr7k+ZedBAGU+N5ci0WEfJVgK1KTjgoWOaW2sWtxkjDv\n" \
"fFBpL8JD7VIWZTgH6A7y4lQehF0VsIPKE3U9WuIckjNjL8F+9cnHn4Yr8I2qtzRz\n" \
"RL6Mkj6k3uWp6hqoBLgP3blRCnESxb7ibSZP5agLvk0O/eHixb+Jn+unroxTk1u7\n" \
"PeuGH30HmyAvHHSnrq788ODFfEKN6Xcn8Q5NbVETN9Wae35udRpL0IRR+fAKfVEj\n" \
"SEyFvvhD+w==\n" \
"-----END CERTIFICATE-----\n";*/

const char* rootCACertificate = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIF6TCCA9GgAwIBAgIQBeTcO5Q4qzuFl8umoZhQ4zANBgkqhkiG9w0BAQwFADCB\n" \
"iDELMAkGA1UEBhMCVVMxEzARBgNVBAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0pl\n" \
"cnNleSBDaXR5MR4wHAYDVQQKExVUaGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNV\n" \
"BAMTJVVTRVJUcnVzdCBSU0EgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMTQw\n" \
"OTEyMDAwMDAwWhcNMjQwOTExMjM1OTU5WjBfMQswCQYDVQQGEwJGUjEOMAwGA1UE\n" \
"CBMFUGFyaXMxDjAMBgNVBAcTBVBhcmlzMQ4wDAYDVQQKEwVHYW5kaTEgMB4GA1UE\n" \
"AxMXR2FuZGkgU3RhbmRhcmQgU1NMIENBIDIwggEiMA0GCSqGSIb3DQEBAQUAA4IB\n" \
"DwAwggEKAoIBAQCUBC2meZV0/9UAPPWu2JSxKXzAjwsLibmCg5duNyj1ohrP0pIL\n" \
"m6jTh5RzhBCf3DXLwi2SrCG5yzv8QMHBgyHwv/j2nPqcghDA0I5O5Q1MsJFckLSk\n" \
"QFEW2uSEEi0FXKEfFxkkUap66uEHG4aNAXLy59SDIzme4OFMH2sio7QQZrDtgpbX\n" \
"bmq08j+1QvzdirWrui0dOnWbMdw+naxb00ENbLAb9Tr1eeohovj0M1JLJC0epJmx\n" \
"bUi8uBL+cnB89/sCdfSN3tbawKAyGlLfOGsuRTg/PwSWAP2h9KK71RfWJ3wbWFmV\n" \
"XooS/ZyrgT5SKEhRhWvzkbKGPym1bgNi7tYFAgMBAAGjggF1MIIBcTAfBgNVHSME\n" \
"GDAWgBRTeb9aqitKz1SA4dibwJ3ysgNmyzAdBgNVHQ4EFgQUs5Cn2MmvTs1hPJ98\n" \
"rV1/Qf1pMOowDgYDVR0PAQH/BAQDAgGGMBIGA1UdEwEB/wQIMAYBAf8CAQAwHQYD\n" \
"VR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMCIGA1UdIAQbMBkwDQYLKwYBBAGy\n" \
"MQECAhowCAYGZ4EMAQIBMFAGA1UdHwRJMEcwRaBDoEGGP2h0dHA6Ly9jcmwudXNl\n" \
"cnRydXN0LmNvbS9VU0VSVHJ1c3RSU0FDZXJ0aWZpY2F0aW9uQXV0aG9yaXR5LmNy\n" \
"bDB2BggrBgEFBQcBAQRqMGgwPwYIKwYBBQUHMAKGM2h0dHA6Ly9jcnQudXNlcnRy\n" \
"dXN0LmNvbS9VU0VSVHJ1c3RSU0FBZGRUcnVzdENBLmNydDAlBggrBgEFBQcwAYYZ\n" \
"aHR0cDovL29jc3AudXNlcnRydXN0LmNvbTANBgkqhkiG9w0BAQwFAAOCAgEAWGf9\n" \
"crJq13xhlhl+2UNG0SZ9yFP6ZrBrLafTqlb3OojQO3LJUP33WbKqaPWMcwO7lWUX\n" \
"zi8c3ZgTopHJ7qFAbjyY1lzzsiI8Le4bpOHeICQW8owRc5E69vrOJAKHypPstLbI\n" \
"FhfFcvwnQPYT/pOmnVHvPCvYd1ebjGU6NSU2t7WKY28HJ5OxYI2A25bUeo8tqxyI\n" \
"yW5+1mUfr13KFj8oRtygNeX56eXVlogMT8a3d2dIhCe2H7Bo26y/d7CQuKLJHDJd\n" \
"ArolQ4FCR7vY4Y8MDEZf7kYzawMUgtN+zY+vkNaOJH1AQrRqahfGlZfh8jjNp+20\n" \
"J0CT33KpuMZmYzc4ZCIwojvxuch7yPspOqsactIGEk72gtQjbz7Dk+XYtsDe3CMW\n" \
"1hMwt6CaDixVBgBwAc/qOR2A24j3pSC4W/0xJmmPLQphgzpHphNULB7j7UTKvGof\n" \
"KA5R2d4On3XNDgOVyvnFqSot/kGkoUeuDcL5OWYzSlvhhChZbH2UF3bkRYKtcCD9\n" \
"0m9jqNf6oDP6N8v3smWe2lBvP+Sn845dWDKXcCMu5/3EFZucJ48y7RetWIExKREa\n" \
"m9T8bJUox04FB6b9HbwZ4ui3uRGKLXASUoWNjDNKD/yZkuBjcNqllEdjB+dYxzFf\n" \
"BT02Vf6Dsuimrdfp5gJ0iHRc2jTbkNJtUQoj1iM=\n" \
"-----END CERTIFICATE-----\n";


//************************************
esp32FOTA2::esp32FOTA2(String firwmareType, int firwmareVersion, int firwmareSubVersion, int firwmareBetaVersion)
//************************************
{
    _firwmareType        = firwmareType;
		DUMP(_firwmareType);
    _firwmareVersion     = firwmareVersion;
		_firwmareSubVersion  = firwmareSubVersion;
		_firwmareBetaVersion = firwmareBetaVersion;
    useDeviceID = false;
}

// Utility to extract header value from headers
//************************************
String esp32FOTA2::getHeaderValue(String header, String headerName)
//************************************
{
    return header.substring(strlen(headerName.c_str()));
}

// OTA Logic
//************************************
void esp32FOTA2::execOTA()
//************************************
{

	TRACE();

    screen.ScreenViewReboot("Upgrade");

    WiFiClient client;
    int contentLength = 0;
    bool isValidContentType = false;

    SerialPort.println("Connecting to: " + String(_host));
    // Connect to Webserver
//    if (client.connect(_host.c_str(), _port))
		if (client.connect(_host.c_str(), _port))
    {
        // Connection Succeed.
        // Fecthing the bin
        SerialPort.println("Fetching Bin: " + String(_bin));

        // Get the contents of the bin file
        client.print(String("GET ") + _bin + " HTTP/1.1\r\n" +
                     "Host: " + _host + "\r\n" +
                     "Cache-Control: no-cache\r\n" +
                     "Connection: close\r\n\r\n");

 

        unsigned long timeout = millis();
        while (client.available() == 0)
        {
            if (millis() - timeout > 5000)
            {
                SerialPort.println("Client Timeout !");
                client.stop();
                return;
            }
        }

        while (client.available())
        {
            // read line till /n
            String line = client.readStringUntil('\n');
            // remove space, to check if the line is end of headers
            line.trim();

            if (!line.length())
            {
                //headers ended
                break; // and get the OTA started
            }

            // Check if the HTTP Response is 200
            // else break and Exit Update
            if (line.startsWith("HTTP/1.1"))
            {
                SerialPort.println("HTTP LINE");
                SerialPort.println(line);
                if (line.indexOf("200") < 0)
                {
                    SerialPort.println("Got a non 200 status code from server. Exiting OTA Update.");
                    break;
                }
            }

            // extract headers here
            // Start with content length
            if (line.startsWith("Content-Length: "))
            {
                contentLength = atoi((getHeaderValue(line, "Content-Length: ")).c_str());
                SerialPort.println("Got " + String(contentLength) + " bytes from server");
            }

            // Next, the content type
            if (line.startsWith("Content-Type: "))
            {
                String contentType = getHeaderValue(line, "Content-Type: ");
                SerialPort.println("Got " + contentType + " payload.");
                if (contentType == "application/octet-stream")
                {
                    isValidContentType = true;
                }
            }
        }
    }
    else
    {
        // Connect to webserver failed
        // May be try?
        // Probably a choppy network?
#ifdef WIFI_DEBUG
        SerialPort.println("Connection to " + String(_host) + " failed. Please check your setup");
#endif
        // retry??
        // execOTA();
    }

    // Check what is the contentLength and if content type is `application/octet-stream`
#ifdef WIFI_DEBUG
    SerialPort.println("contentLength : " + String(contentLength) + ", isValidContentType : " + String(isValidContentType));
#endif

    // check contentLength and content type
    if (contentLength && isValidContentType)
    {
        // Check if there is enough to OTA Update
        bool canBegin = Update.begin(contentLength);

        // If yes, begin
        if (canBegin)
        {
#ifdef WIFI_DEBUG
           SerialPort.println("Begin OTA. This may take 2 - 5 mins to complete. Things might be quite for a while.. Patience!");
#endif
            // No activity would appear on the Serial monitor
            // So be patient. This may take 2 - 5mins to complete
            size_t written = Update.writeStream(client);

            if (written == contentLength)
            {
#ifdef WIFI_DEBUG
                SerialPort.println("Written : " + String(written) + " successfully");
#endif
            }
            else
            {
#ifdef WIFI_DEBUG
                SerialPort.println("Written only : " + String(written) + "/" + String(contentLength) + ". Retry?");
#endif
                // retry??
                // execOTA();
            }

            if (Update.end())
            {
                SerialPort.println("OTA done!");
                if (Update.isFinished())
                {
#ifdef WIFI_DEBUG
                    SerialPort.println("Update successfully completed. Rebooting.");
#endif
                    screen.ScreenViewReboot();
                    ESP.restart();
                }
                else
                {
#ifdef WIFI_DEBUG
                    SerialPort.println("Update not finished? Something went wrong!");
#endif
                }
            }
            else
            {
#ifdef WIFI_DEBUG
               SerialPort.println("Error Occurred. Error #: " + String(Update.getError()));
#endif
            }
        }
        else
        {
            // not enough space to begin OTA
            // Understand the partitions and
            // space availability
#ifdef WIFI_DEBUG
            SerialPort.println("Not enough space to begin OTA");
#endif
            client.flush();
        }
    }
    else
    {
#ifdef WIFI_DEBUG
        SerialPort.println("There was no content in the response");
#endif
        client.flush();
    }
}

//************************************
bool esp32FOTA2::execHTTPexist()
//************************************
{

	String useURL;

	if (useDeviceID)
	{
			// String deviceID = getDeviceID() ;
		useURL = checkURL + "?id=" + getDeviceID();
	}
	else
	{
		useURL = checkURL;
	}

	WiFiClient client;
	_port = 80;

#ifdef WIFI_DEBUG
	SerialPort.println("Getting HTTP");
	SerialPort.println(useURL);
	SerialPort.println("------");
#endif
	if ((WiFi.status() == WL_CONNECTED))
	{ //Check the current connection status

		HTTPClient http;

		http.begin(useURL);        //Specify the URL
		int httpCode = http.GET(); //Make the request

		if (httpCode > 0)
		{ //Check is a file was returned
	
#ifdef WIFI_DEBUG
		  SerialPort.printf("[HTTP] GET... code: %d\n", httpCode);
#endif

			// file found at server
			if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
				String payload = http.getString();
#ifdef WIFI_DEBUG
				SerialPort.println(payload);
#endif
			}
			
			http.end(); //Free the resources
			return true;
		}
		else
		{
#ifdef WIFI_DEBUG
			SerialPort.println("Error on HTTP request");
      SerialPort.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
#endif
			http.end(); //Free the resources
			return false;
		}
	}
	return false;
}

//***********************************
bool esp32FOTA2::execHTTPSexist()
//***********************************
{

	String useURL;

	if (useDeviceID)
	{
			// String deviceID = getDeviceID() ;
		useURL = checkURL + "?id=" + getDeviceID();
	}
	else
	{
		useURL = checkURL;
	}

	WiFiClient client;
	_port = 443;

#ifdef WIFI_DEBUG
	SerialPort.println("Getting HTTPS");
	SerialPort.println(useURL);
	SerialPort.println("------");		
#endif
	
	if ((WiFi.status() == WL_CONNECTED))
	{ //Check the current connection status

		WiFiClientSecure *client = new WiFiClientSecure;
		if(!client) {
#ifdef WIFI_DEBUG
			SerialPort.println("Unable to create client");
#endif
			return false;
		}
		
		client -> setCACert(rootCACertificate);

		HTTPClient https;

		https.begin(*client, useURL);        //Specify the URL
		int httpCode = https.GET(); //Make the request

		if (httpCode > 0)
		{ //Check is a file was returned
	
#ifdef WIFI_DEBUG
		  SerialPort.printf("[HTTPS] GET... code: %d\n", httpCode);
#endif

			// file found at server
			if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
				String payload = https.getString();
				SerialPort.println(payload);
			}
	
			https.end(); //Free the resources
			return true;
		}
		else
		{
#ifdef WIFI_DEBUG
			SerialPort.println("Error on HTTP request");
      SerialPort.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
#endif
			https.end(); //Free the resources
			return false;
		}
	}
	return false;
}

//************************************
uint8_t esp32FOTA2::execHTTPcheck(bool betaVersion)
//************************************
{

    String useURL;

    if (useDeviceID)
    {
        // String deviceID = getDeviceID() ;
        useURL = checkURL + "?id=" + getDeviceID();
    }
    else
    {
        useURL = checkURL;
    }

    WiFiClient client;
    _port = 80;

#ifdef WIFI_DEBUG
    SerialPort.println("Getting HTTP");
    SerialPort.println(useURL);
    SerialPort.println("------");
#endif
    if ((WiFi.status() == WL_CONNECTED))
    { //Check the current connection status

        HTTPClient http;

        http.begin(useURL);        //Specify the URL
        int httpCode = http.GET(); //Make the request

        if (httpCode == 200)
        { //Check is a file was returned

					String payload = http.getString();

					int str_len = payload.length() + 1;
					char JSONMessage[str_len];
					payload.toCharArray(JSONMessage, str_len);

#ifdef WIFI_DEBUG
					SerialPort.print("Parsing size : ");
					SerialPort.println(str_len);
//					SerialPort.println(JSONMessage);
#endif

					StaticJsonDocument<1300> JSONDocument; //Memory pool
					DeserializationError err = deserializeJson(JSONDocument, JSONMessage);

					if (err)
					{ //Check for errors in parsing
#ifdef WIFI_DEBUG
							SerialPort.println("Parsing failed");
#endif
							delay(5000);
							return MAJ_ERROR;
					}


					String tmp;
					tmp = _firwmareType;
					if (betaVersion) tmp = tmp + "b";
#ifdef WIFI_DEBUG
						SerialPort.print("Update Firmware : ");
						SerialPort.println(tmp);
#endif
					
					if (JSONDocument.containsKey(tmp)) {
						JsonObject JSONDocumentUpdate = JSONDocument[tmp];

//           const char *pltype = JSONDocument["type"];
						int plversion     = JSONDocumentUpdate["version"];				
						UpdateVersion = plversion;						
						
#ifdef WIFI_DEBUG
						SerialPort.print("Version : ");		
						SerialPort.println(plversion);
#endif

						int plsubversion  = JSONDocumentUpdate["subversion"];
						UpdateSubVersion = plsubversion;
						
#ifdef WIFI_DEBUG
						SerialPort.print("Sub Version : ");
						SerialPort.println(plsubversion);
#endif
						
						int plbetaversion = JSONDocumentUpdate["betaversion"];
						UpdateBetaVersion = plbetaversion;	
						
#ifdef WIFI_DEBUG
						SerialPort.print("Beta Version : ");
						SerialPort.println(plbetaversion);
#endif
						
						const char *plhost = JSONDocumentUpdate["host"];
						
						_port = JSONDocumentUpdate["port"];
#ifdef WIFI_DEBUG
						SerialPort.print("Port : ");
						SerialPort.println(_port);
#endif
						
						const char *plbin = JSONDocumentUpdate["bin"];

						String jshost(plhost);
						String jsbin(plbin);

						_host = jshost;
						_bin = jsbin;

#ifdef WIFI_DEBUG
						SerialPort.print("Host : ");
						SerialPort.println(_host);

						SerialPort.print("Bin : ");
						SerialPort.println(_bin);

						SerialPort.print("Firmware Version : ");
						SerialPort.println(_firwmareVersion);

						SerialPort.print("Firmware Sub Version : ");
						SerialPort.println(_firwmareSubVersion);

						SerialPort.print("Firmware Beta Version : ");
						SerialPort.println(_firwmareBetaVersion);
#endif

//						String fwtype(pltype);

						if ((betaVersion) && (_firwmareBetaVersion == 0))    
						{
							//mise à jour betaVersion sur une version stable
                            SerialPort.println("MAJ CAS 1");
							http.end(); //Free the resources
							return MAJ_AVAILABLE;
						}		
						else if ((!betaVersion) && (_firwmareBetaVersion > 0))    
						{
							//mise à jour version stabble sur une version beta
                            SerialPort.println("MAJ CAS 2");
							http.end(); //Free the resources
							return MAJ_AVAILABLE;
						}					

						else if (plversion > _firwmareVersion)    // && fwtype == _firwmareType)
						{
                            SerialPort.println("MAJ CAS 3");
							http.end(); //Free the resources
							return MAJ_AVAILABLE;
						}
						else if ((plversion == _firwmareVersion) && (plsubversion > _firwmareSubVersion))
						{
                            SerialPort.println("MAJ CAS 4");
							http.end(); //Free the resources
							return MAJ_AVAILABLE;
						}
						else if ((plversion == _firwmareVersion) && (plsubversion == _firwmareSubVersion) && (plbetaversion > _firwmareBetaVersion))
						{
                            SerialPort.println("MAJ CAS 5");
							http.end(); //Free the resources
							return MAJ_AVAILABLE;
						}
						else 
						{
                            SerialPort.println("MAJ MAJ_NOTAVAILABLE");
							http.end(); //Free the resources
							return MAJ_NOTAVAILABLE;
						}
					}	
					else
					{
#ifdef WIFI_DEBUG
            SerialPort.println("Erreur info firmware non trouvé");
#endif
						http.end(); //Free the resources
            return MAJ_ERROR;
					}
        }

        else
        {
#ifdef WIFI_DEBUG
            SerialPort.println("Error on HTTP request");
#endif
					http.end(); //Free the resources
          return MAJ_ERROR;
        }

        http.end(); //Free the resources
        return MAJ_ERROR;
    }
    return MAJ_ERROR;
}

//************************************
uint8_t esp32FOTA2::execHTTPScheck(bool betaVersion)
//************************************
{

    String useURL;

    if (useDeviceID)
    {
        // String deviceID = getDeviceID() ;
        useURL = checkURL + "?id=" + getDeviceID();
    }
    else
    {
        useURL = checkURL;
    }

    WiFiClient client;
    _port = 443;

#ifdef WIFI_DEBUG
    SerialPort.println("Getting HTTPS");
    SerialPort.println(useURL);
    SerialPort.println("------");		
#endif
		
    if ((WiFi.status() == WL_CONNECTED))
    { //Check the current connection status

			WiFiClientSecure *client = new WiFiClientSecure;
			if(!client) {
#ifdef WIFI_DEBUG
				SerialPort.println("Unable to create client");
#endif
				return false;
			}
				client -> setCACert(rootCACertificate);

        HTTPClient https;

        https.begin(*client, useURL);        //Specify the URL
        int httpCode = https.GET(); //Make the request

        if (httpCode == 200)
        { //Check is a file was returned

            String payload = https.getString();

            int str_len = payload.length() + 1;
            char JSONMessage[str_len];
            payload.toCharArray(JSONMessage, str_len);

            StaticJsonDocument<1300> JSONDocument; //Memory pool
            DeserializationError err = deserializeJson(JSONDocument, JSONMessage);

            if (err)
            { //Check for errors in parsing
#ifdef WIFI_DEBUG
                SerialPort.println("Parsing failed");
#endif
                delay(5000);
                return false;
            }

            const char *pltype = JSONDocument["type"];
            int plversion = JSONDocument["version"];
            const char *plhost = JSONDocument["host"];
            _port = JSONDocument["port"];
            const char *plbin = JSONDocument["bin"];

            String jshost(plhost);
            String jsbin(plbin);

            _host = jshost;
            _bin = jsbin;

            String fwtype(pltype);

            if (plversion > _firwmareVersion && fwtype == _firwmareType)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        else
        {
#ifdef WIFI_DEBUG
            SerialPort.println("Error on HTTP request");
#endif
            return false;
        }

        https.end(); //Free the resources
        return false;
    }
    return false;
}


//************************************
String esp32FOTA2::getDeviceID()
//************************************
{
    char deviceid[21];
    uint64_t chipid;
    chipid = ESP.getEfuseMac();
    sprintf(deviceid, "%" PRIu64, chipid);
    String thisID(deviceid);
    return thisID;
}

// Force a firmware update regartless on current version
//************************************
bool esp32FOTA2::forceUpdate(String firwmareHost, int firwmarePort, String firwmarePath)
//************************************
{
    _host = firwmareHost;
    _bin = firwmarePath;
    _port = firwmarePort;
    execOTA();

    return true;
}


//************************************
String esp32FOTA2::getHTTPVersion()
//************************************
{
	String output;

	output = "{\n";
	
	output += "\"Firmware\":{\n";
	output += "\"type\": " + String(VARIOSCREEN_SIZE) + ",\n";
	output += "\"version\": " + String(_firwmareVersion) + ",\n";
	output += "\"subversion\": " + String(_firwmareSubVersion) + ",\n";
	output += "\"betaversion\": " + String(_firwmareBetaVersion) + "\n}";

	uint8_t updatedNeeded = execHTTPcheck(false);
		
  if (updatedNeeded >= 0)
  {
#ifdef WIFI_DEBUG
		SerialPort.println("************** Version Stable : Mise à jour disponible *****************");
		SerialPort.print("Version : "); 
		SerialPort.print(UpdateVersion);
		SerialPort.print(".");
		SerialPort.println(UpdateSubVersion);
#endif

	  output += ",\n\""+_firwmareType+"\":{\n";
		output += "\"type\": " + String(VARIOSCREEN_SIZE) + ",\n";
		output += "\"version\": " + String(UpdateVersion) + ",\n";
		output += "\"subversion\": " + String(UpdateSubVersion) + ",\n";
		output += "\"betaversion\": 0\n}";
  }
  else
  {
   output = "";
	 return output;
  }


  updatedNeeded = execHTTPcheck(true);
  if (updatedNeeded >= 0)
  {
#ifdef WIFI_DEBUG
		SerialPort.println("***************** Version Beta : Mise à jour disponible *********************");
		SerialPort.print("Version : "); 
		SerialPort.print(UpdateVersion);
		SerialPort.print(".");
		SerialPort.print(UpdateSubVersion);
		SerialPort.print("b");
		SerialPort.println(UpdateBetaVersion);
#endif
	 
		output += ",\n\""+_firwmareType+"b\":{\n";
		output += "\"type\": " + String(VARIOSCREEN_SIZE) + ",\n";
		output += "\"version\": " + String(UpdateVersion) + ",\n";
		output += "\"subversion\": " + String(UpdateSubVersion) + ",\n";
		output += "\"betaversion\": " + String(UpdateBetaVersion) + "\n}";
  }
  else
  {
		output = "";
		return output;
  }

	
	output += "\n}";
	
  return output;
}

/*{
  "Gnuvario154":{
    "version": 0,
		"subversion": 7,
		"betaversion": 0,
    "host": "http://gnuvario-e.yj.fr",
    "port": 80,
    "bin": "/update/Gnuvario154.bin"
	},
  "Gnuvario154b":{
    "version": 0,
		"subversion": 7,
		"betaversion": 8,
    "host": "http://gnuvario-e.yj.fr",
    "port": 80,
    "bin": "/update/Gnuvario154b.bin"
	},*/

//************************************
String esp32FOTA2::getHTTPSVersion()
//************************************
{
	String output;
	return output;
}
