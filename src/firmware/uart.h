/* Josip Medved <jmedved@jmedved.com> * www.medo64.com * MIT License */
// 2024-10-12: Fully refactored
// 2024-10-11: PIC18F25K83 version

/**
 * Handling UART output
 *
 * Defines used:
 *   _UART_CUSTOM_INIT            Uses customizable initialization function
 *   _UART_BAUD_RATE <N>          Baud rate; default is 9600
 *   _UART_MODULE2                Use module 2; where PIC supports it
 *   _UART_EXTENDED_WRITE         Additional write functions
 */

#pragma once
#include <xc.h>
#include <stdbool.h>
#include <stdint.h>


#if defined(_UART_CUSTOM_INIT)
    /** Initializes UART. */
    void uart_init(uint32_t desiredBaudRate);
#else
    /** Initializes UART. */
    void uart_init(void);
#endif


#if defined(_UART_MODULE2)
    /** Returns true if read buffer is empty. */
    #define uart_isReadEmpty()  (U2FIFObits.RXBE != 0)

    /** Returns true if write buffer is full. */
    #define uart_isWriteFull()  (U2FIFObits.TXBF != 0)
#else
    /** Returns true if read buffer is empty. */
    #define uart_isReadEmpty()  (U1FIFObits.RXBE != 0)

    /** Returns true if write buffer is full. */
    #define uart_isWriteFull()  (U1FIFObits.TXBF != 0)
#endif


/** Tries to read byte. Returns true if successful. */
bool uart_tryReadByte(uint8_t* data);

/** Tries to write byte. Returns true if successful. */
bool uart_tryWriteByte(const uint8_t data);


#if defined(_UART_EXTENDED_WRITE)

    /** Blocking write of a multiple bytes. */
    void uart_writeBytes(const uint8_t *data, const uint8_t count);

    /** Blocking write of a null terminated string. */
    void uart_writeString(const char* text);


    /** Blocking write of a 8-bit unsigned integer. */
    void uart_writeInteger8(const uint8_t number);

    /** Blocking write of a 16-bit unsigned integer. */
    void uart_writeInteger16(const uint16_t number);

    /** Blocking write of a hex digit. */
    void uart_writeHex4(const uint8_t value);

    /** Blocking write of a byte as two hex digits. */
    void uart_writeHex8(const uint8_t value);

#endif
