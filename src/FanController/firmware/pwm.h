#pragma once
#include <stdint.h>

void pwm_init(void);
void pwm_set_all(uint8_t pwm);
void pwm_set_individual(uint8_t pwm1, uint8_t pwm2, uint8_t pwm3, uint8_t pwm4);
