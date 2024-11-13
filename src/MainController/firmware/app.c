#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "app.h"
#include "adc.h"
#include "i2c_master.h"
#include "io.h"
#include "ioex.h"
#include "oled.h"
#include "pps.h"
#include "ticker.h"
#include "settings.h"
#include "ssd1306.h"
#include "uart.h"
#include "uartbuffers.h"

#define AVG_SHIFT  3

#define DEPTH_SUMMARY          0
#define DEPTH_DETAILS          1
#define DEPTH_PENDING_RESET    2
#define DEPTH_PENDING_OFF      3
#define DEPTH_PENDING_NOTHING  4

#define TICKS_CLOSE_DETAILS  60 * 24
#define TICKS_IGNORE_DETAILS       6
#define TICKS_GOTO_RESET      3 * 24
#define TICKS_WAIT_RESET      3 * 24
#define TICKS_WAIT_OFF        3 * 24
#define TICKS_DURATION_RESET  3 * 24

#define CUTOFF_CURRENT_FAST   8000
#define CUTOFF_CURRENT_AVG    6666
#define CUTOFF_POWER_FAST   120000
#define CUTOFF_POWER_AVG    100000


void fillUartFromChannel(uint8_t index, uint16_t voltage, uint16_t current, bool isOn);
void fillUartFromTemperature(uint16_t temperature);

uint8_t enableOutputMask(const uint8_t channel, const uint8_t currOutputs);
uint8_t disableOutputMask(const uint8_t channel, const uint8_t currOutputs);
uint8_t resetOutput(const uint8_t channel, const uint8_t currOutputs);

