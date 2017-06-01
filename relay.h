bool set_relay(uint8_t relay, bool state){
  if(relay <= NO_OF_RELAYS){
    #if defined(DEBUG)
      Serial.print(F("Turning relay"));
      Serial.print(relay);
      Serial.print(F(": "));
      Serial.println((state ? "ON" : "OFF"));
    #endif
    
    if(!relay_inverted[relay]) digitalWrite(relay_pin[relay], state);
    else digitalWrite(relay_pin[relay], !state);

    #if defined(RELAY_HAS_LED)
      digitalWrite(relay_led_pin[relay], state);
    #endif

    relay_state[relay] = state;

    if(remember_relay_state[relay]){
      #if !defined(ESP32)
        EEPROM.write(relay, state);
        EEPROM.commit();
      #endif
    }
    
    if(mqtt_connected) sendRelayState(relay, state);

    return true;
  }
  return false;
}

void setupRelays(){
  for(uint8_t i = 0; i < NO_OF_RELAYS; i++){
    pinMode(relay_pin[i], OUTPUT);

    #if defined(RELAY_HAS_LED)
      pinMode(relay_led_pin[i], OUTPUT);
    #endif
    
    delay(1); // give a time to pinMode

    if(remember_relay_state[i]) {
      #if !defined(ESP32)
        set_relay(i, (bool)EEPROM.read(i));
      #else
        set_relay(i, relay_inverted[i]);
      #endif
    }
    else {
      set_relay(i, relay_inverted[i]);
    }
  }
}

