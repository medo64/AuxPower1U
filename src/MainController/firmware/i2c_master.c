/* Josip Medved <jmedved@jmedved.com> * www.medo64.com * MIT License */

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "app.h"
#include "i2c_master.h"

#define I2C_READ  1
#define I2C_WRITE 0
#define I2C_ACK   0
#define I2C_NACK  1

void i2c_master_init(void) {
    #if defined(_16F1454) || defined(_16F1455)  // RC0 RC1

        SSPCON1 = 0;
        SSPCON2 = 0;
        SSPSTAT = 0; // reset all

        SSPCON1bits.SSPM = 0b1000; // I2C master mode
        SSPCON1bits.SSPEN = 1; // enable I2C master mode
        SSP1STATbits.CKE = 1; // slew control enabled, low voltage input (SMBus) enables
        SSP1ADD = baudRateCounter; // setup speed

        TRISC0 = 1; // clock pin configured as input
        TRISC1 = 1; // data pin configured as input}

    #elif defined(_18F25K83) || defined(_18F26K83)

        // unlock PPS
        PPSLOCK = 0x55;
        PPSLOCK = 0xAA;
        PPSLOCKbits.PPSLOCKED = 0;

        #if defined(_I2C_MASTER_MODULE2)  // I2C2

            RB1PPS = 0b00100011;      // I2C/2 SCL RB1
            I2C2SCLPPS = 0b00001001;
            RB2PPS = 0b00100100;      // I2C/2 SDA RB2
            I2C2SDAPPS = 0b00001010;

            LATB    &= 0x11111001;    // clear write latches
            TRISB   &= 0b11111001;    // make pins output
            ANSELB  &= 0b11111001;    // make pins digital
            SLRCONB &= 0b11111001;    // no GPIO slew rate limiting
            ODCONB  |= 0b00000110;    // make pins open-drain

            RB1I2Cbits.SLEW = 1;   // I2C specific slew rate limiting is enabled
            RB2I2Cbits.SLEW = 1;   // I2C specific slew rate limiting is enabled
            RB1I2Cbits.PU = 0b00;  // external pull-ups used
            RB2I2Cbits.PU = 0b00;  // external pull-ups used
            RB1I2Cbits.TH = 0b01;  // SMBus 3.0 (1.35 V) input threshold
            RB2I2Cbits.TH = 0b01;  // SMBus 3.0 (1.35 V) input threshold

        #else  // I2C1

            RC3PPS = 0b00100001;      // I2C/1 SCL RC3
            I2C1SCLPPS = 0b00010011;
            RC4PPS = 0b00100010;      // I2C/1 SDA RC4
            I2C1SDAPPS = 0b00010100;

            LATC    &= 0x11100111;    // clear write latches 
            TRISC   &= 0b11100111;    // make pins output
            ANSELC  &= 0b11100111;    // make pins digital
            SLRCONC &= 0b11100111;    // no GPIO slew rate limiting
            ODCONC  |= 0b00011000;    // make pins open-drain

            RC3I2Cbits.SLEW = 1;  // I2C specific slew rate limiting is enabled
            RC4I2Cbits.SLEW = 1;  // I2C specific slew rate limiting is enabled
            RC3I2Cbits.PU = 0b00;  // external pull-ups used
            RC4I2Cbits.PU = 0b00;  // external pull-ups used
            RC3I2Cbits.TH = 0b01;  // SMBus 3.0 (1.35 V) input threshold
            RC4I2Cbits.TH = 0b01;  // SMBus 3.0 (1.35 V) input threshold
        #endif

        // Lock PPS
        PPSLOCK = 0x55;
        PPSLOCK = 0xAA;
        PPSLOCKbits.PPSLOCKED = 1;

        // Setup I2C
        I2C2CON1bits.ACKCNT = 1;    // Acknowledge value transmitted after received data, when I2CxCNT = 0 - NACK
        I2C2CON2bits.FME = 0;       // 100 kHz - SCL is sampled high twice before driving SCL low. (FSCL = FI2CXCLK/5)
        I2C2CON2bits.ABD = 0;       // Received address data is loaded only into the I2CxADB; Transmitted address data is loaded from the I2CxADB0/1 registers.
        I2C2CON2bits.BFRET = 0b00;  // 8 I2C Clock pulses (bus free)

        I2C2CLKbits.CLK = 0b0011;   // MFINTOSC (500 kHz)

        I2C2CON0bits.MODE = 0b100;  // I2C Master mode, 7-bit address
        I2C2CON0bits.EN = 1;        // Enables the I2C module

        I2C2PIR = 0;
        I2C2ERR = 0;

        I2C2PIR = 0x0;
        I2C2PIE = 0x0;
        I2C2ERR = 0x0;
        I2C2CNT = 0x0;
        I2C2PIEbits.SCIE = 0;  // errata
        I2C2PIEbits.PCIE = 0;

    #endif
}


