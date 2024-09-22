#include <xc.h>
#include <stdbool.h>
#include "app.h"
#include "led.h"

void main(void) {
    init();
    led_init();

    CLRWDT(); led_activity_on(); __delay_ms(250); led_activity_off(); __delay_ms(250); 
    CLRWDT(); led_activity_on(); __delay_ms(250); led_activity_off(); __delay_ms(250); 
    CLRWDT(); led_activity_on(); __delay_ms(250); led_activity_off(); __delay_ms(250); 

    while(true) {
        CLRWDT();
    }
}


void init(void) {
    // Disable interrupts
    GIE = 0;
}
