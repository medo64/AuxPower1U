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

void startup(void);

void main(void) {
    init();
    pwm_init();
    adc_init();

    startup();

    uint16_t temperatureSum = (uint16_t)(adc_get_temperature()) << 3;  // 8x running average
    while(true) {
        CLRWDT();

        uint8_t temperatureNew = adc_get_temperature();  // measure new temperature
        uint8_t temperatureAvg = (uint8_t)(temperatureSum >> 3);  // take average from previous run
        temperatureSum = (temperatureSum - temperatureAvg) + temperatureNew;  // add new measure in

        if (temperatureAvg < 40) {  // minimum fan speed up to 40 °C
            pwm_set_all(SPEED_USE_MIN);
        } else if (temperatureAvg < 60) {  // linear increase 40-60 °C
            uint8_t scaleSpeedPerC = (SPEED_USE_MAX - SPEED_USE_MIN) / 20;
            uint8_t scaleT = temperatureAvg - 40;
            pwm_set_all(SPEED_USE_MIN + scaleSpeedPerC * scaleT);
        } else if (temperatureAvg < 70) {  // higher speed up to 70 °C
            pwm_set_all(SPEED_USE_MAX);
        } else {  // max speed if higher than 70 °C
            pwm_set_all(SPEED_SPEC_MAX);
        }

        __delay_ms(100);  // delay speed changes a bit
    }
}

void startup(void) {
    // each fan:
    // * starts at 30% for 0.5 seconds
    // * goes to full speed for 0.25s
    // * and then goes down to 10%.

    CLRWDT();
    pwm_set_individual(SPEED_SPEC_MIN, 0, 0, 0);
    __delay_ms(250);

    CLRWDT();
    pwm_set_individual(SPEED_SPEC_MIN, SPEED_SPEC_MIN, 0, 0);
    __delay_ms(250);

    CLRWDT();
    pwm_set_individual(SPEED_SPEC_MAX, SPEED_SPEC_MIN, SPEED_SPEC_MIN, 0);
    __delay_ms(250);

    CLRWDT();
    pwm_set_individual(SPEED_USE_MIN, SPEED_SPEC_MAX, SPEED_SPEC_MIN, SPEED_SPEC_MIN);
    __delay_ms(250);

    CLRWDT();
    pwm_set_individual(SPEED_USE_MIN, SPEED_USE_MIN, SPEED_SPEC_MAX, SPEED_SPEC_MIN);
    __delay_ms(250);

    CLRWDT();
    pwm_set_individual(SPEED_USE_MIN, SPEED_USE_MIN, SPEED_USE_MIN, SPEED_SPEC_MAX);
    __delay_ms(250);

    CLRWDT();
    pwm_set_individual(SPEED_USE_MIN, SPEED_USE_MIN, SPEED_USE_MIN, SPEED_USE_MIN);
    __delay_ms(250);
}

void init(void) {
    // Interrupt Control
    GIE = 0;               // Disables all interrupts

    // Oscillator Control
    OSCCON = 0b01111011;   // 16 MHz
}
