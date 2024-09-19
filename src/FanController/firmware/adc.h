#pragma once

#include <xc.h>
#include <stdint.h>

void adc_init(void);
uint8_t adc_get_temperature(void);