void main(void) {
    init();
    pps_init();
    ticker_init();

    CLRWDT();
    io_init();
    io_led_activity_on(); ticker_waitTick(); io_led_activity_off(); ticker_waitTick();
    io_led_activity_on(); ticker_waitTick(); io_led_activity_off(); ticker_waitTick();
    io_led_activity_on(); ticker_waitTick(); io_led_activity_off(); ticker_waitTick();

    CLRWDT();
    io_led_activity_on();
    uart_init();
    uart_writeString("AuxPower1U\r\n");
    io_led_activity_off();

    CLRWDT();
    io_led_activity_on();
    i2c_master_init();
    oled_init();
    oled_splash();
    ioex_init();
    io_led_activity_off();

    bool switch1State, switch2State, switch3State, switch4State, switch5State;
    ioex_button_switch_getAll(&switch1State, &switch2State, &switch3State, &switch4State, &switch5State);
    if (switch1State && !switch2State && !switch3State && !switch4State && switch5State) {
        adc_init();
        test();
        return;
    }

    CLRWDT();
    settings_init();
    uint8_t nextOutputs = settings_outputs_get();
    uint8_t currOutputs = nextOutputs | 0b10000000;  // just to force change when first ran

    CLRWDT();
    adc_init();
    uint16_t voltage1, current1, voltage2, current2, voltage3, current3, voltage4, current4, voltage5, current5, temperature;
    adc_measureBasic(&voltage1, &current1, &voltage2, &current2, &voltage3, &current3, &voltage4, &current4, &voltage5, &current5, &temperature);

    // lower current measurement to avoid tripping average current protection due to a single measurement
    if (current1 > 100) { current1 -= 100; } else { current1 = 0; }
    if (current2 > 100) { current2 -= 100; } else { current2 = 0; }
    if (current3 > 100) { current3 -= 100; } else { current3 = 0; }
    if (current4 > 100) { current4 -= 100; } else { current4 = 0; }
    if (current5 > 100) { current5 -= 100; } else { current5 = 0; }

    // expand initial measurement
    uint32_t voltage1Sum = (uint32_t)voltage1 << AVG_SHIFT, current1Sum = (uint32_t)current1 << AVG_SHIFT;
    uint32_t voltage2Sum = (uint32_t)voltage2 << AVG_SHIFT, current2Sum = (uint32_t)current2 << AVG_SHIFT;
    uint32_t voltage3Sum = (uint32_t)voltage3 << AVG_SHIFT, current3Sum = (uint32_t)current3 << AVG_SHIFT;
    uint32_t voltage4Sum = (uint32_t)voltage4 << AVG_SHIFT, current4Sum = (uint32_t)current4 << AVG_SHIFT;
    uint32_t voltage5Sum = (uint32_t)voltage5 << AVG_SHIFT, current5Sum = (uint32_t)current5 << AVG_SHIFT;
    uint32_t temperatureSum = (uint32_t)temperature << AVG_SHIFT;

    uint8_t currDepth = DEPTH_SUMMARY;  // 0: summary; 1: details; 2: prepare reset; 3: reset; 4: shutdown
    uint8_t nextDepth = DEPTH_SUMMARY;
    uint16_t currDepthTicks = 0;        // how long has system been at the current level
    uint16_t currDepthButtonTicks = 0;  // how many ticks passed with current buttons at current level
    uint8_t currChannel = 0;            // 1-5: selected channel
    uint8_t currButtonMask = 0;         // currently pressed buttons
    uint8_t prevButtonMask = 0;         // previously pressed buttons

    bool prevBlinking = false;
    uint8_t tickCounter = 0;
    while(true) {
        CLRWDT();

        if (UartTxBufferStart < UartTxBufferCount) {  // send data if in buffer
            if (uart_tryWriteByte(UartTxBuffer[UartTxBufferStart])) {
                UartTxBufferStart++;
            }
        }

        uint8_t rxData;
        while (uart_tryReadByte(&rxData)) {
            if ((rxData == '\n') || (rxData == '\r')) {
                bool isNok = true;
                if (UartRxBufferCount == 2) {
                    uint8_t channelIndex = UartRxBuffer[1] - 0x30;
                    if ((channelIndex >= 1) && (channelIndex <= 5)) {
                        switch (UartRxBuffer[0]) {
                            case 'D':
                            case 'd':
                                nextOutputs = disableOutputMask(channelIndex, nextOutputs);
                                isNok = false;
                                break;

                            case 'E':
                            case 'e':
                                nextOutputs = enableOutputMask(channelIndex, nextOutputs);
                                isNok = false;
                                break;
                               break;

                            case 'I':
                            case 'i':
                                currChannel = channelIndex;
                                nextDepth = DEPTH_DETAILS;
                                break;

                            case 'R':
                            case 'r':
                                nextOutputs = resetOutput(channelIndex, nextOutputs);
                                isNok = false;
                                break;
                        }
                    }
                } else if  (UartRxBufferCount == 1) {
                    switch (UartRxBuffer[0]) {
                        case 'I':
                        case 'i':
                            nextDepth = DEPTH_SUMMARY;
                            isNok = false;
                    }
                } else if  (UartRxBufferCount == 0) {
                    isNok = false;  // ignore empty
                }
                CLRWDT();
                if (isNok) {
                    UartRxBufferCount = 0;
                    uartbuffers_txAppend('!');
                    uartbuffers_txAppend('\r');
                    uartbuffers_txAppend('\n');
                }
                UartRxBufferCount = 0;
            } else {
                uartbuffers_rxAppend(rxData);
            }
        }

        if (ticker_hasTicked()) {  // 24th of a second
            io_led_activity_off();
            tickCounter++;
            if (tickCounter == 24) { tickCounter = 0; }
            currDepthTicks++;

            // measure each tick
            adc_measureBasic(&voltage1, &current1, &voltage2, &current2, &voltage3, &current3, &voltage4, &current4, &voltage5, &current5, &temperature);

            // cutoff if exceeding current
            if (current1 > CUTOFF_CURRENT_FAST) { nextOutputs &= 0b11110; }
            if (current2 > CUTOFF_CURRENT_FAST) { nextOutputs &= 0b11101; }
            if (current3 > CUTOFF_CURRENT_FAST) { nextOutputs &= 0b11011; }
            if (current4 > CUTOFF_CURRENT_FAST) { nextOutputs &= 0b10111; }
            if (current5 > CUTOFF_CURRENT_FAST) { nextOutputs &= 0b01111; }

            // cutoff if exceeding power
            if (((uint32_t)voltage1 * current1) > (CUTOFF_POWER_FAST * 1000)) { nextOutputs &= 0b11110; }
            if (((uint32_t)voltage2 * current2) > (CUTOFF_POWER_FAST * 1000)) { nextOutputs &= 0b11101; }
            if (((uint32_t)voltage3 * current3) > (CUTOFF_POWER_FAST * 1000)) { nextOutputs &= 0b11011; }
            if (((uint32_t)voltage4 * current4) > (CUTOFF_POWER_FAST * 1000)) { nextOutputs &= 0b10111; }
            if (((uint32_t)voltage5 * current5) > (CUTOFF_POWER_FAST * 1000)) { nextOutputs &= 0b01111; }

            // calculate average from previous tick
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

            // cutoff if exceeding average current
            if (current1Avg > CUTOFF_CURRENT_AVG) { nextOutputs &= 0b11110; }
            if (current2Avg > CUTOFF_CURRENT_AVG) { nextOutputs &= 0b11101; }
            if (current3Avg > CUTOFF_CURRENT_AVG) { nextOutputs &= 0b11011; }
            if (current4Avg > CUTOFF_CURRENT_AVG) { nextOutputs &= 0b10111; }
            if (current5Avg > CUTOFF_CURRENT_AVG) { nextOutputs &= 0b01111; }

            // cutoff if exceeding average power
            if (((uint32_t)voltage1Avg * current1Avg) > (CUTOFF_POWER_AVG * 1000)) { nextOutputs &= 0b11110; }
            if (((uint32_t)voltage2Avg * current2Avg) > (CUTOFF_POWER_AVG * 1000)) { nextOutputs &= 0b11101; }
            if (((uint32_t)voltage3Avg * current3Avg) > (CUTOFF_POWER_AVG * 1000)) { nextOutputs &= 0b11011; }
            if (((uint32_t)voltage4Avg * current4Avg) > (CUTOFF_POWER_AVG * 1000)) { nextOutputs &= 0b10111; }
            if (((uint32_t)voltage5Avg * current5Avg) > (CUTOFF_POWER_AVG * 1000)) { nextOutputs &= 0b01111; }

            // add new value to averages
            voltage1Sum -= voltage1Avg; voltage1Sum += voltage1;
            voltage2Sum -= voltage2Avg; voltage2Sum += voltage2;
            voltage3Sum -= voltage3Avg; voltage3Sum += voltage3;
            voltage4Sum -= voltage4Avg; voltage4Sum += voltage4;
            voltage5Sum -= voltage5Avg; voltage5Sum += voltage5;
            current1Sum -= current1Avg; current1Sum += current1;
            current2Sum -= current2Avg; current2Sum += current2;
            current3Sum -= current3Avg; current3Sum += current3;
            current4Sum -= current4Avg; current4Sum += current4;
            current5Sum -= current5Avg; current5Sum += current5;
            temperatureSum -= temperatureAvg; temperatureSum += temperature;

            // check if state needs change
            if (currOutputs != nextOutputs) {
                currOutputs = nextOutputs;
                bool nextState1 = (currOutputs & 0b00001) != 0;
                bool nextState2 = (currOutputs & 0b00010) != 0;
                bool nextState3 = (currOutputs & 0b00100) != 0;
                bool nextState4 = (currOutputs & 0b01000) != 0;
                bool nextState5 = (currOutputs & 0b10000) != 0;
                ioex_output_setAll(nextState1, nextState2, nextState3, nextState4, nextState5);  // set outputs
                ioex_button_led_setAll(nextState1, nextState2, nextState3, nextState4, nextState5);  // set LEDs
                settings_outputs_set(nextOutputs);  // save for the next startup
            }

            // check buttons each tick
            ioex_button_switch_getAll(&switch1State, &switch2State, &switch3State, &switch4State, &switch5State);
            prevButtonMask = currButtonMask;
            currButtonMask = 0;
            uint8_t currButtonMaskCount = 0;
            if (switch1State) { currButtonMask |= 0b00001; currButtonMaskCount++; }
            if (switch2State) { currButtonMask |= 0b00010; currButtonMaskCount++; }
            if (switch3State) { currButtonMask |= 0b00100; currButtonMaskCount++; }
            if (switch4State) { currButtonMask |= 0b01000; currButtonMaskCount++; }
            if (switch5State) { currButtonMask |= 0b10000; currButtonMaskCount++; }
            if ((currButtonMaskCount == 1) && (currButtonMask == prevButtonMask)) { currDepthButtonTicks++; } else { currDepthButtonTicks = 0; }
            uint8_t currChannelButtonMask;
            switch (currChannel) {
                case 1: currChannelButtonMask = 0b00001; break;
                case 2: currChannelButtonMask = 0b00010; break;
                case 3: currChannelButtonMask = 0b00100; break;
                case 4: currChannelButtonMask = 0b01000; break;
                case 5: currChannelButtonMask = 0b10000; break;
                default: currChannelButtonMask = 0; break;
            }

            // state changes
            if (currDepth != nextDepth) {
                currDepth = nextDepth;
                currDepthTicks = 0;
                currDepthButtonTicks = 0;
                tickCounter = 0;  // force update
            }

            bool currBlinking = false;
            if (currDepth == DEPTH_PENDING_RESET) {
                switch (currChannel) {  // blinking while waiting for reset (12/s)
                    case 1: ioex_button_led_toggle1(); break;
                    case 2: ioex_button_led_toggle2(); break;
                    case 3: ioex_button_led_toggle3(); break;
                    case 4: ioex_button_led_toggle4(); break;
                    case 5: ioex_button_led_toggle5(); break;
                }
                currBlinking = true;
            } else if (currDepth == DEPTH_PENDING_OFF) {
                if ((tickCounter % 6) == 0) {  // blinking while checking off is (4/s)
                    switch (currChannel) {
                        case 1: ioex_button_led_toggle1(); break;
                        case 2: ioex_button_led_toggle2(); break;
                        case 3: ioex_button_led_toggle3(); break;
                        case 4: ioex_button_led_toggle4(); break;
                        case 5: ioex_button_led_toggle5(); break;
                    }
                    currBlinking = true;
                }
            } else if (currDepth == DEPTH_DETAILS) {
                if ((currButtonMask > 0) && ((tickCounter % 12) == 0)) {  // blink while waiting for reset (1/s)
                    switch (currChannel) {
                        case 1: ioex_button_led_toggle1(); break;
                        case 2: ioex_button_led_toggle2(); break;
                        case 3: ioex_button_led_toggle3(); break;
                        case 4: ioex_button_led_toggle4(); break;
                        case 5: ioex_button_led_toggle5(); break;
                    }
                    currBlinking = true;
                }
            }
            if (prevBlinking != currBlinking) {
                if (!currBlinking && prevBlinking) {  // reset LEDs if blinking is over
                    ioex_button_led_setAll(
                        (currOutputs & 0b00001) != 0,
                        (currOutputs & 0b00010) != 0,
                        (currOutputs & 0b00100) != 0,
                        (currOutputs & 0b01000) != 0,
                        (currOutputs & 0b10000) != 0
                    );
                }
                prevBlinking = currBlinking;
            }

            switch (currDepth) {
                case DEPTH_SUMMARY: {
                    switch (currButtonMask) {  // switch to details
                        case 0b00001: nextDepth = DEPTH_DETAILS; currChannel = 1; break;
                        case 0b00010: nextDepth = DEPTH_DETAILS; currChannel = 2; break;
                        case 0b00100: nextDepth = DEPTH_DETAILS; currChannel = 3; break;
                        case 0b01000: nextDepth = DEPTH_DETAILS; currChannel = 4; break;
                        case 0b10000: nextDepth = DEPTH_DETAILS; currChannel = 5; break;
                    }
                } break;

                case DEPTH_DETAILS: {
                    if (currButtonMask == 0) {  // button has been released
                        if ((prevButtonMask == currChannelButtonMask) && (currDepthButtonTicks < 12)) {  // short double-click brings back to summary
                            if (currDepthTicks > TICKS_IGNORE_DETAILS) {
                                currDepthTicks = 0;
                            } else {
                                nextDepth = DEPTH_SUMMARY;
                            }
                        } else if (currDepthTicks > TICKS_CLOSE_DETAILS) {  // exit after 1 minute automatically
                            nextDepth = DEPTH_SUMMARY;
                        }
                    } else {
                        if (currChannelButtonMask == currButtonMask) {
                            if (currDepthButtonTicks > TICKS_GOTO_RESET)  {  // held longer than 5 seconds
                                nextDepth = DEPTH_PENDING_RESET;
                            }
                        } else {
                            ioex_button_led_setAll(currOutputs & 0b00001, currOutputs & 0b00010, currOutputs & 0b00100, currOutputs & 0b01000, currOutputs & 0b10000);  // reset lights
                            switch (currButtonMask) {  // switch to other details
                                case 0b00001: nextDepth = DEPTH_DETAILS; currChannel = 1; break;
                                case 0b00010: nextDepth = DEPTH_DETAILS; currChannel = 2; break;
                                case 0b00100: nextDepth = DEPTH_DETAILS; currChannel = 3; break;
                                case 0b01000: nextDepth = DEPTH_DETAILS; currChannel = 4; break;
                                case 0b10000: nextDepth = DEPTH_DETAILS; currChannel = 5; break;
                            }
                        }
                    }
                } break;

                case DEPTH_PENDING_RESET: {
                    if (currButtonMask == 0) {  // button has been released
                        nextOutputs = resetOutput(currChannel, nextOutputs);
                        nextDepth = DEPTH_PENDING_NOTHING;
                    } else if (currChannelButtonMask == currButtonMask) {
                        if (currDepthButtonTicks > TICKS_WAIT_RESET)  {  // held longer than 5 seconds
                            nextDepth = DEPTH_PENDING_OFF;
                        }
                    }
                } break;

                case DEPTH_PENDING_OFF: {
                    if (currButtonMask == 0) {  // button has been released
                        nextOutputs = disableOutputMask(currChannel, nextOutputs);
                        nextDepth = DEPTH_PENDING_NOTHING;
                    } else if (currChannelButtonMask == currButtonMask) {
                        if (currDepthButtonTicks > TICKS_WAIT_OFF)  {  // held longer than 3 seconds
                            nextDepth = DEPTH_PENDING_NOTHING;
                        }
                    }
                } break;

                case DEPTH_PENDING_NOTHING: {
                    if (currButtonMask == 0) {  // button has been released
                        nextDepth = DEPTH_SUMMARY;
                    }
                } break;
            }

            // update display twice a second
            if ((tickCounter == 0) || (tickCounter == 12)) {
                io_led_activity_on();

                switch (currDepth) {
                    case DEPTH_SUMMARY: {
                        oled_writeSummary(voltage1Avg, current1Avg, voltage2Avg, current2Avg, voltage3Avg, current3Avg, voltage4Avg, current4Avg, voltage5Avg, current5Avg, temperatureAvg, currOutputs);
                    } break;

                    case DEPTH_DETAILS:
                    case DEPTH_PENDING_RESET:
                    case DEPTH_PENDING_OFF:
                    case DEPTH_PENDING_NOTHING: {
                        switch (currChannel) {
                            case 1: {
                                oled_writeChannel(1, voltage1Avg, current1Avg, currOutputs & 0b00001, currDepth, currDepthButtonTicks);
                            } break;
                            case 2: {
                                oled_writeChannel(2, voltage2Avg, current2Avg, currOutputs & 0b00010, currDepth, currDepthButtonTicks);
                            } break;
                            case 3: {
                                oled_writeChannel(3, voltage3Avg, current3Avg, currOutputs & 0b00100, currDepth, currDepthButtonTicks);
                            } break;
                            case 4: {
                                oled_writeChannel(4, voltage4Avg, current4Avg, currOutputs & 0b01000, currDepth, currDepthButtonTicks);
                            } break;
                            case 5: {
                                oled_writeChannel(5, voltage5Avg, current5Avg, currOutputs & 0b10000, currDepth, currDepthButtonTicks);
                            } break;
                        }

                    } break;
                }
            } else if ((tickCounter == 6) || (tickCounter == 18)) {  // prepare UART twice a second
                uartbuffers_txReset();
                fillUartFromChannel(1, (uint16_t)(voltage1Sum >> AVG_SHIFT), (uint16_t)(current1Sum >> AVG_SHIFT), (currOutputs & 0b00001) > 0);
                fillUartFromChannel(2, (uint16_t)(voltage2Sum >> AVG_SHIFT), (uint16_t)(current2Sum >> AVG_SHIFT), (currOutputs & 0b00010) > 0);
                fillUartFromChannel(3, (uint16_t)(voltage3Sum >> AVG_SHIFT), (uint16_t)(current3Sum >> AVG_SHIFT), (currOutputs & 0b00100) > 0);
                fillUartFromChannel(4, (uint16_t)(voltage4Sum >> AVG_SHIFT), (uint16_t)(current4Sum >> AVG_SHIFT), (currOutputs & 0b01000) > 0);
                fillUartFromChannel(5, (uint16_t)(voltage5Sum >> AVG_SHIFT), (uint16_t)(current5Sum >> AVG_SHIFT), (currOutputs & 0b10000) > 0);
                fillUartFromTemperature((uint16_t)(temperatureSum >> AVG_SHIFT));
            }
        }
    }
}

