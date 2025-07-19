/* Josip Medved <jmedved@jmedved.com> * www.medo64.com * MIT License */

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "app.h"
#include "i2c_master.h"

#if defined(_I2C_MASTER_CUSTOM_INIT)
    void i2c_master_init(uint8_t rate) {
        #if defined(_I2C_MASTER_MODULE2)  // I2C2 (SCL RB1) (SDA RB2)

            #define I2C_MASTER_I2CCON0_REGISTER         I2C2CON0bits
            #define I2C_MASTER_I2CCON1_REGISTER         I2C2CON1bits
            #define I2C_MASTER_I2CCON2_REGISTER         I2C2CON2bits
            #define I2C_MASTER_I2CCLK_REGISTER          I2C2CLKbits

            #define I2C_MASTER_I2CSTAT0_REGISTER        I2C2STAT0bits
            #define I2C_MASTER_I2CSTAT1_REGISTER        I2C2STAT1bits
            #define I2C_MASTER_I2CADB1_REGISTER         I2C2ADB1
            #define I2C_MASTER_I2CCNT_REGISTER          I2C2CNT
            #define I2C_MASTER_I2CRXB_REGISTER          I2C2RXB
            #define I2C_MASTER_I2CTXB_REGISTER          I2C2TXB

        #else  // I2C1 (SCL RC3) (SDA RC4)

            #define I2C_MASTER_I2CCON0_REGISTER         I2C1CON0bits
            #define I2C_MASTER_I2CCON1_REGISTER         I2C1CON1bits
            #define I2C_MASTER_I2CCON2_REGISTER         I2C1CON2bits
            #define I2C_MASTER_I2CCLK_REGISTER          I2C1CLKbits

            #define I2C_MASTER_I2CSTAT0_REGISTER        I2C1STAT0bits
            #define I2C_MASTER_I2CSTAT1_REGISTER        I2C1STAT1bits
            #define I2C_MASTER_I2CADB1_REGISTER         I2C1ADB1
            #define I2C_MASTER_I2CCNT_REGISTER          I2C1CNT
            #define I2C_MASTER_I2CRXB_REGISTER          I2C1RXB
            #define I2C_MASTER_I2CTXB_REGISTER          I2C1TXB

        #endif

        // Setup I2C
        I2C_MASTER_I2CCON1_REGISTER.ACKCNT = 1;    // Acknowledge value transmitted after received data, when I2CxCNT = 0 - NACK

        T6CLK = 0b0010;                            // Fosc
        if (rate < 10) {
            T6PR = (uint8_t)((uint32_t)_XTAL_FREQ / 4 / 100000 - 1);
        } else if (rate > 100) {
            T6PR = (uint8_t)((uint32_t)_XTAL_FREQ / 4 / 1000000 - 1);
        } else {
            T6PR = (uint8_t)((uint32_t)_XTAL_FREQ / 4 / rate / 10000 - 1);  // Fosc / 5(FME=0) / KHz - 1
        }
        T6HLT = 0x00;                              // pre not sync, rising edge, clk not sync, SW ctrl
        T6CONbits.ON = 1;                          // Timer6 is On
        I2C_MASTER_I2CCLK_REGISTER.CLK = 0b1000;   // Timer6 post scaled output
        I2C_MASTER_I2CCON2_REGISTER.FME = 1;       // SCL is sampled high once before driving SCL low. (FSCL = FI2CXCLK/4)

        I2C_MASTER_I2CCON2_REGISTER.ABD = 0;       // Received address data is loaded only into the I2CxADB; Transmitted address data is loaded from the I2CxADB0/1 registers.
        I2C_MASTER_I2CCON2_REGISTER.BFRET = 0b00;  // 8 I2C Clock pulses (bus free)

        I2C_MASTER_I2CCON0_REGISTER.MODE = 0b100;  // I2C Master mode, 7-bit address
        I2C_MASTER_I2CCON0_REGISTER.EN = 1;        // Enables the I2C module

        #if defined(_I2C_MASTER_MODULE2)  // I2C2
            I2C2PIR = 0x0;
            I2C2PIE = 0x0;
            I2C2ERR = 0x0;
            I2C2CNT = 0x0;
            I2C2PIEbits.SCIE = 0;  // errata
            I2C2PIEbits.PCIE = 0;
        #else  // I2C1
            I2C1PIR = 0x0;
            I2C1PIE = 0x0;
            I2C1ERR = 0x0;
            I2C1CNT = 0x0;
            I2C1PIEbits.SCIE = 0;  // errata
            I2C1PIEbits.PCIE = 0;
        #endif
    }
