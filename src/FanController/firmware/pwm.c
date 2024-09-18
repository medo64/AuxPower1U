#include <xc.h>
#include <stdint.h>

void pwm_set_all(uint8_t value) {
    TMR2IF = 0;
    while (!TMR2IF); // wait for the next period
    PWM1DCH = value;
    PWM2DCH = value;
    PWM3DCH = value;
    PWM4DCH = value;
}