void fillUartFromChannel(uint8_t index, uint16_t voltage, uint16_t current, bool isOn) {
    uint32_t power = (uint32_t)voltage * (uint32_t)current / 1000;

    uartbuffers_txAppend(0x30 + index);
    uartbuffers_txAppend(':');
    uartbuffers_txAppend(' ');

    uint8_t buffer[5];
    uint8_t bufferIndex;

    voltage /= 10;  // we don't need mV
    bufferIndex = sizeof(buffer);
    while (bufferIndex > 0) {
        bufferIndex--;
        buffer[bufferIndex] = 0x30 + (voltage % 10);
        voltage /= 10;
        if ((voltage == 0) && (bufferIndex < 3)) { break; }  // we're past decimal point and there are no more digits
    }
    for (uint8_t i = 1; i < sizeof(buffer); i++) {
        if (i < bufferIndex) {
            uartbuffers_txAppend(' ');
        } else {
            uartbuffers_txAppend(buffer[i]);
            if (i == 2) { uartbuffers_txAppend('.'); }
        }
    }
    uartbuffers_txAppend('V');
    uartbuffers_txAppend(' ');
    
    current /= 10;  // we don't need mA
    bufferIndex = sizeof(buffer);
    while (bufferIndex > 0) {
        bufferIndex--;
        buffer[bufferIndex] = 0x30 + (current % 10);
        current /= 10;
        if ((current == 0) && (bufferIndex < 3)) { break; }  // we're past decimal point and there are no more digits
    }
    for (uint8_t i = 2; i < sizeof(buffer); i++) {
        if (i < bufferIndex) {
            uartbuffers_txAppend(' ');
        } else {
            uartbuffers_txAppend(buffer[i]);
            if (i == 2) { uartbuffers_txAppend('.'); }
        }
    }
    uartbuffers_txAppend('A');
    uartbuffers_txAppend(' ');

    power /= 10;  // we don't need mW
    bufferIndex = sizeof(buffer);
    while (bufferIndex > 0) {
        bufferIndex--;
        buffer[bufferIndex] = 0x30 + (power % 10);
        power /= 10;
        if ((power == 0) && (bufferIndex < 3)) { break; }  // we're past decimal point and there are no more digits
    }
    for (uint8_t i = 0; i < sizeof(buffer); i++) {
        if (i < bufferIndex) {
            uartbuffers_txAppend(' ');
        } else {
            uartbuffers_txAppend(buffer[i]);
            if (i == 2) { uartbuffers_txAppend('.'); }
        }
    }
    uartbuffers_txAppend('W');
    uartbuffers_txAppend(' ');

    uartbuffers_txAppend('[');
    if (isOn) {
        uartbuffers_txAppend('O');
        uartbuffers_txAppend('N');
    } else {
        uartbuffers_txAppend('O');
        uartbuffers_txAppend('F');
        uartbuffers_txAppend('F');
    }
    uartbuffers_txAppend(']');

    uartbuffers_txAppend('\r');
    uartbuffers_txAppend('\n');
}

