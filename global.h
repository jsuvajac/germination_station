#pragma once

#include "ssid.h"

#define FAN_PIN 14
#define FAN_TIME 1000 * 60 * 3

#define LIGHT_PIN 12
#define LIGHT_START_TIME 7 // am
#define LIGHT_END_TIME (LIGHT_START_TIME + 16) // hours

#define BASE_POLL_TIME 1000 * 10
#define TIMESTAMP_POLL_TIME 1000 * 5
#define TEMPERATURE_POLL_TIME 1000 * 10

#define UDP_LOCAL_PORT 2390
#define TIMEZONE_OFFSET_FROM_GMT -5