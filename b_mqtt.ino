void PublishMessage(char* message)
{
  PubSubConnect();
  pubsubclient.publish(OUT_FEED,message);
}

char* GetMeasurement()
{
  float temp(NAN), hum(NAN), pres(NAN);
   bme.read(pres, temp, hum, true, B000); 
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["pres"] = String(pres);
  root["temp"] = String(temp);
  root["hum"] = String(hum);
  char buffer[256];
  root.printTo(buffer, sizeof(buffer));
  return buffer;
}
void PubSubConnect() {
  while (!pubsubclient.connected()) {
    if (pubsubclient.connect(clientName,WILL_FEED,1,false, clientName)) {
      
      pubsubclient.subscribe(IN_FEED,1);
      
      Serial.println("mqtt broker connected");
    } 
    else 
    {
      Serial.print("mqtt broker connection wififailed, rc=");
      Serial.print(pubsubclient.state());
      delay(1000);
    }
  }
}


void SetupPubSub(){
  pubsubclient.setServer(mqtt_server, 1883);
  pubsubclient.setCallback(pubsubCallback);
}


void pubsubCallback(char* topic, byte* payload, unsigned int length) {
  String json = String((char *)payload);
  Serial.print("Raw message: ");
  Serial.println(json);
  StaticJsonBuffer<400> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  //const char* messagetype  = root["type"];Serial.print("MessageType: ");  Serial.println(messagetype);
  //const char* content   = root["content"];
  String action = root["action"];
  String cmdval = root["cmdval"];
  Serial.print("action: ");Serial.println(action);
  Serial.print("cmdval: "); Serial.println(cmdval);
  bool retainedread = false;
  if (action== "measurefreq"){
    
     strcpy(wakeup,root["cmdval"]);
     Serial.println("Set MeasureFreq");
     retainedread = true;
     JsonObject& root = jsonBuffer.createObject();
     root["result"] = "success";
     char buffer[256];
     root.printTo(buffer, sizeof(buffer));
      PubSubConnect();
  pubsubclient.publish(STATUS_FEED,buffer);
  }
  if(action == "setota"){
    
     Serial.println("Set OTA");
     retainedread = true;
  }

  if(retainedread){
       // RetainedMessage Read publish empty 
       PubSubConnect();
       pubsubclient.publish(IN_FEED,"",true);
    
  }
}