void fillUartFromTemperature(uint16_t temperature) {
    uartbuffers_txAppend('T');
    uartbuffers_txAppend(':');
    uartbuffers_txAppend(' ');

    uint8_t buffer[3];
    uint8_t bufferIndex;

    bufferIndex = sizeof(buffer);
    while (bufferIndex > 0) {
        bufferIndex--;
        buffer[bufferIndex] = 0x30 + (temperature % 10);
        temperature /= 10;
        if ((temperature == 0) && (bufferIndex < 2)) { break; }  // we're past decimal point and there are no more digits
    }
    for (uint8_t i = 0; i < sizeof(buffer); i++) {
        if (i < bufferIndex) {
            uartbuffers_txAppend(' ');
        } else {
            uartbuffers_txAppend(buffer[i]);
            if (i == 1) { uartbuffers_txAppend('.'); }
        }
    }
    uartbuffers_txAppend('C');

    uartbuffers_txAppend('\r');
    uartbuffers_txAppend('\n');
}

uint8_t disableOutputMask(const uint8_t channel, const uint8_t currOutputs) {
    uint8_t nextOutputs = currOutputs;

    switch (channel) {  // channel OFF
        case 1: nextOutputs &= 0b11110; break;
        case 2: nextOutputs &= 0b11101; break;
        case 3: nextOutputs &= 0b11011; break;
        case 4: nextOutputs &= 0b10111; break;
        case 5: nextOutputs &= 0b01111; break;
    }
    return nextOutputs;
}

