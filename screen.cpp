#include "screen.h"

#define LINE_LEN 17

extern float temp;
extern float hum;
extern float pres;
extern unsigned long current_timestamp;

char line_buffer[LINE_LEN] = {0};

void setup_screen() {
    ssd1306_128x64_i2c_init();
    ssd1306_setFixedFont(ssd1306xled_font8x16);
    ssd1306_clearScreen();
}

void print_temp() {
    ssd1306_clearBlock(0, 0, 128, 16 * 3);

    memset(line_buffer, 0, LINE_LEN);
    sprintf(line_buffer, " %.2f *C", temp);
    ssd1306_printFixed(0,  16 * 0, line_buffer, STYLE_NORMAL);

    memset(line_buffer, 0, LINE_LEN);
    sprintf(line_buffer, " %.2f %RH", hum);
    ssd1306_printFixed(0,  16 * 1, line_buffer, STYLE_NORMAL);

    memset(line_buffer, 0, LINE_LEN);
    sprintf(line_buffer, "%.2f hPa", pres);
    ssd1306_printFixed(0,  16 * 2, line_buffer, STYLE_NORMAL);
}

void print_time() {
    ssd1306_clearBlock(0, 16 * 3, 128, 16);
    memset(line_buffer, 0, LINE_LEN);

    int hours  = get_timestamp_hour(current_timestamp);
    int minutes = get_timestamp_minute(current_timestamp);

    line_buffer[0] = '0' + hours / 10;
    line_buffer[1] = '0' + hours % 10;
    line_buffer[2] = ':';
    line_buffer[3] = '0' + minutes / 10;
    line_buffer[4] = '0' + minutes % 10;

    ssd1306_printFixed(0,  16 * 3, line_buffer, STYLE_NORMAL);
}