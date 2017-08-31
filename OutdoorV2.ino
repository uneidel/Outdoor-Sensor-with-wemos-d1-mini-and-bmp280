
#include <FS.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <DNSServer.h>            
#include <ESP8266WebServer.h>    
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <NtpClientLib.h>
#include <IRsend.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <ArduinoJson.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <PubSubClient.h>
#include <BME280I2C.h>
#include <Wire.h>  
#include <EEPROM.h>

#define clientName "outdoor"
const char* WILL_FEED="/devices";
const char* OUT_FEED="/sensors/outdoorout";
const char* IN_FEED="/sensors/outdoorin";
const char* STATUS_FEED = "/sensors/outdoorstatus";


const int maxcurtemp = 50;
bool shouldSaveConfig = false;


char mqtt_server[40] = "192.168.178.100";
char wakeup[6] = "2"; // in sec.

WiFiClient espClient;

ESP8266WebServer httpServer(1234);
ESP8266HTTPUpdateServer httpUpdater;
int curloop = 0;
PubSubClient pubsubclient(espClient);
BME280I2C bme;  

//EEPROM
int address = 0;


// CheckFrequeny
int CheckFrequency = 10;

  


