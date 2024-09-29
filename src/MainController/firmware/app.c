#include <xc.h>
#include <stdbool.h>
#include "app.h"
#include "adc.h"
#include "i2c_master.h"
#include "io.h"
#include "ioex.h"
#include "oled.h"
#include "ticker.h"
#include "ssd1306.h"

#define AVG_SHIFT  3

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
    ioex_button_getSwitches(&switch1State, &switch2State, &switch3State, &switch4State, &switch5State);
    if (switch1State && !switch2State && !switch3State && !switch4State && switch5State) {
        test();
        return;
    }

    uint16_t voltage1, current1, voltage2, current2, voltage3, current3, voltage4, current4, voltage5, current5, temperature;
    adc_measureBasic(&voltage1, &current1, &voltage2, &current2, &voltage3, &current3, &voltage4, &current4, &voltage5, &current5, &temperature);

    uint32_t voltage1Sum = (uint32_t)voltage1 << AVG_SHIFT, current1Sum = (uint32_t)current1 << AVG_SHIFT;
    uint32_t voltage2Sum = (uint32_t)voltage2 << AVG_SHIFT, current2Sum = (uint32_t)current2 << AVG_SHIFT;
    uint32_t voltage3Sum = (uint32_t)voltage3 << AVG_SHIFT, current3Sum = (uint32_t)current3 << AVG_SHIFT;
    uint32_t voltage4Sum = (uint32_t)voltage4 << AVG_SHIFT, current4Sum = (uint32_t)current4 << AVG_SHIFT;
    uint32_t voltage5Sum = (uint32_t)voltage5 << AVG_SHIFT, current5Sum = (uint32_t)current5 << AVG_SHIFT;
    uint32_t temperatureSum = (uint32_t)temperature << AVG_SHIFT;

    uint8_t tickCounter = 0;
    while(true) {
        CLRWDT();

        if (ticker_hasTicked()) {  // 24th of a second
            tickCounter++;

            adc_measureBasic(&voltage1, &current1, &voltage2, &current2, &voltage3, &current3, &voltage4, &current4, &voltage5, &current5, &temperature);
            voltage1Sum -= (voltage1Sum >> AVG_SHIFT); voltage1Sum += voltage1;
            voltage2Sum -= (voltage2Sum >> AVG_SHIFT); voltage2Sum += voltage2;
            voltage3Sum -= (voltage3Sum >> AVG_SHIFT); voltage3Sum += voltage3;
            voltage4Sum -= (voltage4Sum >> AVG_SHIFT); voltage4Sum += voltage4;
            voltage5Sum -= (voltage5Sum >> AVG_SHIFT); voltage5Sum += voltage5;
            current1Sum -= (current1Sum >> AVG_SHIFT); current1Sum += current1;
            current2Sum -= (current2Sum >> AVG_SHIFT); current2Sum += current2;
            current3Sum -= (current3Sum >> AVG_SHIFT); current3Sum += current3;
            current4Sum -= (current4Sum >> AVG_SHIFT); current4Sum += current4;
            current5Sum -= (current5Sum >> AVG_SHIFT); current5Sum += current5;
            temperatureSum -= (temperatureSum >> AVG_SHIFT); temperatureSum += temperature;

            ioex_button_getSwitches(&switch1State, &switch2State, &switch3State, &switch4State, &switch5State);
            ioex_button_setLeds(switch1State, switch2State, switch3State, switch4State, switch5State);
            ioex_button_setOutputs(switch1State, switch2State, switch3State, switch4State, switch5State);

            io_led_activity_off();
        }

        if (tickCounter == 24) {
            tickCounter = 0;
            io_led_activity_on();
            
            uint16_t voltage1Avg = (uint16_t)(voltage1Sum >> AVG_SHIFT);
            uint16_t voltage2Avg = (uint16_t)(voltage2Sum >> AVG_SHIFT);
            uint16_t voltage3Avg = (uint16_t)(voltage3Sum >> AVG_SHIFT);
            uint16_t voltage4Avg = (uint16_t)(voltage4Sum >> AVG_SHIFT);
            uint16_t voltage5Avg = (uint16_t)(voltage5Sum >> AVG_SHIFT);
            uint16_t current1Avg = (uint16_t)(current1Sum >> AVG_SHIFT);
            uint16_t current2Avg = (uint16_t)(current2Sum >> AVG_SHIFT);
            uint16_t current3Avg = (uint16_t)(current3Sum >> AVG_SHIFT);
            uint16_t current4Avg = (uint16_t)(current4Sum >> AVG_SHIFT);
            uint16_t current5Avg = (uint16_t)(current5Sum >> AVG_SHIFT);
            uint16_t temperatureAvg = (uint16_t)(temperatureSum >> AVG_SHIFT);
            oled_writeSummary(voltage1Avg, current1Avg, voltage2Avg, current2Avg, voltage3Avg, current3Avg, voltage4Avg, current4Avg, voltage5Avg, current5Avg, temperatureAvg);
        }
    }
}


