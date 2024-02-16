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
unsigned int last_timestamp = 0ul;

bool should_check_timestamp = false;
bool should_check_temperature = false;

void setup() {
    Serial.begin(115200);
    while (!Serial) {}

    setup_network();
    setup_temperature_sensor();
    setup_timers();
    setup_screen();

    // pin setup
    pinMode(FAN_PIN, OUTPUT);
    pinMode(LIGHT_PIN_1, OUTPUT);
    pinMode(LIGHT_PIN_2, OUTPUT);
    digitalWrite(FAN_PIN, HIGH);
    digitalWrite(LIGHT_PIN_1, HIGH);
    digitalWrite(LIGHT_PIN_2, HIGH);

    // get inital sensor readings and timestamp
    Serial.println("Initial sensor reading:");
    get_temperature();
    print_temp();

    Serial.println("Initial attempt to get a timestamp:");
    for (int i = 0; i < 5; i++) {
        Serial.print(".");
        get_timestamp();
        if (current_timestamp != 0ul)
            break;
    }
    Serial.println();

}

void loop() {
    // check polling
    if (should_check_timestamp) {
        should_check_timestamp = false;
        get_timestamp();
    }

    if (should_check_temperature) {
        should_check_temperature = false;
        get_temperature();
        // print_temp();
    }

    // update lights and fan on top of the hour
    if (current_timestamp > last_timestamp) {
        int last_hour = get_timestamp_hour(last_timestamp);
        int current_hour = get_timestamp_hour(current_timestamp);

        // int last_minute = get_timestamp_minute(last_timestamp);
        // int current_minute = get_timestamp_minute(current_timestamp);

        if (last_hour != current_hour) {
            if (last_timestamp != 0ul) {
                // don't run initially
                start_fan();
            }

            // hour change
            if (current_hour >= LIGHT_START_TIME && current_hour < LIGHT_END_TIME) {
                digitalWrite(LIGHT_PIN_1, LOW); // on
                digitalWrite(LIGHT_PIN_2, LOW);
            }
            else {
                digitalWrite(LIGHT_PIN_1, HIGH); 
                digitalWrite(LIGHT_PIN_2, HIGH); 
            }
        }

        last_timestamp = current_timestamp;
    }

    measure_local_time();
}
