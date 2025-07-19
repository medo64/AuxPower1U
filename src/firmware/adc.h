#pragma once

void adc_init(void);

void adc_measureBasic(uint16_t* voltage1, uint16_t* current1, uint16_t* voltage2, uint16_t* current2, uint16_t* voltage3, uint16_t* current3, uint16_t* voltage4, uint16_t* current4, uint16_t* voltage5, uint16_t* current5, uint16_t* temperature);
void adc_measureExtra(uint16_t* voltage12V, uint16_t* voltage5V, uint16_t* temperatureDie);