#else
    void i2c_master_init(void) {
        #if defined(_I2C_MASTER_MODULE2)  // I2C2 (SCL RB1) (SDA RB2)

            #define I2C_MASTER_I2CCON0_REGISTER         I2C2CON0bits
            #define I2C_MASTER_I2CCON1_REGISTER         I2C2CON1bits
            #define I2C_MASTER_I2CCON2_REGISTER         I2C2CON2bits
            #define I2C_MASTER_I2CCLK_REGISTER          I2C2CLKbits

            #define I2C_MASTER_I2CSTAT0_REGISTER        I2C2STAT0bits
            #define I2C_MASTER_I2CSTAT1_REGISTER        I2C2STAT1bits
            #define I2C_MASTER_I2CADB1_REGISTER         I2C2ADB1
            #define I2C_MASTER_I2CCNT_REGISTER          I2C2CNT
            #define I2C_MASTER_I2CRXB_REGISTER          I2C2RXB
            #define I2C_MASTER_I2CTXB_REGISTER          I2C2TXB

        #else  // I2C1 (SCL RC3) (SDA RC4)

            #define I2C_MASTER_I2CCON0_REGISTER         I2C1CON0bits
            #define I2C_MASTER_I2CCON1_REGISTER         I2C1CON1bits
            #define I2C_MASTER_I2CCON2_REGISTER         I2C1CON2bits
            #define I2C_MASTER_I2CCLK_REGISTER          I2C1CLKbits

            #define I2C_MASTER_I2CSTAT0_REGISTER        I2C1STAT0bits
            #define I2C_MASTER_I2CSTAT1_REGISTER        I2C1STAT1bits
            #define I2C_MASTER_I2CADB1_REGISTER         I2C1ADB1
            #define I2C_MASTER_I2CCNT_REGISTER          I2C1CNT
            #define I2C_MASTER_I2CRXB_REGISTER          I2C1RXB
            #define I2C_MASTER_I2CTXB_REGISTER          I2C1TXB

        #endif

        // Setup I2C
        I2C_MASTER_I2CCON1_REGISTER.ACKCNT = 1;       // Acknowledge value transmitted after received data, when I2CxCNT = 0 - NACK
        #if defined(_I2C_MASTER_RATE_KHZ)
            T6CLK = 0b0010;                           // Fosc
            T6PR = _XTAL_FREQ / 4 / _I2C_MASTER_RATE_KHZ / 1000 - 1;  // Fosc / 5(FME=0) / KHz - 1
            T6HLT = 0x00;                             // pre not sync, rising edge, clk not sync, SW ctrl
            T6CONbits.ON = 1;                         // Timer6 is On
            I2C_MASTER_I2CCLK_REGISTER.CLK = 0b1000;  // Timer6 post scaled output
            I2C_MASTER_I2CCON2_REGISTER.FME = 1;      // SCL is sampled high once before driving SCL low. (FSCL = FI2CXCLK/4)
        #else
            I2C_MASTER_I2CCLK_REGISTER.CLK = 0b0011;  // MFINTOSC (500 kHz)
            I2C_MASTER_I2CCON2_REGISTER.FME = 0;      // SCL is sampled high twice before driving SCL low. (FSCL = FI2CXCLK/5)
        #endif
        I2C_MASTER_I2CCON2_REGISTER.ABD = 0;          // Received address data is loaded only into the I2CxADB; Transmitted address data is loaded from the I2CxADB0/1 registers.
        I2C_MASTER_I2CCON2_REGISTER.BFRET = 0b00;     // 8 I2C Clock pulses (bus free)

        I2C_MASTER_I2CCON0_REGISTER.MODE = 0b100;     // I2C Master mode, 7-bit address
        I2C_MASTER_I2CCON0_REGISTER.EN = 1;           // Enables the I2C module

        #if defined(_I2C_MASTER_MODULE2)  // I2C2
            I2C2PIR = 0x0;
            I2C2PIE = 0x0;
            I2C2ERR = 0x0;
            I2C2CNT = 0x0;
            I2C2PIEbits.SCIE = 0;  // errata
            I2C2PIEbits.PCIE = 0;
        #else  // I2C1
            I2C1PIR = 0x0;
            I2C1PIE = 0x0;
            I2C1ERR = 0x0;
            I2C1CNT = 0x0;
            I2C1PIEbits.SCIE = 0;  // errata
            I2C1PIEbits.PCIE = 0;
        #endif
    }
#endif

