#pragma once
#include <stdint.h>

void measurement_init(uint16_t cutoffFastCurrent1, uint16_t cutoffFastCurrent2, uint16_t cutoffFastCurrent3, uint16_t cutoffFastCurrent4, uint16_t cutoffFastCurrent5);

void measurement_basic(uint8_t* nextOutputs, uint16_t* voltage1, uint16_t* current1, uint16_t* voltage2, uint16_t* current2, uint16_t* voltage3, uint16_t* current3, uint16_t* voltage4, uint16_t* current4, uint16_t* voltage5, uint16_t* current5, uint16_t* temperature);
void measurement_extra(uint16_t* voltage12V, uint16_t* voltage5V, uint16_t* temperatureDie);
