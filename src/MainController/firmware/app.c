#include <xc.h>
#include <stdbool.h>
#include "app.h"
#include "i2c_master.h"
#include "io.h"
#include "ioex.h"
#include "oled.h"
#include "ticker.h"

void main(void) {
    init();
    ticker_init();

    io_init();
    io_led_activity_on(); ticker_waitTick(); io_led_activity_off(); ticker_waitTick();
    io_led_activity_on(); ticker_waitTick(); io_led_activity_off(); ticker_waitTick();
    io_led_activity_on(); ticker_waitTick(); io_led_activity_off(); ticker_waitTick();

    i2c_master_init();

    io_led_activity_on();
    oled_init();
    oled_splash();
    io_led_activity_off();

    io_led_activity_on();
    //ioex_init();
    io_led_activity_off();


    //ioex_button1_led_on();
    //ioex_output1_on();

    bool switch1State, switch2State, switch3State, switch4State, switch5State;
    uint8_t tickCounter = 0;
    while(true) {
        CLRWDT();
        if (ticker_hasTicked()) {  // 24th of a second
            tickCounter++;
            io_led_activity_off();
        }

        //ioex_button_getSwitches(&switch1State, &switch2State, &switch3State, &switch4State, &switch5State);
        //if (switch1State) { ioex_button1_led_on(); } else { ioex_button1_led_off(); }
        //if (switch2State) { ioex_button2_led_on(); } else { ioex_button2_led_off(); }
        //if (switch3State) { ioex_button3_led_on(); } else { ioex_button3_led_off(); }
        //if (switch4State) { ioex_button4_led_on(); } else { ioex_button4_led_off(); }
        //if (switch5State) { ioex_button5_led_on(); } else { ioex_button5_led_off(); }

        if (tickCounter == 24) {
            tickCounter = 0;
            io_led_activity_on();
            oled_writeSummary(12000, 55000, 55000, 55000, 15000, 16700, 1200, 1200, 1200, 13400, 205);
        }
    }
}


void init(void) {
    // Disable interrupts
    GIE = 0;
}
