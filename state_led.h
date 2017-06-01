bool blink_enabled = false;
volatile bool blink_state = false;

#if !defined(ESP32)
void blinkState(){
#else
void IRAM_ATTR blinkState(){
  xSemaphoreGiveFromISR(makeBlink, NULL);
#endif
  blink_state = !blink_state;

  if(blink_enabled) {
    digitalWrite(status_led_pin, (status_led_inverted ? !blink_state : blink_state));
  }
}

void blinkAlive(){
  if(!wifi_connected && !mqtt_connected) {
    digitalWrite(status_led_pin, (status_led_inverted ? LOW : HIGH));
  }
}

void blinkAliveTimeout(){
  if(!wifi_connected && !mqtt_connected){
    digitalWrite(status_led_pin, (status_led_inverted ? HIGH : LOW));
  }
}

void setupStatusLed(){
  pinMode(status_led_pin, OUTPUT);
  #if !defined(ESP32)
    blinkTimer.attach_ms(BLINK_STATE_INTERVAL, blinkState);
    blinkAliveTimeoutTimer.attach_ms(BLINK_ALIVE_TIMEOUT, blinkAliveTimeout);
    blinkAliveTimer.attach_ms(BLINK_ALIVE_INTERVAL, blinkAlive);
  #else
    makeBlink = xSemaphoreCreateBinary();
    blinkTimer = timerBegin(0, 80, true);
    timerAttachInterrupt(blinkTimer, &blinkState, true);
    timerAlarmWrite(blinkTimer, BLINK_STATE_INTERVAL*1000, true);
    timerAlarmEnable(timer);
  #endif
}

