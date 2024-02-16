#ifndef ESP8266
  #error This code is designed to run on ESP8266 platform! Please check your Tools->Board setting.
#endif

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include <ESP8266TimerInterrupt.h>
#include <ESP8266_ISR_Timer.h>

#include "global.h"
#include "timer.h"
#include "network.h"
#include "screen.h"
#include "temperature_sensor.h"

float temp(NAN);
float hum(NAN);
float pres(NAN);

unsigned int current_timestamp = 0ul;

bool should_check_timestamp = false;
bool should_check_temperature = false;

bool is_light_on = false;
bool is_fan_on = false;

void setup() {
    Serial.begin(115200);
    while (!Serial) {}

    setup_network();
    setup_temperature_sensor();
    setup_timers();
    setup_screen();

    // get inital sensor readings and timestamp
    Serial.println("Initial sensor reading:");
    get_temperature();
    print_temp();

    Serial.println("Initial attempt to get a timestamp:");
    for (int i = 0; i < 5; i++) {
        Serial.print(".");
        if (get_timestamp_form_network()) break;
    }
    Serial.println();
    update_local_time();

    // pin and state setup
    pinMode(FAN_PIN, OUTPUT);
    pinMode(LIGHT_PIN_1, OUTPUT);
    pinMode(LIGHT_PIN_2, OUTPUT);

    update_relays();
}

void loop() {
    // check polling
    if (should_check_timestamp) {
        should_check_timestamp = false;
        get_timestamp_form_network();
    }

    if (should_check_temperature) {
        should_check_temperature = false;
        get_temperature();
        print_temp();
    }

    // update lights and fan on top of the hour
    if (tick_local_time()) {
        if (is_top_of_minute()) {
            update_relays();
        }
    }
}

void update_relays() {
    LocalTime time = get_local_time();
    is_light_on = (time.hour >= LIGHT_START_TIME && time.hour < LIGHT_END_TIME);
    is_fan_on = is_light_on ? temp > FAN_TEMP_LOW : temp > FAN_TEMP_HIGH;

    Serial.print("lights: ");
    Serial.print(is_light_on ? "ON" : "OFF");
    Serial.println("");

    Serial.print("fan: ");
    Serial.print(is_fan_on ? "ON" : "OFF");
    Serial.println("");

    // NOTE: low -> relay on
    digitalWrite(LIGHT_PIN_1, !is_light_on);
    digitalWrite(LIGHT_PIN_2, !is_light_on);
    digitalWrite(FAN_PIN,     !is_fan_on);
}

