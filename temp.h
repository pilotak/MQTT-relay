#if defined(TEMP_DHT)
  #include "DHT.h"
  DHT dht(temperature_pin, TEMP_DHT);
   
#elif defined(TEMP_DS18B20)
  #include <OneWire.h>
  #include <DallasTemperature.h>
  
  OneWire oneWire(temperature_pin);
  DallasTemperature sensors(&oneWire);
  DeviceAddress tempDeviceAddress;

  uint32_t lastTempRequest;
  bool reading_temp = false;
#endif

volatile bool request_temp = false;

void triggerRequestTemp(){
  request_temp = true;
}

void setupTemp(){
  #if defined(TEMP_DS18B20)
    #if defined(DEBUG)
      Serial.println(F("Locating DS18B20s..."));
    #endif
    
    sensors.begin();
    sensors.setResolution(12);
    sensors.setWaitForConversion(false);

    #if defined(DEBUG)
      Serial.print(F("Found: "));
      Serial.println(sensors.getDeviceCount(), DEC);
    #endif
  #endif
}


void requestTemp(){
  request_temp = false;
  
  #if defined(TEMP_DHT)
    float temp, humidity;
    
    temp = dht.readTemperature();
    humidity = dht.readHumidity();

    #if defined(DEBUG)
      Serial.print(F("Temperature: "));
      Serial.println(temp, 2);
      Serial.print(F("Humidity: "));
      Serial.println(humidity, 2);
    #endif

    if(mqtt_connected){
      if (!isnan(temp)) {
        sendTempHumidity(temp, 0);
      }
  
      if(!isnan(humidity)){
        sendTempHumidity(humidity, 1);
      }
    }
  
  #elif defined(TEMP_DS18B20)
    sensors.setWaitForConversion(false);
    sensors.requestTemperatures();
    lastTempRequest = millis(); 
    reading_temp = true;
  #endif

  
}

#if defined(TEMP_DS18B20)
void processTemp(){
  reading_temp = false;
  
  for(uint8_t i = 0; i < sensors.getDeviceCount(); i++){
    float temp = sensors.getTempCByIndex(i);
    
    #if defined(DEBUG)
      Serial.print(F("Temperature: "));
      Serial.println(temp, 2);
    #endif
    if(mqtt_connected && (temp != 85.0 || temp != -127.0)) sendTempHumidity(temp, i);
  }
}
#endif