void test(void) {
    ssd1306_displayInvert();
    bool switch1State, switch2State, switch3State, switch4State, switch5State;
    while(true) {
        CLRWDT();
        ioex_button_getSwitches(&switch1State, &switch2State, &switch3State, &switch4State, &switch5State);
        if (!(switch1State | switch2State | switch3State | switch4State | switch5State)) { break; }
    }
    ssd1306_displayNormal();
    ticker_waitTick();

    uint8_t updateCounter = 0;
    uint8_t state = 0;

    uint16_t voltage1, current1, voltage2, current2, voltage3, current3, voltage4, current4, voltage5, current5, temperature;
    adc_measureBasic(&voltage1, &current1, &voltage2, &current2, &voltage3, &current3, &voltage4, &current4, &voltage5, &current5, &temperature);

    uint16_t voltage12V, voltage5V, temperatureDie;
    adc_measureExtra(&voltage12V, &voltage5V, &temperatureDie);

    uint32_t voltage1Sum = (uint32_t)voltage1 << AVG_SHIFT, current1Sum = (uint32_t)current1 << AVG_SHIFT;
    uint32_t voltage2Sum = (uint32_t)voltage2 << AVG_SHIFT, current2Sum = (uint32_t)current2 << AVG_SHIFT;
    uint32_t voltage3Sum = (uint32_t)voltage3 << AVG_SHIFT, current3Sum = (uint32_t)current3 << AVG_SHIFT;
    uint32_t voltage4Sum = (uint32_t)voltage4 << AVG_SHIFT, current4Sum = (uint32_t)current4 << AVG_SHIFT;
    uint32_t voltage5Sum = (uint32_t)voltage5 << AVG_SHIFT, current5Sum = (uint32_t)current5 << AVG_SHIFT;
    uint32_t temperatureSum = (uint32_t)temperature << AVG_SHIFT;
    uint32_t voltage12VSum = (uint32_t)voltage12V << AVG_SHIFT;
    uint32_t voltage5VSum = (uint32_t)voltage5V << AVG_SHIFT;
    uint32_t temperatureDieSum = (uint32_t)temperatureDie << AVG_SHIFT;

    while(true) {
        CLRWDT();
        if (ticker_hasTicked()) {
            updateCounter++;

            adc_measureBasic(&voltage1, &current1, &voltage2, &current2, &voltage3, &current3, &voltage4, &current4, &voltage5, &current5, &temperature);
            voltage1Sum -= (voltage1Sum >> AVG_SHIFT); voltage1Sum += voltage1;
            voltage2Sum -= (voltage2Sum >> AVG_SHIFT); voltage2Sum += voltage2;
            voltage3Sum -= (voltage3Sum >> AVG_SHIFT); voltage3Sum += voltage3;
            voltage4Sum -= (voltage4Sum >> AVG_SHIFT); voltage4Sum += voltage4;
            voltage5Sum -= (voltage5Sum >> AVG_SHIFT); voltage5Sum += voltage5;
            current1Sum -= (current1Sum >> AVG_SHIFT); current1Sum += current1;
            current2Sum -= (current2Sum >> AVG_SHIFT); current2Sum += current2;
            current3Sum -= (current3Sum >> AVG_SHIFT); current3Sum += current3;
            current4Sum -= (current4Sum >> AVG_SHIFT); current4Sum += current4;
            current5Sum -= (current5Sum >> AVG_SHIFT); current5Sum += current5;
            temperatureSum -= (temperatureSum >> AVG_SHIFT); temperatureSum += temperature;

            adc_measureExtra(&voltage12V, &voltage5V, &temperatureDie);
            voltage12VSum -= (voltage12VSum >> AVG_SHIFT); voltage12VSum += voltage12V;
            voltage5VSum -= (voltage5VSum >> AVG_SHIFT); voltage5VSum += voltage5V;
            temperatureDieSum -= (temperatureDieSum >> AVG_SHIFT); temperatureDieSum += temperatureDie;

            if (updateCounter == 4) {
                updateCounter = 0;

                switch(index) {
                    case 0: oled_writeTestVT(index, (uint16_t)(voltage12VSum >> AVG_SHIFT), (uint16_t)(temperatureSum >> AVG_SHIFT)); break;
                    case 1: oled_writeTestVC(1, (uint16_t)(voltage1Sum >> AVG_SHIFT), (uint16_t)(current1Sum >> AVG_SHIFT)); break;
                    case 2: oled_writeTestVC(2, (uint16_t)(voltage2Sum >> AVG_SHIFT), (uint16_t)(current2Sum >> AVG_SHIFT)); break;
                    case 3: oled_writeTestVC(3, (uint16_t)(voltage3Sum >> AVG_SHIFT), (uint16_t)(current3Sum >> AVG_SHIFT)); break;
                    case 4: oled_writeTestVC(4, (uint16_t)(voltage4Sum >> AVG_SHIFT), (uint16_t)(current4Sum >> AVG_SHIFT)); break;
                    case 5: oled_writeTestVC(5, (uint16_t)(voltage5Sum >> AVG_SHIFT), (uint16_t)(current5Sum >> AVG_SHIFT)); break;
                    case 6: oled_writeTestVT(index, (uint16_t)(voltage5VSum >> AVG_SHIFT), (uint16_t)(temperatureDieSum >> AVG_SHIFT)); break;
                }

                ioex_button_getSwitches(&switch1State, &switch2State, &switch3State, &switch4State, &switch5State);
                if (switch1State | switch2State | switch3State | switch4State | switch5State) {
                    ssd1306_displayInvert();
                    index = (index + 1) % 7;
                    while(true) {
                        CLRWDT();
                        ioex_button_getSwitches(&switch1State, &switch2State, &switch3State, &switch4State, &switch5State);
                        if (!(switch1State | switch2State | switch3State | switch4State | switch5State)) { break; }
                    }
                    ssd1306_displayNormal();
                    ticker_waitTick();
                    switch(index) {
                        case 0: ioex_button_setLeds(false, false, false, false, false); ioex_button_setOutputs(false, false, false, false, false); break;
                        case 1: ioex_button_setLeds(true, false, false, false, false); ioex_button_setOutputs(true, false, false, false, false); break;
                        case 2: ioex_button_setLeds(false, true, false, false, false); ioex_button_setOutputs(false, true, false, false, false); break;
                        case 3: ioex_button_setLeds(false, false, true, false, false); ioex_button_setOutputs(false, false, true, false, false); break;
                        case 4: ioex_button_setLeds(false, false, false, true, false); ioex_button_setOutputs(false, false, false, true, false); break;
                        case 5: ioex_button_setLeds(false, false, false, false, true); ioex_button_setOutputs(false, false, false, false, true); break;
                        case 6: ioex_button_setLeds(false, false, false, false, false); ioex_button_setOutputs(false, false, false, false, false); break;
                    }
                }
            }
        }
    }
}


void init(void) {
    // Disable interrupts
    GIE = 0;
}
