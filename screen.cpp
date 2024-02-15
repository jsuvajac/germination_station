#include "screen.h"

#define LINE_LEN 16
#define FONT_HEIGHT 16
#define FONT_WIDTH 8

extern float temp;
extern float hum;
extern float pres;

char line_buffer[LINE_LEN] = {0};

void setup_screen() {
    ssd1306_128x64_i2c_init();
    ssd1306_setFixedFont(ssd1306xled_font8x16);
    ssd1306_clearScreen();
}

void print_temp() {
    ssd1306_clearBlock(0, 0, FONT_WIDTH * 9, FONT_HEIGHT * 2);

    int x = 3 * FONT_WIDTH;
    int y = 21 * 1;
    memset(line_buffer, 0, LINE_LEN);
    sprintf(line_buffer, "%.2f *C", temp);
    ssd1306_printFixed(x, y, line_buffer, STYLE_NORMAL);

    x = 3 * FONT_WIDTH;
    y = 21 * 2;
    memset(line_buffer, 0, LINE_LEN);
    sprintf(line_buffer, "%.2f %%RH", hum);
    ssd1306_printFixed(x, y, line_buffer, STYLE_NORMAL);
}
