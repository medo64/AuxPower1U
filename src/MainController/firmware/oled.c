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

void oled_fillNumber1(char* text, uint16_t value) {
    if (value >= 10) {
        text[0] = '^';
    } else {
        text[0] = (uint8_t)('0' + value);
    }
}

void oled_fillNumber2(char* text, uint16_t value, bool prefixWithZero) {
    if (value >= 100) {
        text[0] = '^';
        text[1] = '^';
    } else if (value >= 10) {
        text[0] = (uint8_t)('0' + value / 10);
        text[1] = (uint8_t)('0' + value % 10);
    } else {
        text[0] = prefixWithZero ? '0' : ' ';
        text[1] = (uint8_t)('0' + value);
    }
}

void oled_fillNumber3(char* text, uint16_t value, bool prefixWithZero) {
    if (value >= 1000) {
        text[0] = '^';
        text[1] = '^';
        text[2] = '^';
    } else if (value >= 100) {
        text[0] = (uint8_t)('0' + value / 100);
        text[1] = (uint8_t)('0' + (value / 10) % 10);
        text[2] = (uint8_t)('0' + value % 10);
    } else if (value >= 10) {
        text[0] = prefixWithZero ? '0' : ' ';
        text[1] = (uint8_t)('0' + value / 10);
        text[2] = (uint8_t)('0' + value % 10);
    } else {
        text[0] = prefixWithZero ? '0' : ' ';
        text[1] = prefixWithZero ? '0' : ' ';
        text[2] = (uint8_t)('0' + value);
    }
}

void oled_writeSummary(uint16_t voltage1, uint16_t current1, uint16_t voltage2, uint16_t current2, uint16_t voltage3, uint16_t current3, uint16_t voltage4, uint16_t current4, uint16_t voltage5, uint16_t current5, uint16_t temperature) {
    uint32_t power = 0;
    power += (uint32_t)voltage1 * (uint32_t)current1;
    power += (uint32_t)voltage2 * (uint32_t)current2;
    power += (uint32_t)voltage3 * (uint32_t)current3;
    power += (uint32_t)voltage4 * (uint32_t)current4;
    power += (uint32_t)voltage5 * (uint32_t)current5;
    uint16_t powerWhole = (uint16_t)(power / (uint32_t)1000000);

    char line1[17];
    oled_fillNumber2(&line1[0], voltage1 / 1000, false);
    line1[2] = ' ';
    oled_fillNumber2(&line1[3], voltage2 / 1000, false);
    line1[5] = ' ';
    oled_fillNumber2(&line1[6], voltage3 / 1000, false);
    line1[8] = ' ';
    oled_fillNumber2(&line1[9], voltage4 / 1000, false);
    line1[11] = ' ';
    oled_fillNumber2(&line1[12], voltage5 / 1000, false);
    line1[14] = ' ';
    line1[15] = 'V';
    line1[16] = 0;

    char line2[17];
    oled_fillNumber2(&line2[0], temperature / 10, false);
    line2[2] = 0xF8;
    line2[3] = 'C';
    line2[4] = ' ';
    line2[5] = powerWhole >= 100 ? 0xB0 : ' ';
    line2[6] = powerWhole >= 200 ? 0xB1 : ' ';
    line2[7] = powerWhole >= 300 ? 0xB2 : ' ';
    line2[8] = powerWhole >= 400 ? 0xDB : ' ';
    line2[9] = powerWhole >= 500 ? 0xDB : ' ';
    line2[10] = ' ';
    oled_fillNumber3(&line2[11], powerWhole, false);
    line2[14] = ' ';
    line2[15] = 'W';
    line2[16] = 0;

    ssd1306_moveTo(1, 1);
    ssd1306_writeText16(line1);
    ssd1306_moveToNextRow16();
    ssd1306_clearRemaining();
    ssd1306_moveToNextRow();
    ssd1306_writeText(line2);
}

void oled_writeTest0(uint16_t voltage, uint16_t temperature) {
    ssd1306_moveTo(1, 1);
    ssd1306_writeText16("      TEST      ");

    char line[17];
    line[0] = ' ';
    oled_fillNumber2(&line[1], voltage / 1000, false);
    line[3] = '.';
    oled_fillNumber3(&line[4], voltage % 1000, true);
    line[7] = 'V';
    line[8] = ' ';
    oled_fillNumber2(&line[9], temperature / 10, false);
    line[11] = '.';
    oled_fillNumber1(&line[12], temperature % 10);
    line[13] = 0xF8;
    line[14] = 'C';
    line[15] = ' ';
    line[16] = 0;
    ssd1306_moveToNextRow16();
    ssd1306_writeText16(line);
}

void oled_writeTestX(uint8_t index, uint16_t voltage, uint16_t current) {
    char header[17] = "     TEST X     ";
    header[10] = 0x30 + index;
    ssd1306_moveTo(1, 1);
    ssd1306_writeText16(header);

    char line[17];
    line[0] = ' ';
    oled_fillNumber2(&line[1], voltage / 1000, false);
    line[3] = '.';
    oled_fillNumber3(&line[4], voltage % 1000, true);
    line[7] = 'V';
    line[8] = ' ';
    oled_fillNumber1(&line[9], current / 1000);
    line[10] = '.';
    oled_fillNumber3(&line[11], current % 1000, true);
    line[14] = 'A';
    line[15] = ' ';
    line[16] = 0;
    ssd1306_moveToNextRow16();
    ssd1306_writeText16(line);
}
