#include <xc.h>
#include <stdbool.h>
#include "app.h"
#include "led.h"
#include "ssd1306.h"
#include "ticker.h"

void main(void) {
    init();
    led_init();
    ticker_init();

    CLRWDT();
    led_activity_on(); ticker_waitTick(); led_activity_off(); ticker_waitTick();
    led_activity_on(); ticker_waitTick(); led_activity_off(); ticker_waitTick();
    led_activity_on(); ticker_waitTick(); led_activity_off(); ticker_waitTick();

    while(true) {
        CLRWDT();
    }
}


void init(void) {
    // Disable interrupts
    GIE = 0;
}
