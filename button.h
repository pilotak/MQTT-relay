volatile bool make_short_press = false;
volatile long last_debounce_time = 0;
volatile bool read_long_press = false;
volatile bool reading_button = false;

void buttonRise();
void buttonFall();

void processButton(){
  if(reading_button && read_long_press && (millis() - last_debounce_time) >= BUTTON_LONG_PRESS){ // button is still pressed
    #if defined(DEBUG)
      Serial.println("\nLong press ");
    #endif
    
    read_long_press = false;
  }
  else if(make_short_press){
    #if defined(DEBUG)
      Serial.println("\nShort press");
    #endif
    
    make_short_press = false;

    set_relay(0, !relay_state[0]);
  }
}

void buttonRise(){
  unsigned long currentTime = millis();
  
  if ((currentTime - last_debounce_time) >= BUTTON_DEBOUNCE_TIME){
    last_debounce_time = currentTime;
    
    if (reading_button){
      reading_button = false;

      if(read_long_press) make_short_press = true;
      
      attachInterrupt(button_pin, buttonFall, FALLING);
    }
  }
}

void buttonFall(){
  unsigned long currentTime = millis();
  
  if ((currentTime - last_debounce_time) >= BUTTON_DEBOUNCE_TIME){
    last_debounce_time = currentTime;
    
    if (!reading_button) {
      reading_button = true;
      read_long_press = true;
      
      attachInterrupt(button_pin, buttonRise, RISING);
    }
  }
}
