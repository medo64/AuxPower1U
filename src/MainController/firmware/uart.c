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


bool uart_canRead() {
    #if defined(_UART_MODULE2)
        return !U2FIFObits.RXBE;
    #else
        return !U1FIFObits.RXBE;
    #endif
}

bool uart_canWrite() {
    #if defined(_UART_MODULE2)
        return !U2FIFObits.TXBF;
    #else
        return !U1FIFObits.TXBF;
    #endif
}


void uart_checkAndResetReadErrors() {
    #if defined(_UART_MODULE2)
        if (U2ERRIRbits.FERIF || U2ERRIRbits.RXFOIF) { //framing or overrun error
            U2CON0bits.RXEN = 0;  // disable receive, also clears errors
            U2CON0bits.RXEN = 1;  // enable receive
            uint8_t dummyRead;
            while (!U2FIFObits.RXBE) { dummyRead = U2RXB; }  // read data to clear FIFO
            U2CON1bits.ON = 0;  // disable USART.
            U2CON1bits.ON = 1;  // enable USART.
        }
    #else
        if (U1ERRIRbits.FERIF || U1ERRIRbits.RXFOIF) { //framing or overrun error
            U1CON0bits.RXEN = 0;  // disable receive, also clears errors
            U1CON0bits.RXEN = 1;  // enable receive
            uint8_t dummyRead;
            while (!U1FIFObits.RXBE) { dummyRead = U1RXB; }  // read data to clear FIFO
            U1CON1bits.ON = 0;  // disable USART.
            U1CON1bits.ON = 1;  // enable USART.
        }
    #endif
}

uint8_t uart_readByte() {
    uart_checkAndResetReadErrors();

    #if defined(_UART_MODULE2)
        while (U2FIFObits.RXBE);
        return U2RXB;
    #else
        while (U1FIFObits.RXBE);
        return U1RXB;
    #endif
}

void uart_writeByte(const uint8_t data) {
    #if defined(_UART_MODULE2)
        while (U2FIFObits.TXBF);  // wait while buffer is full
        U2TXB = data;
    #else
        while (U1FIFObits.TXBF);  // wait while buffer is full
        U1TXB = data;
    #endif
}


bool uart_tryReadByte(uint8_t* data) {
    uart_checkAndResetReadErrors();

    #if defined(_UART_MODULE2)
        if (U2FIFObits.RXBE) {
            return false;
        } else {
            *data = U2RXB;
            return true;
        }
    #else
        if (U1FIFObits.RXBE) {
            return false;
        } else {
            *data = U1RXB;
            return true;
        }
    #endif
}

bool uart_tryWriteByte(const uint8_t data) {
    #if defined(_UART_MODULE2)
        if (U2FIFObits.TXBF) {
            return false;
        } else {
            U2TXB = data;
            return true;
        }
    #else
        if (U1FIFObits.TXBF) {
            return false;
        } else {
            U1TXB = data;
            return true;
        }
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


void uart_writeUInt8(const uint8_t value) {
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

void uart_writeUInt16(const uint16_t value) {
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

void uart_writeHexDigit(const uint8_t value) {
    uint8_t data = 0x30 + (value & 0x0F);
    if (data > 0x39) { data += 7; }
    uart_writeByte(data);
}

void uart_writeHexUInt8(const uint8_t value) {
    uart_writeHexDigit(value >> 4);
    uart_writeHexDigit(value);
}


#if !defined(_XTAL_FREQ)
    #error "Must define _XTAL_FREQ";
#endif

#if !defined(_UART_RX_PPS_INPUT)
    #if defined(_UART_MODULE2)
        #define _UART_RX_PPS_INPUT  0b10111  // RC7
    #else
        #define _UART_RX_PPS_INPUT  0b01111  // RB7
    #endif
#endif

#if !defined(_UART_TX_PPS_OUTPUT)
    #if defined(_UART_MODULE2)
        #define _UART_TX_PPS_OUTPUT  RC6PPS
    #else
        #define _UART_TX_PPS_OUTPUT  RB6PPS
    #endif
#endif

void uart_setup(uint32_t desiredBaudRate) {
    #if defined(_UART_MODULE2)
        U2CON0bits.BRGS = 1;  // Baud rate generator is high speed
        U2BRG = (uint16_t)((_XTAL_FREQ * 10 / desiredBaudRate / 4 - 5) / 10);

        U2CON0bits.MODE  = 0b0000;  // asynchronous mode
        U2CON0bits.TXEN = 1;  // enable transmit
        U2CON0bits.RXEN = 1;  // enable receive

        U2CON1bits.ON = 1;  // enable USART
    #else
        U1CON0bits.BRGS = 1;  // Baud rate generator is high speed
        U1BRG = (uint16_t)((_XTAL_FREQ * 10 / desiredBaudRate / 4 - 5) / 10);

        U1CON0bits.MODE  = 0b0000;  // asynchronous mode
        U1CON0bits.TXEN = 1;  // enable transmit
        U1CON0bits.RXEN = 1;  // enable receive

        U1CON1bits.ON = 1;  // enable USART
    #endif
}
