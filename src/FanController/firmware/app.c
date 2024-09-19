#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "app.h"
#include "adc.h"
#include "pwm.h"

#define SPEED_SPEC_MIN   85  //  30%
#define SPEED_SPEC_MAX  255  // 100%
#define SPEED_USE_MIN    26  //  10%
#define SPEED_USE_MAX   128  //  50%

void main(void) {
    init();
    pwm_init();
    adc_init();

    CLRWDT();
    pwm_set_all(SPEED_SPEC_MIN);
    __delay_ms(500);

    CLRWDT();
    pwm_set_all(SPEED_SPEC_MAX);  // (in case something is stuck)
    __delay_ms(250);

    CLRWDT();
    pwm_set_all(SPEED_USE_MIN);
    __delay_ms(250);

    while(true) {
        CLRWDT();

        uint8_t temperature = adc_get_temperature();
        if (temperature < 40) {  // minimum fan speed up to 40C
            pwm_set_all(SPEED_USE_MIN);
        } else if (temperature < 60) {  // linear increase 40-60
            uint8_t scaleSpeedPerC = (SPEED_USE_MAX - SPEED_USE_MIN) / 20;
            uint8_t scaleT = temperature - 60;
            pwm_set_all(SPEED_USE_MIN + scaleSpeedPerC * scaleT);
        } else if (temperature < 70) {  // higher speed 
            pwm_set_all(SPEED_USE_MAX);
        } else {  // max speed if 70+
            pwm_set_all(SPEED_SPEC_MAX);
        }
    }
}


void init(void) {
    // Interrupt Control
    GIE = 0;               // Disables all interrupts

    // Oscillator Control
    OSCCON = 0b01111011;   // 16 MHz
}
