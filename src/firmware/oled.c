#include <stdbool.h>
#include <stdint.h>
#include "ssd1306.h"

bool oledDisplayisOn = false;

void oled_init(void) {
    ssd1306_init();
    oledDisplayisOn = true;
}

void oled_splash(void) {
    ssd1306_writeText16("    AuxPower    ");
    ssd1306_moveToNextRow16();
    ssd1306_moveToNextRow();
    ssd1306_writeText("   medo64.com   ");
}

void oled_displayOn(void) {
    if (oledDisplayisOn) { return; }
    ssd1306_init();
    ssd1306_displayOn();
    oledDisplayisOn = true;
}

void oled_displayOff(void) {
    if (!oledDisplayisOn) { return; }
    ssd1306_displayOff();
    oledDisplayisOn = false;
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

void oled_writeSummary(uint16_t voltage1, uint16_t current1, uint16_t voltage2, uint16_t current2, uint16_t voltage3, uint16_t current3, uint16_t voltage4, uint16_t current4, uint16_t voltage5, uint16_t current5, uint16_t temperature, uint8_t outputOnMask) {
    uint32_t power = 0;
    power += (uint32_t)voltage1 * (uint32_t)current1;
    power += (uint32_t)voltage2 * (uint32_t)current2;
    power += (uint32_t)voltage3 * (uint32_t)current3;
    power += (uint32_t)voltage4 * (uint32_t)current4;
    power += (uint32_t)voltage5 * (uint32_t)current5;
    uint16_t powerWhole = (uint16_t)((power + 500000) / (uint32_t)1000000);
    uint8_t powerPercent = (uint8_t)(powerWhole * 100 / 300);  // 300W seems as a realistic maximum

    char lineT1[3] = { 0, 0, 0 }, lineT2[3] = { 0, 0, 0 }, lineT3[3] = { 0, 0, 0 }, lineT4[3] = { 0, 0, 0 }, lineT5[3] = { 0, 0, 0 };
    oled_fillNumber2(&lineT1[0], (voltage1 + 500) / 1000, false);
    oled_fillNumber2(&lineT2[0], (voltage2 + 500) / 1000, false);
    oled_fillNumber2(&lineT3[0], (voltage3 + 500) / 1000, false);
    oled_fillNumber2(&lineT4[0], (voltage4 + 500) / 1000, false);
    oled_fillNumber2(&lineT5[0], (voltage5 + 500) / 1000, false);

    char lineBL[6];
    oled_fillNumber2(&lineBL[0], (temperature + 5) / 10, false);
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
    ssd1306_writeText16(lineT1);
    ssd1306_writeCharacter16(' ');
    ssd1306_writeText16(lineT2);
    ssd1306_writeCharacter16(' ');
    ssd1306_writeText16(lineT3);
    ssd1306_writeCharacter16(' ');
    ssd1306_writeText16(lineT4);
    ssd1306_writeCharacter16(' ');
    ssd1306_writeText16(lineT5);
    ssd1306_writeCharacter16(' ');
    ssd1306_writeCharacter16('V');
    ssd1306_moveToNextRow16();

    if ((outputOnMask & 0b00001) != 0) {
        ssd1306_writeCharacter(0xDF);
        ssd1306_writeCharacter(0xDF);
    } else {
        ssd1306_writeText("  ");
    }
    ssd1306_writeCharacter(' ');
    if ((outputOnMask & 0b00010) != 0) {
        ssd1306_writeCharacter(0xDF);
        ssd1306_writeCharacter(0xDF);
    } else {
        ssd1306_writeText("  ");
    }
    ssd1306_writeCharacter(' ');
    if ((outputOnMask & 0b00100) != 0) {
        ssd1306_writeCharacter(0xDF);
        ssd1306_writeCharacter(0xDF);
    } else {
        ssd1306_writeText("  ");
    }
    ssd1306_writeCharacter(' ');
    if ((outputOnMask & 0b01000) != 0) {
        ssd1306_writeCharacter(0xDF);
        ssd1306_writeCharacter(0xDF);
    } else {
        ssd1306_writeText("  ");
    }
    ssd1306_writeCharacter(' ');
    if ((outputOnMask & 0b10000) != 0) {
        ssd1306_writeCharacter(0xDF);
        ssd1306_writeCharacter(0xDF);
    } else {
        ssd1306_writeText("  ");
    }
    ssd1306_writeLine("  ");

    ssd1306_writeText(lineBL);
    ssd1306_writeProgress(5, powerPercent);
    ssd1306_writeLine(lineBR);
}

void oled_writeChannel(uint8_t channel, uint16_t voltage, uint16_t current, bool isOutputOn, uint8_t state, uint16_t ticks) {
    char textTL[9];
    oled_fillNumber2(&textTL[0], (uint16_t)(voltage / 1000), false);
    textTL[2] = '.';
    oled_fillNumber2(&textTL[3], (uint16_t)((voltage % 1000) / 10), true);
    textTL[5] = ' ';
    textTL[6] = 'V';
    textTL[7] = ' ';
    textTL[8] = 0;

    char textBL[9];
    oled_fillNumber2(&textBL[0], (uint16_t)(current / 1000), false);
    textBL[2] = '.';
    oled_fillNumber2(&textBL[3], (uint16_t)((current % 1000 ) / 10), true);
    textBL[5] = ' ';
    textBL[6] = 'A';
    textBL[7] = ' ';
    textBL[8] = 0;

    uint32_t power = (uint32_t)voltage * (uint32_t)current / (uint32_t)1000;  // mW
    char textBR[9];
    textBR[0] = ' ';
    oled_fillNumber3(&textBR[1], (uint16_t)(power / 1000), false);
    textBR[4] = '.';
    oled_fillNumber1(&textBR[5], (uint16_t)((power % 1000) / 100));
    textBR[6] = ' ';
    textBR[7] = 'W';
    textBR[8] = 0;

    char textTR[7];
    textTR[0] = ' ';
    if (state == 1) {  // DEPTH_DETAILS
        textTR[1] = (ticks >= 12) ? '.' : ' ';
        textTR[2] = (ticks >= 24) ? '.' : ' ';
        textTR[3] = (ticks >= 36) ? '.' : ' ';
        textTR[4] = (ticks >= 48) ? '.' : ' ';
        textTR[5] = (ticks >= 60) ? '.' : ' ';
    } else if (state == 2) {  // DEPTH_PENDING_RESET
        textTR[1] = (ticks >= 12) ? 'R' : 'r';
        textTR[2] = (ticks >= 24) ? 'E' : 'e';
        textTR[3] = (ticks >= 36) ? 'S' : 's';
        textTR[4] = (ticks >= 48) ? 'E' : 'e';
        textTR[5] = (ticks >= 60) ? 'T' : 't';
    } else if (state == 3) {  // DEPTH_PENDING_OFF
        textTR[1] = (ticks >= 12) ? 'P' : 'p';
        textTR[2] = (ticks >= 24) ? 'O' : 'o';
        textTR[3] = (ticks >= 36) ? 'W' : 'w';
        textTR[4] = (ticks >= 48) ? 'E' : 'e';
        textTR[5] = (ticks >= 60) ? 'R' : 'r';
    } else {
        textTR[1] = ' ';
        textTR[2] = ' ';
        textTR[3] = ' ';
        textTR[4] = ' ';
        textTR[5] = ' ';
    }
    textTR[6] = 0;

    ssd1306_moveTo(1, 1);
    ssd1306_writeText16(textTL);
    ssd1306_writeText16(textTR);
    if (isOutputOn) {
        ssd1306_writeInverseCharacter16(0xDD);
        ssd1306_writeInverseCharacter16(0x30 + channel);
    } else {
        ssd1306_writeCharacter16(0xB3);
        ssd1306_writeCharacter16(0x30 + channel);
    }
    ssd1306_moveToNextRow16();
    ssd1306_writeText16(textBL);
    ssd1306_writeText16(textBR);
}

void oled_writeReset(uint8_t channel, uint16_t voltage, uint16_t current, uint16_t ticks) {
    char text[14];
    text[0] = ' ';
    text[1] = 'R';
    text[2] = 'E';
    text[3] = 'S';
    text[4] = 'E';
    text[5] = 'T';
    text[6] = 'T';
    text[7] = 'I';
    text[8] = 'N';
    text[9] = 'G';
    text[10] = (ticks > 12) ? '.' : ' ';
    text[11] = (ticks > 36) ? '.' : ' ';
    text[12] = (ticks > 60) ? '.' : ' ';
    text[13] = 0;

    ssd1306_moveTo(1, 1);
    ssd1306_writeText16(text);
    ssd1306_writeCharacter16(' ');
    ssd1306_writeInverseCharacter16(0xDD);
    ssd1306_writeInverseCharacter16(0x30 + channel);
    ssd1306_moveToNextRow16();
    ssd1306_writeLine("                ");

    char textBL[9];
    oled_fillNumber2(&textBL[0], (uint16_t)(current / 1000), false);
    textBL[2] = '.';
    oled_fillNumber2(&textBL[3], (uint16_t)((current % 1000 ) / 10), true);
    textBL[5] = ' ';
    textBL[6] = 'A';
    textBL[7] = ' ';
    textBL[8] = 0;

    uint32_t power = (uint32_t)voltage * (uint32_t)current / (uint32_t)1000;  // mW
    char textBR[9];
    textBR[0] = ' ';
    oled_fillNumber3(&textBR[1], (uint16_t)(power / 1000), false);
    textBR[4] = '.';
    oled_fillNumber1(&textBR[5], (uint16_t)((power % 1000) / 100));
    textBR[6] = ' ';
    textBR[7] = 'W';
    textBR[8] = 0;

    ssd1306_writeText(textBL);
    ssd1306_writeText(textBR);
}


bool oled_testPolarity = false;

void oled_writeTestVT(uint8_t index, uint16_t voltage, uint16_t temperature) {
    oled_testPolarity = !oled_testPolarity;

    char header[17] = "     TEST X     ";
    if (oled_testPolarity) {
        header[0] = 0xB0;
        header[15] = 0xB0;
    }
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
    oled_fillNumber2(&line[9], temperature / 10, false);
    line[11] = '.';
    oled_fillNumber1(&line[12], temperature % 10);
    line[13] = 0xF8;
    line[14] = 'C';
    line[15] = ' ';
    line[16] = 0;
    ssd1306_writeLine16(line);
}

void oled_writeTestVC(uint8_t index, uint16_t voltage, uint16_t current) {
    oled_testPolarity = !oled_testPolarity;

    char header[17] = "     TEST X     ";
    if (oled_testPolarity) {
        header[0] = 0xB0;
        header[15] = 0xB0;
    }
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
