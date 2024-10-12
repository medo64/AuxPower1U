/* Josip Medved <jmedved@jmedved.com> * www.medo64.com * MIT License */
// 2024-10-11: PIC18F25K83 version

/**
 * Handling UART output
 *
 * Defines used:
 *   _UART_CUSTOM_INIT            Uses customizable initialization function
 *   _UART_BAUD_RATE <N>          Baud rate; default is 9600
 *   _UART_MODULE2                Use module 2; where PIC supports it
 */

#pragma once
#include <stdbool.h>
#include <stdint.h>


#if defined(_UART_CUSTOM_INIT)
    /** Initializes UART. */
    void uart_init(uint32_t desiredBaudRate);
#else
    /** Initializes UART. */
    void uart_init(void);
#endif


/** Returns true if there is data waiting to be read. */
bool uart_canRead(void);

/** Returns true if there is no data currently being written. */
bool uart_canWrite(void);


/** Blocking read of a single byte. */
uint8_t uart_readByte(void);

/** Blocking write of a single byte. */
void uart_writeByte(const uint8_t data);


/** Tries to read byte. Returns true if successful. */
bool uart_tryReadByte(uint8_t* data);

/** Tries to write byte. Returns true if successful. */
bool uart_tryWriteByte(const uint8_t data);


/** Blocking write of a multiple bytes. */
void uart_writeBytes(const uint8_t *data, const uint8_t count);

/** Blocking write of a null terminated string. */
void uart_writeString(const char* text);


/** Blocking write of a 8-bit unsigned integer. */
void uart_writeUInt8(const uint8_t number);

/** Blocking write of a 16-bit unsigned integer. */
void uart_writeUInt16(const uint16_t number);

/** Blocking write of a hex digit. */
void uart_writeHexDigit(const uint8_t value);

/** Blocking write of a byte as two hex digits. */
void uart_writeHexUInt8(const uint8_t value);
