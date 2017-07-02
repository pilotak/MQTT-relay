#if !defined(ESP32)
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
  #include <EEPROM.h>
  #include <Ticker.h>
#else
  #include <WiFi.h>
  #include <ESPmDNS.h>
  
  hw_timer_t * blinkTimer = NULL;
  volatile SemaphoreHandle_t makeBlink;
  portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
#endif

#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>

#include "setting.h"

WiFiClient espClient;
PubSubClient mqtt(espClient, MQTT_SERVER, MQTT_PORT);

#if !defined(ESP32)
Ticker blinkTimer, tempTimer, blinkAliveTimer, blinkAliveTimeoutTimer;
#endif

bool set_relay(uint8_t relay, bool state); // Compiler workaround
bool relay_state[NO_OF_RELAYS];
bool mqtt_connected = false;
bool wifi_connected = false;

#if defined(HAS_TEMP)
  void setupTemp(); // Compiler workaround
#endif

#if defined(HAS_STATUS_LED)
  #include "state_led.h"
#endif

#if defined(HAS_BUTTON)
  #include "button.h"
#endif

#include "wifi.h"
#include "mqtt.h"
#include "relay.h"

#if defined(HAS_TEMP)
  #include "temp.h"
#endif

void setup() {
  #if defined(DEBUG)
    Serial.begin(115200);
    Serial.print(F("\n\nBooting "));
    Serial.println(DEVICE_NAME);
  #endif

  #if !defined(ESP32)
  EEPROM.begin(NO_OF_RELAYS*2);
  #endif

  setupRelays();

  #if defined(HAS_STATUS_LED)
    setupStatusLed();
  #endif

  #if defined(HAS_BUTTON)
    pinMode(button_pin, INPUT);
    attachInterrupt(button_pin, buttonFall, FALLING);
  #endif
  
  #if defined(HAS_TEMP)
    setupTemp();
    #if defined(TEMP_DS18B20)
      uint32_t interval = TEMP_INTERVAL - DS18B20_WAIT_TIME;
    #elif defined(TEMP_DHT)
      uint32_t interval = TEMP_INTERVAL;
    #endif
    
    tempTimer.attach_ms(interval, triggerRequestTemp);
  #endif

  mqtt.set_callback(mqttCallback);
}

void loop() {
  #if defined(HAS_BUTTON)
    processButton();

    if(!reading_button){
  #endif
  
  if(wifiReconnect()) {
    ArduinoOTA.handle();
    mqtt_connected = mqttReconnect();

    if(mqtt_connected){
      mqtt.loop();
    }
  }
  #if defined(HAS_TEMP)
    if(request_temp) requestTemp();
    #if defined(TEMP_DS18B20)
    else if(reading_temp && millis() - lastTempRequest >= DS18B20_WAIT_TIME){
      processTemp();
    }
    #endif
  #endif

  #if defined(HAS_BUTTON)
    }
  #endif
}

