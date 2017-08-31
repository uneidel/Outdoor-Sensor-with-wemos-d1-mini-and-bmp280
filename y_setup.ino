void setup() {
  Serial.begin(115200);  
  
  
   while(!bme.begin()){
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }
  //wifiManager.resetSettings();
  LoadSetup();

  // Just using EEPROM hoping faster than SPFISS
  EEPROM.begin(512);

  // currently using direct wifi since wifimanager has some problems with eeprom & deepsleep
  WiFi.begin("fritzbox", "tintenfische1");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  } 
  
  /*
  WiFiManager wifiManager;
   
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  WiFiManagerParameter custom_mqtt_server("mqtt", "MQTT Server", mqtt_server, 50);
  WiFiManagerParameter custom_measure_freq("freq", "Measure Freq.(sec)", wakeup, 50);
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_measure_freq);
  wifiManager.autoConnect("SetupHotSpot");
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  strcpy(mqtt_server, custom_mqtt_server.getValue());
  strcpy(wakeup, custom_measure_freq.getValue());
  Serial.print("WakeUp: ");
  Serial.println(wakeup);
  
   if (shouldSaveConfig) {
      saveConfig();
   }  
  */
  Serial.print("MQTT Server: ");
  Serial.println(mqtt_server);
  SetupPubSub();
  PubSubConnect();
  PublishMessage(GetMeasurement());
  
  //Check every 10 awake for messages
  if (true)
  {
    Serial.println("Checking for incoming messages");
    PubSubConnect();
    pubsubclient.loop();pubsubclient.loop();
    Serial.println("Messages checked");
    saveConfig();
  }
  //ESP.deepSleep(20e6); //20e6 euals 20*1000000

  Serial.print("going for sleep for ");Serial.print(atoi(wakeup)); Serial.println(" seconds");
  ESP.deepSleep(atoi(wakeup) * 1000000);
  


}


boolean CheckForSub()
{
   byte val = EEPROM.read(address);
   // int val = atoi(bval);
   boolean bret =false;
   if (val < CheckFrequency) { val++; }
   else{ val =0;bret = true;  }
   EEPROM.write(address, val);
   EEPROM.commit();
   return bret;
}


