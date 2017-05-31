#define DEBUG

#define RELAY_HAS_LED
//#define HAS_STATUS_LED
#define HAS_TEMP
#define TEMP_DS18B20
#define DS18B20_WAIT_TIME 850 // ms
//#define TEMP_DHT DHT22
//#define TEMP_DHT DHT11

#define TEMP_INTERVAL 10000 // ms // must be greater than DS18B20_WAIT_TIME if using DS18B20

#define BLINK_ALIVE_INTERVAL 3000 // ms, when Wifi and MQTT not connected it will indicate that is alive

#define DEVICE_NAME "wifi-relay" // used for MQTT and OTA, must be unique like MAC address is
#define WIFI_SSID "xxx"
#define WIFI_PASSWORD "xxx"

IPAddress MQTT_SERVER(192, 168, 0, 100);
#define MQTT_PORT 1883
#define MQTT_USER "xxx"
#define MQTT_PASSWORD "xx"
#define MQTT_STATUS_TOPIC DEVICE_NAME "/status"
#define MQTT_RELAY_TOPIC DEVICE_NAME "/relay" // will result ie. wifi-relay/relay* where aterisk is relay number
#define MQTT_RELAY_TOPIC_STATE  "/state" // will result ie. wifi-relay/relay1/state
#define MQTT_OTA_TOPIC DEVICE_NAME "/ota"
#define MQTT_TEMP_TOPIC DEVICE_NAME "/temp" // will result ie. wifi-relay/temp* where aterisk is temp index, or without number in case of DHT22
#define MQTT_HUMIDITY_TOPIC DEVICE_NAME "/humidity" // will result ie. wifi-relay/humidity
#define MQTT_TEMP_SEARCH_TOPIC DEVICE_NAME "/temp-search" // will result ie. wifi-relay/temp-search

#define MQTT_STATE_ON "ON"
#define MQTT_STATE_OFF "OFF"

#define CONNECTION_INTERVAL 5000      // 5s
#define WIFI_CONNECTION_TIMEOUT 20000 // 20s
#define BLINK_STATE_INTERVAL 250 // ms


const uint8_t NO_OF_RELAYS  = 3;

const uint8_t temperature_pin = 2;

const uint8_t status_led_pin = 6;
const bool status_led_inverted = true;

const uint8_t relay_pin[NO_OF_RELAYS] = {13, 12, 16};
const uint8_t relay_led_pin[NO_OF_RELAYS] = {4, 5, 14};

const bool relay_inverted[NO_OF_RELAYS] = {true, false, false}; // NC, NO, NO
const bool remember_relay_state[NO_OF_RELAYS] = {true, true, true};


