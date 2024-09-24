#include <stdint.h>
#include "ssd1306.h"

void oled_init(void) {
    ssd1306_init();
}

void oled_splash(void) {
    ssd1306_writeText16("   AuxPower1U   ");
    ssd1306_moveToNextRow16();
    ssd1306_writeText("   medo64.com   ");
}

void oled_fillNumber2(char* text, uint16_t value) {
    if (value >= 100) {
        text[0] = '^';
        text[1] = '^';
    } else if (value >= 10) {
        text[0] = (uint8_t)('0' + value / 10);
        text[1] = (uint8_t)('0' + value % 10);
    } else {
        text[0] = ' ';
        text[1] = (uint8_t)('0' + value);
    }
}

void oled_fillNumber3(char* text, uint16_t value) {
    if (value >= 1000) {
        text[0] = '^';
        text[1] = '^';
        text[2] = '^';
    } else if (value >= 100) {
        text[0] = (uint8_t)('0' + value / 100);
        text[1] = (uint8_t)('0' + (value / 10) % 10);
        text[2] = (uint8_t)('0' + value % 10);
    } else if (value >= 10) {
        text[0] = ' ';
        text[1] = (uint8_t)('0' + value / 10);
        text[2] = (uint8_t)('0' + value % 10);
    } else {
        text[0] = ' ';
        text[1] = ' ';
        text[2] = (uint8_t)('0' + value);
    }
}

void oled_writeSummary(uint16_t voltage1, uint16_t voltage2, uint16_t voltage3, uint16_t voltage4,uint16_t voltage5, uint16_t current1, uint16_t current2, uint16_t current3, uint16_t current4,uint16_t current5, uint16_t temperature) {
    uint32_t power = 0;
    power += (uint32_t)voltage1 * (uint32_t)current1;
    power += (uint32_t)voltage2 * (uint32_t)current2;
    power += (uint32_t)voltage3 * (uint32_t)current3;
    power += (uint32_t)voltage4 * (uint32_t)current4;
    power += (uint32_t)voltage5 * (uint32_t)current5;
    uint16_t powerWhole = (uint16_t)(power / (uint32_t)1000000);

    char line1[16];
    oled_fillNumber2(&line1[0], voltage1 / 1000);
    line1[2] = ' ';
    oled_fillNumber2(&line1[3], voltage2 / 1000);
    line1[5] = ' ';
    oled_fillNumber2(&line1[6], voltage3 / 1000);
    line1[8] = ' ';
    oled_fillNumber2(&line1[9], voltage4 / 1000);
    line1[11] = ' ';
    oled_fillNumber2(&line1[12], voltage5 / 1000);
    line1[14] = ' ';
    line1[15] = 'V';

    char line2[16];
    oled_fillNumber2(&line2[0], temperature / 10);
    line2[2] = 0xF8;
    line2[3] = 'C';
    line2[4] = ' ';
    line2[5] = power >= 100 ? 0xB0 : ' ';
    line2[6] = power >= 200 ? 0xB1 : ' ';
    line2[7] = power >= 300 ? 0xB2 : ' ';
    line2[8] = power >= 400 ? 0xDB : ' ';
    line2[9] = power >= 500 ? 0xDB : ' ';
    line2[10] = ' ';
    oled_fillNumber3(&line2[11], powerWhole);
    line2[14] = ' ';
    line2[15] = 'W';

    ssd1306_moveTo(1, 1);
    ssd1306_writeText16(line1);
    ssd1306_moveToNextRow16();
    ssd1306_clearRemaining();
    ssd1306_moveToNextRow();
    ssd1306_writeText(line2);
}
