#include <xc.h>
#include <stdbool.h>
#include "app.h"
#include "adc.h"
#include "i2c_master.h"
#include "io.h"
#include "ioex.h"
#include "oled.h"
#include "ticker.h"

void main(void) {
    init();
    ticker_init();
    adc_init();

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
    ioex_init();
    io_led_activity_off();

    bool switch1State, switch2State, switch3State, switch4State, switch5State;
    uint16_t voltage1, current1, voltage2, current2, voltage3, current3, voltage4, current4, voltage5, current5, temperature, voltage1in;
    uint8_t tickCounter = 0;
    while(true) {
        CLRWDT();

        if (ticker_hasTicked()) {  // 24th of a second
            tickCounter++;
            adc_measureAll(&voltage1, &current1, &voltage2, &current2, &voltage3, &current3, &voltage4, &current4, &voltage5, &current5, &temperature, &voltage1in);

            ioex_button_getSwitches(&switch1State, &switch2State, &switch3State, &switch4State, &switch5State);
            ioex_button_setLeds(switch1State, switch2State, switch3State, switch4State, switch5State);
            ioex_button_setOutputs(switch1State, switch2State, switch3State, switch4State, switch5State);

            io_led_activity_off();
        }

        if (tickCounter == 24) {
            tickCounter = 0;
            io_led_activity_on();
            oled_writeSummary(voltage1, current1, voltage2, current2, voltage3, current3, voltage4, current4, voltage5, current5, temperature);
        }
    }
}


void init(void) {
    // Disable interrupts
    GIE = 0;
}