uint8_t enableOutputMask(const uint8_t channel, const uint8_t currOutputs) {
    uint8_t nextOutputs = currOutputs;

    switch (channel) {  // channel ON
        case 1: nextOutputs |= 0b00001; break;
        case 2: nextOutputs |= 0b00010; break;
        case 3: nextOutputs |= 0b00100; break;
        case 4: nextOutputs |= 0b01000; break;
        case 5: nextOutputs |= 0b10000; break;
    }
    return nextOutputs;
}

uint8_t resetOutput(const uint8_t channel, const uint8_t currOutputs) {
    uint8_t nextOutputs = currOutputs;

    switch (channel) {  // channel OFF
        case 1: ioex_output_set1(false); ioex_button_led_set1(false); break;
        case 2: ioex_output_set2(false); ioex_button_led_set2(false); break;
        case 3: ioex_output_set3(false); ioex_button_led_set3(false); break;
        case 4: ioex_output_set4(false); ioex_button_led_set4(false); break;
        case 5: ioex_output_set5(false); ioex_button_led_set5(false); break;
    }

    uint8_t resetTicks = 0;
    while (true) {
        CLRWDT();
        if (ticker_hasTicked()) {
            resetTicks++;
            if (resetTicks % 12 == 0) { oled_writeReset(channel, resetTicks); }
            if (resetTicks >= TICKS_DURATION_RESET) { break; }
        }
    }

    switch (channel) {  // channel ON
        case 1: ioex_output_set1(true); ioex_button_led_set1(true); nextOutputs |= 0b00001; break;
        case 2: ioex_output_set2(true); ioex_button_led_set2(true); nextOutputs |= 0b00010; break;
        case 3: ioex_output_set3(true); ioex_button_led_set3(true); nextOutputs |= 0b00100; break;
        case 4: ioex_output_set4(true); ioex_button_led_set4(true); nextOutputs |= 0b01000; break;
        case 5: ioex_output_set5(true); ioex_button_led_set5(true); nextOutputs |= 0b10000; break;
    }

    return nextOutputs;
}


