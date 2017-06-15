bool sendRelayState(uint8_t relay, bool state){
  char buffer[20];
  String topic = MQTT_RELAY_TOPIC;
  topic.concat(relay);
  topic.concat(MQTT_RELAY_TOPIC_STATE);

  if(state) {
    memcpy(buffer, MQTT_STATE_ON, sizeof(MQTT_STATE_ON));
  }
  else {
    memcpy(buffer, MQTT_STATE_OFF, sizeof(MQTT_STATE_OFF));
  }

  #if defined(DEBUG)
    Serial.print(F("Sending relay"));
    Serial.print(relay);
    Serial.print(F(" state: "));
    Serial.println((state ? MQTT_STATE_ON : MQTT_STATE_OFF));
  #endif
  
  return mqtt.publish(MQTT::Publish(topic, buffer).set_retain().set_qos(1));
}

bool sendSignalQuality(){
  int dBm = WiFi.RSSI();
  byte quality;
  if(dBm <= -100) quality = 0;
  else if(dBm >= -50) quality = 100;
  else quality = 2 * (dBm + 100);

  char buffer[4];
  snprintf(buffer, 4, "%u", quality );

  #if defined(DEBUG)
    Serial.print("Signal quality: ");
    Serial.println(buffer);
  #endif

  return mqtt.publish(MQTT::Publish(MQTT_SIGNAL_TOPIC_STATE, buffer).set_qos(1));
}

bool sendTempHumidity(float data, uint8_t type){
  char buffer[10];
  String topic;
  
  dtostrf(data, 7, 2, buffer);

  #if defined(TEMP_DHT)
    topic = (type == 0 ? MQTT_TEMP_TOPIC : MQTT_HUMIDITY_TOPIC);
  #elif defined(TEMP_DS18B20)
    topic = MQTT_TEMP_TOPIC;
    topic.concat(type);
  #endif
  return mqtt.publish(MQTT::Publish(topic, buffer).set_qos(1));
}

void mqttCallback(const MQTT::Publish& pub) {

  String topic = pub.topic();

  #if defined(DEBUG)
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("]: ");
    if(pub.payload_len() > 0) Serial.print(pub.payload_string());
    Serial.println();
  #endif

  if(topic == MQTT_OTA_TOPIC){
    uint32_t startTime = millis();
    uint32_t size = pub.payload_len();
    if (size == 0)
      return;

    #if defined(DEBUG)
      Serial.print(F("Receiving OTA of %u uint8_ts...\r\n"));
      Serial.println(size);
    #endif
    
    #if !defined(ESP32)
      Serial.setDebugOutput(true);
      if (ESP.updateSketch(*pub.payload_stream(), size, true, false)) {
        #if defined(DEBUG)
          Serial.println(F("Clearing retained message."));
        #endif
        mqtt.publish(MQTT::Publish(pub.topic(), "").set_retain());
        mqtt.disconnect();
        
        #if defined(DEBUG)
          Serial.print(F("Update Success: "));
          Serial.println(millis() - startTime);
          Serial.println(F("Rebooting..."));
        #endif
        ESP.restart();
        delay(10000);
      }
    #endif
  }
  else if(topic == MQTT_STATUS_TOPIC) {
    for(uint8_t i = 0; i < NO_OF_RELAYS; i++){
      sendRelayState(i, relay_state[i]);
    }
  }
  else if(topic == MQTT_SIGNAL_TOPIC){
    sendSignalQuality();
  }
  #if defined(TEMP_DS18B20)
  else if(topic == MQTT_TEMP_SEARCH_TOPIC){
      setupTemp();
  }
  #endif
  else if(topic.substring(0,topic.length()-1) == MQTT_RELAY_TOPIC ){
    uint8_t relay = topic.substring(topic.length()-1).toInt();

    String topic = pub.payload_string();

    if(topic == MQTT_STATE_ON) set_relay(relay, HIGH);
    else if(topic == MQTT_STATE_OFF) set_relay(relay, LOW);
  }
}

bool mqttReconnect(){
  static unsigned long lastReconnectAttempt = 0;
   if (!mqtt.connected()) {
    #if defined(HAS_STATUS_LED) && STATUS_LED_MODE == 1
        blink_enabled = true;
    #endif
    
    unsigned long now = millis();
    
    if (now - lastReconnectAttempt > MQTT_CONNECTION_INTERVAL) {
      #if defined(DEBUG)
        Serial.print(F("Connecting to MQTT: "));
      #endif
      lastReconnectAttempt = now;

      if (mqtt.connect(MQTT::Connect(DEVICE_NAME).set_auth(MQTT_USER, MQTT_PASSWORD))) {
        mqtt.subscribe(MQTT::Subscribe()
          .add_topic(MQTT_OTA_TOPIC, 2)
          .add_topic(MQTT_STATUS_TOPIC, 1)
          .add_topic(MQTT_SIGNAL_TOPIC, 1)
        );

        #if defined(TEMP_DS18B20)
          mqtt.subscribe(MQTT::Subscribe().add_topic(MQTT_TEMP_SEARCH_TOPIC, 1));
        #endif

        for(uint8_t i = 0; i < NO_OF_RELAYS; i++){
          String topic = MQTT_RELAY_TOPIC;
          topic.concat(i);
          mqtt.subscribe(MQTT::Subscribe().add_topic(topic, 1));
        }
        lastReconnectAttempt = 0;

        #if defined(DEBUG)
          Serial.println(F("OK"));
        #endif
      }
      else {
        #if defined(DEBUG)
          Serial.println(F("failed"));
        #endif
        return false;
      }
      return mqtt.connected();
    }
  }
  
  #if defined(HAS_STATUS_LED) && STATUS_LED_MODE == 1
      blink_enabled = false;
      digitalWrite(status_led_pin, (status_led_inverted ? LOW : HIGH));
  #endif
  
  return true; 
}

