#pragma once

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "global.h"


void setup_network();
bool get_timestamp_form_network();
void send_NTP_packet(IPAddress &address);