void test(void) {
    ssd1306_displayInvert();
    bool switch1State, switch2State, switch3State, switch4State, switch5State;
    while(true) {
        CLRWDT();
        ioex_button_switch_getAll(&switch1State, &switch2State, &switch3State, &switch4State, &switch5State);
        if (!(switch1State | switch2State | switch3State | switch4State | switch5State)) { break; }
    }
    ssd1306_displayNormal();
    ticker_waitTick();

    uint8_t updateCounter = 0;
    uint8_t index = 0;

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
                    case 0: oled_writeTestVT(index, (uint16_t)(voltage12VSum >> AVG_SHIFT), (uint16_t)(temperatureSum >> AVG_SHIFT)); break;    // 12V + Temperature
                    case 1: oled_writeTestVC(1, (uint16_t)(voltage1Sum >> AVG_SHIFT), (uint16_t)(current1Sum >> AVG_SHIFT)); break;             // Channel 1: Voltage + Current
                    case 2: oled_writeTestVC(2, (uint16_t)(voltage2Sum >> AVG_SHIFT), (uint16_t)(current2Sum >> AVG_SHIFT)); break;             // Channel 2: Voltage + Current
                    case 3: oled_writeTestVC(3, (uint16_t)(voltage3Sum >> AVG_SHIFT), (uint16_t)(current3Sum >> AVG_SHIFT)); break;             // Channel 3: Voltage + Current
                    case 4: oled_writeTestVC(4, (uint16_t)(voltage4Sum >> AVG_SHIFT), (uint16_t)(current4Sum >> AVG_SHIFT)); break;             // Channel 4: Voltage + Current
                    case 5: oled_writeTestVC(5, (uint16_t)(voltage5Sum >> AVG_SHIFT), (uint16_t)(current5Sum >> AVG_SHIFT)); break;             // Channel 5: Voltage + Current
                    case 6: oled_writeTestVT(index, (uint16_t)(voltage5VSum >> AVG_SHIFT), (uint16_t)(temperatureDieSum >> AVG_SHIFT)); break;  // Internal Voltage + Internal Temperature
                }

                ioex_button_switch_getAll(&switch1State, &switch2State, &switch3State, &switch4State, &switch5State);
                if (switch1State | switch2State | switch3State | switch4State | switch5State) {
                    ssd1306_displayInvert();
                    index = (index + 1) % 7;
                    while(true) {
                        CLRWDT();
                        ioex_button_switch_getAll(&switch1State, &switch2State, &switch3State, &switch4State, &switch5State);
                        if (!(switch1State | switch2State | switch3State | switch4State | switch5State)) { break; }
                    }
                    ssd1306_displayNormal();
                    ticker_waitTick();
                    switch(index) {
                        case 0: ioex_button_led_setAll(false, false, false, false, false); ioex_output_setAll(false, false, false, false, false); break;
                        case 1: ioex_button_led_setAll(true, false, false, false, false); ioex_output_setAll(true, false, false, false, false); break;
                        case 2: ioex_button_led_setAll(false, true, false, false, false); ioex_output_setAll(false, true, false, false, false); break;
                        case 3: ioex_button_led_setAll(false, false, true, false, false); ioex_output_setAll(false, false, true, false, false); break;
                        case 4: ioex_button_led_setAll(false, false, false, true, false); ioex_output_setAll(false, false, false, true, false); break;
                        case 5: ioex_button_led_setAll(false, false, false, false, true); ioex_output_setAll(false, false, false, false, true); break;
                        case 6: ioex_button_led_setAll(false, false, false, false, false); ioex_output_setAll(false, false, false, false, false); break;
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