bool i2c_master_writeBytes(const uint8_t address, const uint8_t* data, const uint8_t count) {
    #if defined(_16F1454) || defined(_16F1455)


    #elif defined(_18F25K83) || defined(_18F26K83)

        I2C2STAT1bits.TXWE = 0;  // clear buffer error
        I2C2STAT1bits.CLRBF = 1;  // clear buffer
        while (I2C2STAT1bits.CLRBF);  // wait for buffer clear

        I2C2ADB1 = (uint8_t)(address << 1);  // load address
        I2C2TXB = *data;
        I2C2CNT = count;

        I2C2CON0bits.S = 1; //Start
        while (I2C2CON0bits.S);

        uint8_t i = count - 1;
        while (I2C2STAT0bits.MMA) {
            if (I2C2CON0bits.MDR) {
                if (I2C2STAT1bits.TXBE) {
                    if (i > 0) {
                        i--;
                        data++;
                        I2C2TXB = *data;
                    } else {  // not sure how we got here
                        I2C2TXB = 0x00;
                    }
                }
            }
        }

        return (I2C2CNT == 0);

    #endif
}

bool i2c_master_writeBytesWithPrefix(const uint8_t address, const uint8_t firstByteOfData, const uint8_t* restOfData, const uint8_t count) {
    #if defined(_16F1454) || defined(_16F1455)


    #elif defined(_18F25K83) || defined(_18F26K83)

        I2C2STAT1bits.TXWE = 0;  // clear buffer error
        I2C2STAT1bits.CLRBF = 1;  // clear buffer
        while (I2C2STAT1bits.CLRBF);  // wait for buffer clear

        I2C2ADB1 = (uint8_t)(address << 1);  // load address
        I2C2TXB = firstByteOfData;
        I2C2CNT = count + 1;  // let's ignore possible overflow

        I2C2CON0bits.S = 1; //Start
        while (I2C2CON0bits.S);

        uint8_t i = count;
        while (I2C2STAT0bits.MMA) {
            if (I2C2CON0bits.MDR) {
                if (I2C2STAT1bits.TXBE) {
                    if (i > 0) {
                        i--;
                        I2C2TXB = *restOfData;
                        restOfData++;
                    } else {  // not sure how we got here
                        I2C2TXB = 0x00;
                    }
                }
            }
        }

        return (I2C2CNT == 0);

    #endif
}


bool i2c_master_writeZeroBytes(const uint8_t address, const uint8_t zeroCount) {
    #if defined(_16F1454) || defined(_16F1455)


    #elif defined(_18F25K83) || defined(_18F26K83)

        I2C2STAT1bits.TXWE = 0;  // clear buffer error
        I2C2STAT1bits.CLRBF = 1;  // clear buffer
        while (I2C2STAT1bits.CLRBF);  // wait for buffer clear

        I2C2ADB1 = (uint8_t)(address << 1);  // load address
        I2C2TXB = 0x00;
        I2C2CNT = zeroCount;

        I2C2CON0bits.S = 1; //Start
        while (I2C2CON0bits.S);

        uint8_t i = zeroCount - 1;
        while (I2C2STAT0bits.MMA) {
            if (I2C2CON0bits.MDR) {
                if (I2C2STAT1bits.TXBE) {
                    if (i > 0) { i--; }
                    I2C2TXB = 0x00;
                }
            }
        }

        return (I2C2CNT == 0);

    #endif
}

bool i2c_master_writeZeroBytesWithPrefix(const uint8_t address, const uint8_t firstByteOfData, const uint8_t zeroCount) {
    #if defined(_16F1454) || defined(_16F1455)


    #elif defined(_18F25K83) || defined(_18F26K83)

        I2C2STAT1bits.TXWE = 0;  // clear buffer error
        I2C2STAT1bits.CLRBF = 1;  // clear buffer
        while (I2C2STAT1bits.CLRBF);  // wait for buffer clear

        I2C2ADB1 = (uint8_t)(address << 1);  // load address
        I2C2TXB = firstByteOfData;
        I2C2CNT = zeroCount + 1;  // let's ignore possible overflow

        I2C2CON0bits.S = 1; //Start
        while (I2C2CON0bits.S);

        uint8_t i = zeroCount;
        while (I2C2STAT0bits.MMA) {
            if (I2C2CON0bits.MDR) {
                if (I2C2STAT1bits.TXBE) {
                    if (i > 0) { i--; }
                    I2C2TXB = 0x00;
                }
            }
        }

        return (I2C2CNT == 0);

    #endif
}
