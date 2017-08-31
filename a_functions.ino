void LoadSetup(){
if (SPIFFS.begin()) {
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial); //debug
        if (json.success()) {
          strcpy(mqtt_server, json["mqtt_server"]);  
          strcpy(wakeup,json["wakeup"]);         
        } 
        else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  
}

char * ConvertfloatString(float Value) {
     char charBuf[10];
     dtostrf(Value, 13,4, charBuf);
     return charBuf;
}


void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;

}
void saveConfig(){
 
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["mqtt_server"] = mqtt_server;
    Serial.print("Wakeup: ");Serial.println(wakeup);
    json["wakeup"] = wakeup;
   
    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close(); 
}
void SplitBySpace(String input, String& cmd, String& cmdval){
  for (int i = 0; i < input.length(); i++) {
    if (input.substring(i, i+1) == " ") {
      cmd = input.substring(0, i);
      cmdval= input.substring(i+1);
      break;
    }
  }
}

String GetCurrentTime(bool wait) {
  String datetimestring = "";
  datetimestring = NTP.getTimeDateString();
  if (wait){
    while (datetimestring == "Time not set"){
      datetimestring = NTP.getTimeDateString();
      delay(100);
    }
  }
  return datetimestring;
}


void SetupOTA(){
 
  MDNS.begin(clientName);
  httpUpdater.setup(&httpServer);
  httpServer.begin();
  MDNS.addService("http", "tcp", 1234);
}







