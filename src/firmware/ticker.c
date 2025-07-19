#include <xc.h>
#include <stdbool.h>
#include "app.h"
#include "ticker.h"

#if !defined (_XTAL_FREQ)
    #error Unknown frequency
#endif

#if defined(_16F1454) || defined(_16F1455)
    #if (_XTAL_FREQ > 48000000)
        #define TICKER_TIMER0_PRESCALER  0b011  // 1:16
        #define TICKER_TIMER0_TARGET     (_XTAL_FREQ / 4 / (1 << (TICKER_TIMER0_PRESCALER + 1)) / 24 / 256)  // 162 @ 64 MHz
        #define TICKER_TIMER0_START      ((_XTAL_FREQ - (TICKER_TIMER0_TARGET * 4 * (1 << (TICKER_TIMER0_PRESCALER + 1)) * 24 * 256)) / 4 / (1 << (TICKER_TIMER0_PRESCALER + 1)) / 24)  // 194 @ 64 MHz
    #elif (_XTAL_FREQ > 32000000)
        #define TICKER_TIMER0_PRESCALER  0b010  // 1:8
        #define TICKER_TIMER0_TARGET     (_XTAL_FREQ / 4 / (1 << (TICKER_TIMER0_PRESCALER + 1)) / 24 / 256)  // 244 @ 48 MHz
        #define TICKER_TIMER0_START      ((_XTAL_FREQ - (TICKER_TIMER0_TARGET * 4 * (1 << (TICKER_TIMER0_PRESCALER + 1)) * 24 * 256)) / 4 / (1 << (TICKER_TIMER0_PRESCALER + 1)) / 24)  // 36 @ 48 MHz
    #endif
#elif defined(_18F25K83) || defined(_18F26K83)
    #if (_XTAL_FREQ > 48000000)
        #define TICKER_TIMER0_PRESCALER  0b0100  // 1:16
        #define TICKER_TIMER0_TARGET     (_XTAL_FREQ / 4 / (1 << TICKER_TIMER0_PRESCALER) / 24 / 256)  // 162 @ 64 MHz
        #define TICKER_TIMER0_START      ((_XTAL_FREQ - (TICKER_TIMER0_TARGET * 4 * (1 << TICKER_TIMER0_PRESCALER) * 24 * 256)) / 4 / (1 << TICKER_TIMER0_PRESCALER) / 24)  // 194 @ 64 MHz
    #elif (_XTAL_FREQ > 32000000)
        #define TICKER_TIMER0_PRESCALER  0b0100  // 1:8
        #define TICKER_TIMER0_TARGET     (_XTAL_FREQ / 4 / (1 << TICKER_TIMER0_PRESCALER) / 24 / 256)  // 244 @ 48 MHz
        #define TICKER_TIMER0_START      ((_XTAL_FREQ - (TICKER_TIMER0_TARGET * 4 * (1 << TICKER_TIMER0_PRESCALER) * 24 * 256)) / 4 / (1 << TICKER_TIMER0_PRESCALER) / 24)  // 36 @ 48 MHz
    #endif
#endif

#if TICKER_TIMER0_START < 0
    #define TICKER_TIMER0_START         0
#endif
#if !defined(TICKER_TIMER0_TARGET)
    #error Cannot determine postscaler target
#elif TICKER_TIMER0_TARGET > 255
    #error Cannot determine postscaler target (scaler too small)
#endif

uint8_t counter = 0;

void ticker_init(void) {
    #if defined(_16F1454) || defined(_16F1455)
        OPTION_REGbits.TMR0CS = 0; // FOSC/4
        #if defined(TICKER_TIMER0_PRESCALER)
            OPTION_REGbits.PSA = 0; // Prescaler is assigned to the Timer0 module
            OPTION_REGbits.PS = TICKER_TIMER0_PRESCALER;
        #else
            OPTION_REGbits.PSA = 1; // Prescaler is not assigned to the Timer0 module
        #endif
        TMR0 = TICKER_TIMER0_START;
    #elif defined(_18F25K83) || defined(_18F26K83)
        T0CON1bits.CS = 0b010; // FOSC/4
        T0CON1bits.CKPS = TICKER_TIMER0_PRESCALER;
        TMR0 = TICKER_TIMER0_START;
        T0CON0bits.EN = 1; // 1 = The module is enabled and operating
    #endif
}

void ticker_reset(void) {
    TMR0 = TICKER_TIMER0_START;
    counter = 0;
}

inline bool ticker_hasTicked(void) { // 24x per second
    if (TMR0IF) {
        TMR0IF = 0;
        if (counter == TICKER_TIMER0_TARGET) {
            counter = 0;
            TMR0 = TICKER_TIMER0_START;
            return true;
        }
        counter++;
    }
    return false;
}


#if !defined(_TICKER_NO_WAIT_TICK)
    void ticker_waitTick(void) {
        ticker_reset();
        while (!ticker_hasTicked()) { CLRWDT(); }
    }
#endif

#if !defined(_TICKER_NO_WAIT_TICKS)
    void ticker_waitTicks(uint8_t tickCount) {
        ticker_reset();
        while (true) {
            if (tickCount == 0) { break; }
            while (!ticker_hasTicked()) { CLRWDT(); }
            tickCount--;
        }
    }
#endif