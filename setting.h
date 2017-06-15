#define DEBUG

/* ======= STATUS LED =======*/
#define HAS_STATUS_LED

#define BLINK_STATE_INTERVAL 250 // ms; when MQTT is not connected, led will flash in this interval
#define BLINK_ALIVE_INTERVAL 5000 // ms; when Wifi and MQTT not connected it will indicate in this interval that is alive
#define BLINK_ALIVE_TIMEOUT 100 // ms; how long will be the flash light

#define STATUS_LED_MODE 1 // 1 = when MQTT is not connected, led will flash, mqtt connected = led will light; 2 = only blinking that it is alive

const uint8_t status_led_pin = 13;
const bool status_led_inverted = true; // if true, LOW will make it light


/* ======= TEMPERATURE =======*/
#define HAS_TEMP
const uint8_t temperature_pin = 2;

#if defined(HAS_TEMP)
  #define TEMP_DS18B20
  #define DS18B20_WAIT_TIME 850 // ms
  
  //#define TEMP_DHT DHT22
  //#define TEMP_DHT DHT11
  
  #define TEMP_INTERVAL 10000 // ms // must be greater than DS18B20_WAIT_TIME if using DS18B20
#endif


/* ======= BUTTON =======*/
#define HAS_BUTTON
const uint8_t button_pin = 0;
#define BUTTON_LONG_PRESS 2000 // ms
#define BUTTON_DEBOUNCE_TIME 8 //ms


/* ======= WIFI =======*/
#define WIFI_SSID "***"
#define WIFI_PASSWORD "******"
#define WIFI_CONNECTION_TIMEOUT 20000 // ms; how long to wait for Wifi to connected


/* ======= MQTT =======*/
#define DEVICE_NAME "wifi-relay" // used for MQTT and OTA, must be unique like MAC address is
IPAddress MQTT_SERVER(192, 168, 0, 100);
#define MQTT_PORT 1883
#define MQTT_USER "***"
#define MQTT_PASSWORD "******"

#define MQTT_STATUS_TOPIC DEVICE_NAME "/status"
#define MQTT_RELAY_TOPIC DEVICE_NAME "/relay" // will result ie. wifi-relay/relay* where aterisk is relay number
#define MQTT_RELAY_TOPIC_STATE  "/state" // will result ie. wifi-relay/relay0/state
#define MQTT_OTA_TOPIC DEVICE_NAME "/ota" // will result ie. wifi-relay/ota
#define MQTT_TEMP_TOPIC DEVICE_NAME "/temp" // will result ie. wifi-relay/temp* where aterisk is temp index, or without number in case of DHT22/11
#define MQTT_HUMIDITY_TOPIC DEVICE_NAME "/humidity" // will result ie. wifi-relay/humidity
#define MQTT_TEMP_SEARCH_TOPIC DEVICE_NAME "/temp-search" // will result ie. wifi-relay/temp-search
#define MQTT_SIGNAL_TOPIC DEVICE_NAME "/signal" // will result ie. wifi-relay/signal
#define MQTT_SIGNAL_TOPIC_STATE MQTT_SIGNAL_TOPIC "/state" // will result ie. wifi-relay/signal/state
#define MQTT_STATE_ON "ON"
#define MQTT_STATE_OFF "OFF"

#define MQTT_CONNECTION_INTERVAL 5000 // ms; how long wait before trying to connect to MQTT again


/* ======= RELAY =======*/
#define RELAY_HAS_LED // wheter so assign LED to relay state
const uint8_t NO_OF_RELAYS  = 3;
const uint8_t relay_pin[NO_OF_RELAYS] = {13, 12, 16};
const bool relay_inverted[NO_OF_RELAYS] = {false, false, false};  // if true, LOW will make it turn on
const bool remember_relay_state[NO_OF_RELAYS] = {true, true, true}; // wheter to remember state after restart

#if defined(RELAY_HAS_LED)
  const uint8_t relay_led_pin[NO_OF_RELAYS] = {4, 5, 14};
#endif


