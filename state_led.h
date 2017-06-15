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
  #if STATUS_LED_MODE == 1
    if(!wifi_connected && !mqtt_connected) { 
  #elif STATUS_LED_MODE == 2
    blink_state = true;
  #endif
    digitalWrite(status_led_pin, (status_led_inverted ? LOW : HIGH));
  #if STATUS_LED_MODE == 1
    }
  #endif
}

void blinkAliveTimeout(){
  #if STATUS_LED_MODE == 1
    if(!wifi_connected && !mqtt_connected) {
  #elif STATUS_LED_MODE == 2
    if(blink_state){
      blink_state = false;
  #endif
    digitalWrite(status_led_pin, (status_led_inverted ? HIGH : LOW));
  }
}

void setupStatusLed(){
  pinMode(status_led_pin, OUTPUT);
  digitalWrite(status_led_pin, (status_led_inverted ? HIGH : LOW));
  #if !defined(ESP32)
    #if defined(BLINK_STATE_INTERVAL)
      blinkTimer.attach_ms(BLINK_STATE_INTERVAL, blinkState);
    #endif  
    blinkAliveTimeoutTimer.attach_ms(BLINK_ALIVE_TIMEOUT, blinkAliveTimeout);
    blinkAliveTimer.attach_ms(BLINK_ALIVE_INTERVAL, blinkAlive);
    
  #else
    #if defined(BLINK_STATE_INTERVAL)
      makeBlink = xSemaphoreCreateBinary();
      blinkTimer = timerBegin(0, 80, true);
      timerAttachInterrupt(blinkTimer, &blinkState, true);
      timerAlarmWrite(blinkTimer, BLINK_STATE_INTERVAL*1000, true);
      timerAlarmEnable(timer);
    #endif
  #endif

  blink_enabled = true;
}

