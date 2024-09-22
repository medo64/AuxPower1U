#pragma once

void led_init(void);

#define led_activity_on()      LATBbits.LATB3 = 1;
#define led_activity_off()     LATBbits.LATB3 = 0;
