#include <xc.h>
#include <stdbool.h>
#include "app.h"
#include "io.h"
#include "ssd1306.h"
#include "i2c_master.h"
#include "ticker.h"

void main(void) {
    init();
    ticker_init();
    io_init();

    CLRWDT();
    io_led_activity_on(); ticker_waitTick(); io_led_activity_off(); ticker_waitTick();
    io_led_activity_on(); ticker_waitTick(); io_led_activity_off(); ticker_waitTick();
    io_led_activity_on(); ticker_waitTick(); io_led_activity_off(); ticker_waitTick();

    i2c_master_init();
    ssd1306_init();
    ssd1306_clearAll();
    ssd1306_writeText16("   AuxPower1U   ");
    ssd1306_moveToNextRow16();
    ssd1306_writeText("   medo64.com   ");

    while(true) {
        CLRWDT();
        ticker_waitTick(); io_led_activity_on();
        ticker_waitTick(); io_led_activity_off();
    }
}


void init(void) {
    // Disable interrupts
    GIE = 0;
}
