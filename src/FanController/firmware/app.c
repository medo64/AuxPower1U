#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "app.h"
#include "pwm.h"

#define SPEED_SPEC_MIN   85  // 30%
#define SPEED_SPEC_MAX  255  // 30%
#define SPEED_USE_MIN    26  // 10%
#define SPEED_USE_MAX   128  // 50%

void main(void) {
    init();
    pwm_init();

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
        // linear increase 40-60
        // if temperature > 65 go to 100%
        CLRWDT();
    }
}


void init(void) {
    // Interrupt Control
    GIE = 0;               // Disables all interrupts

    // Oscillator Control
    OSCCON = 0b01111011;   // 16 MHz
}
