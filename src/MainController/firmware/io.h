#pragma once

void io_init(void);

#define io_led_activity_on()      LATBbits.LATB3 = 1;
#define io_led_activity_off()     LATBbits.LATB3 = 0;
