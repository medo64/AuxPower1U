/* Josip Medved <jmedved@jmedved.com> * www.medo64.com * MIT License */
// 2024-10-13: Added higher speed modes
// 2024-10-11: Removed PPS code
// 2024-09-23: Initial version

/**
 * Handling I2C master communication
 *
 * Defines used:
 *   _I2C_MASTER_MODULE2:          Use module 2 (where PIC supports it)
 *   _I2C_MASTER_RATE_KHZ <value>: If used, sets I2C speed to defined value; requires Timer 6
 *
 * Notes:
 *   Both CLOCK and DATA pin has to be configured as input
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

#if !defined(_16F1454) && !defined(_16F1455) && !defined(_18F25K83) && !defined(_18F26K83)
    #error "Unsupported device"
#endif

#if defined(_I2C_MASTER_RATE_KHZ)
    #if !defined(_I2C_MASTER_RATE_KHZ)
        #error "Must have _XTAL_FREQ defined"
    #endif
    #if (_I2C_MASTER_RATE_KHZ < 100) || (_I2C_MASTER_RATE_KHZ > 1000)
        #error "I2C rate not in 100-1000 range"
    #endif
#endif


#if defined(_16F1454) || defined(_16F1455)  // RC0 RC1
    /** Initializes I2C as a master. */
    void i2c_master_init(uint8_t baudRateCounter);
#elif defined(_18F25K83) || defined(_18F26K83)
    /** Initializes I2C as a master. */
    void i2c_master_init(void);
#endif

/** Reads multiple bytes from a register. */
bool i2c_master_readRegisterBytes(const uint8_t deviceAddress, const uint8_t registerAddress, uint8_t* readData, const uint8_t readCount);

/** Writes multiple bytes. */
bool i2c_master_writeRegisterBytes(const uint8_t deviceAddress, const uint8_t registerAddress, const uint8_t* data, const uint8_t count);

/** Writes multiple bytes. */
bool i2c_master_writeRegisterZeroBytes(const uint8_t deviceAddress, const uint8_t registerAddress, const uint8_t zeroCount);

/** Writes multiple bytes. */
bool i2c_master_writeBytes(const uint8_t deviceAddress, const uint8_t* data, const uint8_t count);

/** Writes multiple bytes. */
bool i2c_master_writeZeroBytes(const uint8_t deviceAddress, const uint8_t zeroCount);
