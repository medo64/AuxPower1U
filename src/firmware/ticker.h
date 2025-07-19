/* Josip Medved <jmedved@jmedved.com> * www.medo64.com * MIT License */
// 2024-09-21: Added support for PIC18F25K83 and PIC18F26K83
// 2023-10-11: Refactoring

/**
 * Timer that "ticks" 24 times a second
 * 
 * Defines used:
 *   _XTAL_FREQ <N>:               Frequency in Hz (only if not using external I2C master)
 *   _TICKER_USE_TIMER0:           Use TMR0 for time management (default)
 *   _TICKER_NO_WAIT_TICK:         Don't expose ticker_waitTick function
 *   _TICKER_NO_WAIT_TICKS:        Don't expose ticker_waitTicks function
 */

#pragma once

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "app.h"

#if !defined(_16F1454) && !defined(_16F1455) && !defined(_18F25K83) && !defined(_18F26K83)
    #error "Unsupported device"
#endif

#if !defined(_TICKER_USE_TIMER0)
    #define _TICKER_USE_TIMER0
#endif


/** Initialize timer0 */
void ticker_init(void);

/** Reset timer */
void ticker_reset(void);

/** Check if timer was triggered (24tps) since last check */
inline bool ticker_hasTicked(void);

#if !defined(_TICKER_NO_WAIT_TICK)
    /** waits for a full tick (1/24th of a second) */
    void ticker_waitTick(void);
#endif

#if !defined(_TICKER_NO_WAIT_TICKS)
    /** waits N ticks (1/24th of a second each) */
    void ticker_waitTicks(uint8_t tickCount);
#endif
