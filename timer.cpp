#include "timer.h"

ESP8266Timer ITimer;
ESP8266_ISR_Timer ISR_Timer;

extern bool should_check_timestamp;
extern bool should_check_temperature;
extern bool is_fan_on;
extern float temp;


void poll_time() {
    should_check_timestamp = true;
}

void poll_temperature() {
    should_check_temperature = true;
}


void end_fan() {
    digitalWrite(FAN_PIN, HIGH);
}
void start_fan() {
    if (temp > FAN_TEMP_LOW && temp <= FAN_TEMP_HIGH) {
        Serial.println("Starting fan");
        digitalWrite(FAN_PIN, LOW);
        ISR_Timer.setInterval(FAN_TIME, end_fan);
    }
    else {
        Serial.println("Fan not starting as the temperature is out of range");
    }
}


void IRAM_ATTR TimerHandler() {
    ISR_Timer.run();
}

void setup_timers() {
    if (ITimer.attachInterruptInterval(1000 * BASE_POLL_TIME, TimerHandler))
    {
        Serial.println(F("Starting ITimer OK"));
    }
    else
        Serial.println(F("Can't set ITimer. Select another freq. or timer"));

    ISR_Timer.setInterval(TIMESTAMP_POLL_TIME, poll_time);
    ISR_Timer.setInterval(TEMPERATURE_POLL_TIME, poll_temperature);
}
