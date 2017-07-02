# MQTT relay
Yet another custom code to control Sonoff and other custom relay boards built around ESP8266 and ESP32.

The main goals are:
- Universal for Sonoff and other custom maed relay boards with ESP8266 and ESP32
- All setting is in one file so you have one file for each device but with completely different behaviour
- Button reading is interrupt based debounce, so you really don't miss a click even with delay used somewhere in code
- Button has a short press and long press
- You can define how many relays it has, also what is ON state wheter ```LOW``` or ```HIGH```
- Different modes state LED indication
- Remember last state during restart
- Clearly MQTT controlled, ready to be controlled by Home Automation server, like [HomeAssistant](https://home-assistant.io/)

Below are examples of HomeAssistant configuration

## Control relay
```yaml
switch:
  - platform: mqtt
    name: "sonoff_switch"
    command_topic: "sonoff/relay0"
    state_topic: "sonoff/relay0/state"
    payload_on: 'ON'
    payload_off: 'OFF'
    optimistic: false
    qos: 1
```

## Temperature with couple of DS18B20
```yaml
sensor:
  - platform: mqtt
    state_topic: "sonoff/temp0"
    name: "Tepmerature 1"
    unit_of_measurement: "°C"
  - platform: mqtt
    state_topic: "sonoff/temp1"
    name: "Tepmerature 2"
    unit_of_measurement: "°C"
```

## Temperature and humidity with DHT22
```yaml
sensor:
  - platform: mqtt
    state_topic: "sonoff/temp"
    name: "Tepmerature"
    unit_of_measurement: "°C"
  - platform: mqtt
    state_topic: "sonoff/humidity"
    name: "Humidity"
    unit_of_measurement: "%"
```