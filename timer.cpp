#include "timer.h"

ESP8266Timer ITimer;
ESP8266_ISR_Timer ISR_Timer;

extern unsigned int current_timestamp;
extern unsigned int last_timestamp;
extern bool should_check_timestamp;
extern bool should_check_temperature;
extern bool is_fan_on;
extern float temp;

// timer for nudging local time approximation
unsigned int print_time_start = 0ul;
unsigned int print_time_current = 0ul;

// local approximation
unsigned int millis_at_timestamp = 0ul;
unsigned long raw_seconds;

LocalTime last_local_time;
LocalTime local_time;

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

bool tick_local_time() {
    if (millis() - print_time_current > 1000) {
        print_time_current = millis();

        raw_seconds = current_timestamp + (millis() - millis_at_timestamp) / 1000;

        LocalTime timestamp_time = LocalTime {
            (int) (current_timestamp % 60),
            (int) (current_timestamp / 60) % 60,
            (int) (current_timestamp / (60 * 60) + (24 + TIMEZONE_OFFSET_FROM_GMT)) % 24
        };
        last_local_time = local_time;
        local_time = LocalTime {
            (int) (raw_seconds % 60),
            (int) (raw_seconds / 60) % 60,
            (int) (raw_seconds / (60 * 60) + (24 + TIMEZONE_OFFSET_FROM_GMT)) % 24
        };

        Serial.print(local_time.hour);
        Serial.print(":");
        Serial.print(local_time.minute);
        Serial.print(":");
        Serial.print(local_time.second);

        Serial.print(" ( ");
        Serial.print(local_time.hour - last_local_time.hour);
        Serial.print(":");
        Serial.print(local_time.minute - last_local_time.minute);
        Serial.print(":");
        Serial.print(local_time.second - last_local_time.second);
        Serial.print(" ) ");


        Serial.print(" ( ");
        Serial.print(timestamp_time.hour);
        Serial.print(":");
        Serial.print(timestamp_time.minute);
        Serial.print(":");
        Serial.print(timestamp_time.second);
        Serial.println(" )");

        return true;
    }

    return false;
}

bool is_top_of_hour() {
    return (local_time.hour - last_local_time.hour);
}

LocalTime get_time() {
    return local_time;
}
