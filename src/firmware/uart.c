#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "app.h"
#include "uart.h"


void uart_setup(uint32_t desiredBaudRate);

#if defined(_UART_CUSTOM_INIT)
    void uart_init(uint32_t desiredBaudRate) {
        uart_setup(desiredBaudRate);
    }
#else
    void uart_init() {
        #if defined(_UART_BAUD_RATE)
            uart_setup(_UART_BAUD_RATE);
        #else
            uart_setup(9600);
        #endif
    }
#endif


bool uart_tryReadByte(uint8_t* data) {
    if (uart_isReadEmpty()) { return false; }
    #if defined(_UART_MODULE2)
        *data = U2RXB;
    #else
        *data = U1RXB;
    #endif
    return true;
}

bool uart_tryWriteByte(const uint8_t data) {
    if (uart_isWriteFull()) { return false; }
    #if defined(_UART_MODULE2)
        U2TXB = data;
    #else
        U1TXB = data;
    #endif
    return true;
}


#if defined(_UART_EXTENDED_WRITE)

    void uart_writeByte(const uint8_t data) {
        while (uart_isWriteFull()) { };
        #if defined(_UART_MODULE2)
            U2TXB = data;
        #else
            U1TXB = data;
        #endif
    }

    void uart_writeBytes(const uint8_t *data, const uint8_t count) {
        for (uint8_t i = 0; i < count; i++) {
            uart_writeByte(data[i]);
        }
    }

    void uart_writeString(const char* text) {
        for (uint8_t i = 0; i < 255; i++) { //allow max 255 characters
            if (text[i] == '\0') {
                break;
            } else {
                uart_writeByte(text[i]);
            }
        }
    }


    void uart_writeInteger8(const uint8_t value) {
        uint8_t value2 = value;
        uint8_t chars[3] = { 0 };
        uint8_t i = 0;
        for (; i < sizeof(chars); i++) {
            chars[i] = 0x30 + (value2 % 10);
            value2 /= 10;
            if (value2 == 0) { break; }
        }
        for (; i != 255; i--) {
            if (chars[i] != 0) { uart_writeByte(chars[i]); }
        }
    }

    void uart_writeInteger16(const uint16_t value) {
        uint16_t value2 = value;
        uint8_t chars[5] = { 0 };
        uint8_t i = 0;
        for (; i < sizeof(chars); i++) {
            chars[i] = 0x30 + (value2 % 10);
            value2 /= 10;
            if (value2 == 0) { break; }
        }
        for (; i != 255; i--) {
            if (chars[i] != 0) { uart_writeByte(chars[i]); }
        }
    }

    void uart_writeHex4(const uint8_t value) {
        uint8_t data = 0x30 + (value & 0x0F);
        if (data > 0x39) { data += 7; }
        uart_writeByte(data);
    }

    void uart_writeHex8(const uint8_t value) {
        uart_writeHex4(value >> 4);
        uart_writeHex4(value);
    }

#endif


#if !defined(_XTAL_FREQ)
    #error "Must define _XTAL_FREQ";
#endif

void uart_setup(uint32_t desiredBaudRate) {
    #if defined(_UART_MODULE2)
        U2CON0bits.BRGS = 1;  // Baud rate generator is high rate
        U2BRG = (uint16_t)(_XTAL_FREQ / desiredBaudRate / 4 - 1);

        U2CON0bits.MODE  = 0b0000;  // asynchronous mode
        U2CON0bits.TXEN = 1;  // enable transmit
        U2CON0bits.RXEN = 1;  // enable receive

        U2CON1bits.ON = 1;  // enable USART
    #else
        U1CON0bits.BRGS = 1;  // Baud rate generator is high rate
        U1BRG = (uint16_t)(_XTAL_FREQ / desiredBaudRate / 4 - 1);

        U1CON0bits.MODE  = 0b0000;  // asynchronous mode
        U1CON0bits.TXEN = 1;  // enable transmit
        U1CON0bits.RXEN = 1;  // enable receive

        U1CON1bits.ON = 1;  // enable USART
    #endif
}
