#include <xc.h>
#include <stdint.h>

void pwm_set_individual(uint8_t pwm1, uint8_t pwm2, uint8_t pwm3, uint8_t pwm4) {
    TMR2IF = 0;
    while (!TMR2IF); // wait for the next period

    PWM1DCH = pwm1;
    PWM2DCH = pwm2;
    PWM3DCH = pwm3;
    PWM4DCH = pwm4;
}

void pwm_set_all(uint8_t pwm) {
    pwm_set_individual(pwm, pwm, pwm, pwm);
}


void pwm_init(void) {
    // Pin Setup
    TRISA  &= 0b001010;      // PWM4 PWM3 MCLR PWM1 Temp PWM2
    ANSELA &= 0b001010;

    // Clear the PWMxCON register
    PWM1CON0 = 0;
    PWM2CON0 = 0;
    PWM3CON0 = 0;
    PWM4CON0 = 0;

    // Load the PR2 register with the PWM period value [25 kHz @ 16 MHz Fosc]
    PR2 = 0x9F;

    // Clear the PWMxDCH register and bits <7:6> of the PWMxDCL register
    PWM1DCH = 0b00000000;
    PWM1DCL = 0b00;
    PWM2DCH = 0b00000000;
    PWM2DCL = 0b00;
    PWM3DCH = 0b00000000;
    PWM3DCL = 0b00;
    PWM4DCH = 0b00000000;
    PWM4DCL = 0b00;

    // Configure the T2CKPS bits of the T2CON register with the Timer2 prescale value
    T2CKPS1 = 0;  // Prescaler is 1
    T2CKPS0 = 0;

    // Enable Timer2 by setting the TMR2ON bit of the T2CON register
    TMR2IF = 0;  // Clear the TMR2IF interrupt flag
    TMR2ON = 1;  // imer2 is on
    while (!TMR2IF);  // and wait until Timer2 overflows

    // Enable the PWMx pin output drivers
    PWM1OE = 1; 
    PWM2OE = 1;
    PWM3OE = 1;
    PWM4OE = 1;

    // Configure the PWM module by loading the PWMxCON register with the appropriate values
    PWM1POL = 0;  // PWM output is active-high
    PWM2POL = 0;
    PWM3POL = 0;
    PWM4POL = 0;

    // Enable PWM output
    PWM1EN = 1;  // PWM module is enabled
    PWM2EN = 1;
    PWM3EN = 1;
    PWM4EN = 1;
}
