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
    uint8_t powerPercent = (uint8_t)(powerWhole * 100 / 300);  // 300W seems as a realistic maximum

    char lineT[17];
    oled_fillNumber2(&lineT[0], voltage1 / 1000, false);
    lineT[2] = ' ';
    oled_fillNumber2(&lineT[3], voltage2 / 1000, false);
    lineT[5] = ' ';
    oled_fillNumber2(&lineT[6], voltage3 / 1000, false);
    lineT[8] = ' ';
    oled_fillNumber2(&lineT[9], voltage4 / 1000, false);
    lineT[11] = ' ';
    oled_fillNumber2(&lineT[12], voltage5 / 1000, false);
    lineT[14] = ' ';
    lineT[15] = 'V';
    lineT[16] = 0;

    char lineBL[6];
    oled_fillNumber2(&lineBL[0], temperature / 10, false);
    lineBL[2] = 0xF8;
    lineBL[3] = 'C';
    lineBL[4] = ' ';
    lineBL[5] = 0;

    char lineBR[7];
    lineBR[0] = ' ';
    oled_fillNumber3(&lineBR[1], powerWhole, false);
    lineBR[4] = ' ';
    lineBR[5] = 'W';
    lineBR[6] = 0;

    ssd1306_moveTo(1, 1);
    ssd1306_writeLine16(lineT);
    ssd1306_writeLine("                ");
    ssd1306_writeText(lineBL);
    ssd1306_writeProgress(5, powerPercent);
    ssd1306_writeText(lineBR);
}

void oled_writeTest0(uint16_t voltage, uint16_t temperature) {
    ssd1306_moveTo(1, 1);
    ssd1306_writeInverseLine16("      TEST      ");

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
    ssd1306_writeLine16(line);
}

void oled_writeTestX(uint8_t index, uint16_t voltage, uint16_t current) {
    char header[17] = "     TEST X     ";
    header[10] = 0x30 + index;
    ssd1306_moveTo(1, 1);
    ssd1306_writeInverseLine16(header);

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
    ssd1306_writeLine16(line);
}