bool i2c_master_readRegisterBytes(const uint8_t deviceAddress, const uint8_t registerAddress, uint8_t* readData, const uint8_t readCount) {
    I2C_MASTER_I2CSTAT1_REGISTER.TXWE = 0;       // clear buffer error
    I2C_MASTER_I2CSTAT1_REGISTER.CLRBF = 1;      // clear buffer
    while (I2C_MASTER_I2CSTAT1_REGISTER.CLRBF);  // wait for buffer clear

    I2C_MASTER_I2CADB1_REGISTER = (uint8_t)(deviceAddress << 1);  // load address
    I2C_MASTER_I2CTXB_REGISTER = registerAddress;                 // send one byte
    I2C_MASTER_I2CCNT_REGISTER = 1;                               // let's ignore possible overflow

    I2C_MASTER_I2CCON0_REGISTER.RSEN = 1;   // restart enabled
    I2C_MASTER_I2CCON0_REGISTER.S = 1;      // start
    while (I2C_MASTER_I2CCON0_REGISTER.S);

    while (I2C_MASTER_I2CSTAT0_REGISTER.MMA) {
        if (I2C_MASTER_I2CCON0_REGISTER.MDR) {
            if (I2C_MASTER_I2CSTAT1_REGISTER.RXBF) {     // have data to read
                *readData = I2C_MASTER_I2CRXB_REGISTER;
                readData++;
            } else {
                I2C_MASTER_I2CADB1_REGISTER |= 0b1;      // read address
                I2C_MASTER_I2CCNT_REGISTER = readCount;
                I2C_MASTER_I2CCON0_REGISTER.S = 1;       // restart
                while (I2C_MASTER_I2CCON0_REGISTER.S);
                I2C_MASTER_I2CCON0_REGISTER.RSEN = 0;    // clear restart flag
            }
        }
    }

    if (I2C_MASTER_I2CSTAT1_REGISTER.RXBF) {  // read last byte
        *readData = I2C_MASTER_I2CRXB_REGISTER;
    }

    return (I2C_MASTER_I2CCNT_REGISTER == 0);
}


bool i2c_master_writeRegisterBytes(const uint8_t deviceAddress, const uint8_t registerAddress, const uint8_t* data, const uint8_t count) {
    I2C_MASTER_I2CSTAT1_REGISTER.TXWE = 0;       // clear buffer error
    I2C_MASTER_I2CSTAT1_REGISTER.CLRBF = 1;      // clear buffer
    while (I2C_MASTER_I2CSTAT1_REGISTER.CLRBF);  // wait for buffer clear

    I2C_MASTER_I2CADB1_REGISTER = (uint8_t)(deviceAddress << 1);  // load address
    I2C_MASTER_I2CTXB_REGISTER = registerAddress;                 // send first byte
    I2C_MASTER_I2CCNT_REGISTER = count + 1;                       // let's ignore possible overflow

    I2C_MASTER_I2CCON0_REGISTER.S = 1;      // start
    while (I2C_MASTER_I2CCON0_REGISTER.S);

    while (I2C_MASTER_I2CSTAT0_REGISTER.MMA) {
        if (I2C_MASTER_I2CCON0_REGISTER.MDR) {
            if (I2C_MASTER_I2CSTAT1_REGISTER.TXBE) {
                if (I2C_MASTER_I2CCNT_REGISTER > 0) {
                    I2C_MASTER_I2CTXB_REGISTER = *data;
                    data++;
                } else {  // not sure how we got here
                    I2C_MASTER_I2CTXB_REGISTER = 0x00;
                }
            }
        }
    }

    return (I2C_MASTER_I2CCNT_REGISTER == 0);
}

bool i2c_master_writeRegisterZeroBytes(const uint8_t deviceAddress, const uint8_t registerAddress, const uint8_t zeroCount) {
    I2C_MASTER_I2CSTAT1_REGISTER.TXWE = 0;       // clear buffer error
    I2C_MASTER_I2CSTAT1_REGISTER.CLRBF = 1;      // clear buffer
    while (I2C_MASTER_I2CSTAT1_REGISTER.CLRBF);  // wait for buffer clear

    I2C_MASTER_I2CADB1_REGISTER = (uint8_t)(deviceAddress << 1);  // load address
    I2C_MASTER_I2CTXB_REGISTER = registerAddress;                 // send first byte
    I2C_MASTER_I2CCNT_REGISTER = zeroCount + 1;                   // let's ignore possible overflow

    I2C_MASTER_I2CCON0_REGISTER.S = 1;      // start
    while (I2C_MASTER_I2CCON0_REGISTER.S);

    while (I2C_MASTER_I2CSTAT0_REGISTER.MMA) {
        if (I2C_MASTER_I2CCON0_REGISTER.MDR) {
            if (I2C_MASTER_I2CSTAT1_REGISTER.TXBE) {
                I2C_MASTER_I2CTXB_REGISTER = 0x00;
            }
        }
    }

    return (I2C_MASTER_I2CCNT_REGISTER == 0);
}


bool i2c_master_writeBytes(const uint8_t deviceAddress, const uint8_t* data, const uint8_t count) {
    return i2c_master_writeRegisterBytes(deviceAddress, *data, data + 1, count - 1);
}

bool i2c_master_writeZeroBytes(const uint8_t deviceAddress, const uint8_t zeroCount) {
    return i2c_master_writeRegisterZeroBytes(deviceAddress, 0, zeroCount - 1);
}
