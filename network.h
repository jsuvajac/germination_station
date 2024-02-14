#pragma once

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "global.h"


void setup_network();
void get_timestamp();
void send_NTP_packet(IPAddress &address);

int get_timestamp_hour(unsigned long timestamp);
int get_timestamp_minute(unsigned long timestamp);
int get_timestamp_second(unsigned long timestamp);
