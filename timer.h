#pragma once

// Select a Timer Clock
#define USING_TIM_DIV1                false           // for shortest and most accurate timer
#define USING_TIM_DIV16               false           // for medium time and medium accurate timer
#define USING_TIM_DIV256              true            // for longest timer but least accurate. Default

// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "ESP8266TimerInterrupt.h"
#include "ESP8266_ISR_Timer.hpp"

#include "global.h"

extern ESP8266_ISR_Timer ISR_Timer;  // declaration of the global variable ISRTimer

void setup_timers();
void start_fan();