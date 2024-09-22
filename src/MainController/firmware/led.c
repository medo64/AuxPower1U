#include <xc.h>

void led_init(void) {
    LATBbits.LATB3 = 0;
    TRISBbits.TRISB3 = 0;
    ANSELBbits.ANSELB3 = 0;
}
