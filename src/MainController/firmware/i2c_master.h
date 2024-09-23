/* Josip Medved <jmedved@jmedved.com> * www.medo64.com * MIT License */
// TODO .....: Initial version

/**
 * Handling I2C master communication
 *
 * Defines used:
 *   _I2C_MASTER_MODULE2:          Use module 2 (where PIC supports it)
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


/** Initializes I2C as a master. */
void i2c_master_init(void);

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
