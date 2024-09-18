#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "app.h"
#include "pwm.h"

void main(void) {
    init();
    pwm_set_all(128);

    CLRWDT();
    __delay_ms(2000);

    pwm_set_all(32);
    while(true) {
        CLRWDT();
    }
}


void init(void) {
    // Interrupt Control
    GIE = 0;               // Disables all interrupts

    // Oscillator Control
    OSCCON = 0b01111011;   // 16 MHz

    // Pin Setup
    TRISA = 0b001010;      // PWM4 PWM3 MCLR PWM1 Temp PWM2
    ANSELA = 0b000010;
    
    // PWM Module (16 MHz)
    PWM1CON0 = 0;          // Clear the PWMxCON register
    PWM2CON0 = 0;
    PWM3CON0 = 0;
    PWM4CON0 = 0;
    PR2 = 0x9F;            // Load the PR2 register with the PWM period value
    PWM1DCH = 0b00000000;  // Clear the PWMxDCH register and bits <7:6> of the PWMxDCL register
    PWM1DCL = 0b00;
    PWM2DCH = 0b00000000;
    PWM2DCL = 0b00;
    PWM3DCH = 0b00000000;
    PWM3DCL = 0b00;
    PWM4DCH = 0b00000000;
    PWM4DCL = 0b00;
    TMR2IF = 0;            // Clear the TMR2IF interrupt flag bit of the PIR1 register [Interrupt is not pending]
    T2CKPS1 = 0;           // Configure the T2CKPS bits of the T2CON register with the Timer2 prescale value [Prescaler is 1]
    T2CKPS0 = 0;
    TMR2ON = 1;            // Enable Timer2 by setting the TMR2ON bit of the T2CON register [Timer2 is on]
    while (!TMR2IF);       // and wait until Timer2 overflows [Interrupt is pending]
    TRISA &= 0b001010;     // Enable the PWMx pin output driver(s) by clearing the associated TRIS bit(s) [PWM4 PWM3 ---- PWM1 ---- PWM2]
    PWM1OE = 1;            // and setting the PWMxOE bit of the PWMxCON register [Output to PWMx pin is enabled]
    PWM2OE = 1;
    PWM3OE = 1;
    PWM4OE = 1;
    PWM1POL = 0;           // Configure the PWM module by loading the PWMxCON register with the appropriate values [0 = PWM output is active-high]
    PWM2POL = 0;
    PWM3POL = 0;
    PWM4POL = 0;
    PWM1EN = 1;            // Enable PWM output [PWM module is enabled]
    PWM2EN = 1;
    PWM3EN = 1;
    PWM4EN = 1;
}
