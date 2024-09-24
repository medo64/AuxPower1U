#pragma once
#include <stdint.h>

void oled_init(void);
void oled_splash(void);

void oled_writeSummary(uint16_t voltage1, uint16_t voltage2, uint16_t voltage3, uint16_t voltage4,uint16_t voltage5, uint16_t current1, uint16_t current2, uint16_t current3, uint16_t current4,uint16_t current5, uint16_t temperature);
