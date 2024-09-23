#include <xc.h>
#include <stdbool.h>
#include "app.h"
#include "io.h"
#include "ssd1306.h"
#include "i2c_master.h"
#include "ticker.h"
#include "ioex.h"

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

    ioex_init();
    ioex_button1_led_on();
    ioex_output1_on();

    uint8_t x[2] = { 0, 0 };
    bool y = i2c_master_readRegisterBytes(0x20, 1, x, 1);

    bool switch1State, switch2State, switch3State, switch4State, switch5State;
    while(true) {
        CLRWDT();
        ticker_waitTick(); io_led_activity_on();
        ticker_waitTick(); io_led_activity_off();

        ioex_button_getSwitches(&switch1State, &switch2State, &switch3State, &switch4State, &switch5State);
        if (switch1State) { ioex_button1_led_on(); } else { ioex_button1_led_off(); }
        if (switch2State) { ioex_button2_led_on(); } else { ioex_button2_led_off(); }
        if (switch3State) { ioex_button3_led_on(); } else { ioex_button3_led_off(); }
        if (switch4State) { ioex_button4_led_on(); } else { ioex_button4_led_off(); }
        if (switch5State) { ioex_button5_led_on(); } else { ioex_button5_led_off(); }
    }
}


void init(void) {
    // Disable interrupts
    GIE = 0;
}
