#pragma once

void settings_init(void);

uint8_t settings_outputs_get(void);
void settings_outputs_set(uint8_t value);

void settings_cutoff_current_get(uint16_t* current1, uint16_t* current2, uint16_t* current3, uint16_t* current4, uint16_t* current5);
