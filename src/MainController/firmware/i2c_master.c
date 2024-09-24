/* Josip Medved <jmedved@jmedved.com> * www.medo64.com * MIT License */

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "app.h"
#include "i2c_master.h"

#if defined(_16F1454) || defined(_16F1455)  // RC0 RC1

    void i2c_master_16f_start(void) {
        SSPCON2bits.SEN = 1;      // initiate Start condition
        while (SSPCON2bits.SEN);  // wait until done
    }

    void i2c_master_16f_restart(void) {
        SSPCON2bits.RSEN = 1;      // initiate Repeated Start condition
        while (SSPCON2bits.RSEN);  // wait until done
    }

    void i2c_master_16f_stop(void) {
        SSPCON2bits.PEN = 1;      // initiate Stop condition
        while (SSPCON2bits.PEN);  // wait until done
    }

    void i2c_master_16f_waitIdle(void) {
        while ((SSPCON2 & 0x1F) | SSPSTATbits.R_nW);  // wait until idle
    }

    void i2c_master_16f_resetBus(void) {
        //reset I2C bus
        LATC0 = 0;                                // set clock low
        LATC1 = 0;                                // set data low
        TRISC0 = 1;                               // clock pin configured as output
        TRISC1 = 1;                               // data pin configured as input
        for (unsigned char j=0; j<3; j++) {       // repeat 3 times with alternating data
            for (unsigned char i=0; i<10; i++) {  // 9 cycles + 1 extra
                TRISC0 = 0; __delay_us(50);       // force clock low
                TRISC0 = 1; __delay_us(50);       // release clock high
            }
            TRISC1 = !TRISC1;                     // toggle data line
        }
    }

    bool i2c_master_16f_writeByte(const uint8_t value) {
        i2c_master_16f_waitIdle();
        SSPBUF = value;                             // set data
        if (SSPCON1bits.WCOL) { return false; }     // fail if there is a collision
        while (SSPSTATbits.BF);                     // wait until write is done
        return SSPCON2bits.ACKSTAT ? false : true;  // return if successful
    }

    bool i2c_master_16f_startWrite(const uint8_t address) {
        i2c_master_16f_waitIdle();
        i2c_master_16f_start();                                    // start operation
        return i2c_master_16f_writeByte((uint8_t)(address << 1));  // load address
    }

    bool i2c_master_16f_readByte(uint8_t* value) {
        if (PIR2bits.BCL1IF) { return false; }

        SSPCON2bits.RCEN = 1;                       // start receive
        while ( !SSPSTATbits.BF );                  // wait for byte

        *value = SSPBUF;                             // read byte
        if (SSPCON2bits.ACKSTAT) { return false; }  // end prematurely if there's an error

        SSPCON2bits.ACKDT = 0;                      // ACK byte
        SSPCON2bits.ACKEN = 1;                      // initiate acknowledge sequence
        while (SSPCON2bits.ACKEN);                  // wait for done

        return true;                                 // return success
    }

    void i2c_master_init(uint8_t baudRateCounter) {
        SSPCON1 = 0;  SSPCON2 = 0;  SSPSTAT = 0;  // reset all

        i2c_master_16f_resetBus();
        SSPCON1bits.SSPM = 0b1000;                // I2C master mode
        SSPCON1bits.SSPEN = 1;                    // enable I2C master mode
        SSP1STATbits.CKE = 1;                     // slew control enabled, low voltage input (SMBus) enables
        SSP1ADD = baudRateCounter;                // setup speed

        TRISC0 = 1;                               // clock pin configured as input
        TRISC1 = 1;                               // data pin configured as input}
    }

#elif defined(_18F25K83) || defined(_18F26K83)

    void i2c_master_init(void) {
        // unlock PPS
        PPSLOCK = 0x55;
        PPSLOCK = 0xAA;
        PPSLOCKbits.PPSLOCKED = 0;

        #if defined(_I2C_MASTER_MODULE2)  // I2C2 (SCL RB1) (SDA RB2)

            #define I2C_MASTER_SCL_PPS_PORT_REGISTER    RB1PPS
            #define I2C_MASTER_SCL_PPS_PORT_VALUE       0b00100011
            #define I2C_MASTER_SCL_PPS_SIGNAL_REGISTER  I2C2SCLPPS
            #define I2C_MASTER_SCL_PPS_SIGNAL_VALUE     0b00001001
            #define I2C_MASTER_SDA_PPS_PORT_REGISTER    RB2PPS
            #define I2C_MASTER_SDA_PPS_PORT_VALUE       0b00100100
            #define I2C_MASTER_SDA_PPS_SIGNAL_REGISTER  I2C2SDAPPS
            #define I2C_MASTER_SDA_PPS_SIGNAL_VALUE     0b00001010

            #define I2C_MASTER_LAT_REGISTER             LATB
            #define I2C_MASTER_TRIS_REGISTER            TRISB
            #define I2C_MASTER_ANSEL_REGISTER           ANSELB
            #define I2C_MASTER_SLRCON_REGISTER          SLRCONB
            #define I2C_MASTER_ODCON_REGISTER           ODCONB
            #define I2C_MASTER_LAT_MASK                 0x11111001

            #define I2C_MASTER_SCL_I2CPORT_REGISTER     RB1I2Cbits
            #define I2C_MASTER_SDA_I2CPORT_REGISTER     RB2I2Cbits
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

            #define I2C_MASTER_SCL_PPS_PORT_REGISTER    RC3PPS
            #define I2C_MASTER_SCL_PPS_PORT_VALUE       0b00100001
            #define I2C_MASTER_SCL_PPS_SIGNAL_REGISTER  I2C1SCLPPS
            #define I2C_MASTER_SCL_PPS_SIGNAL_VALUE     0b00010011
            #define I2C_MASTER_SDA_PPS_PORT_REGISTER    RC4PPS
            #define I2C_MASTER_SDA_PPS_PORT_VALUE       0b00100010
            #define I2C_MASTER_SDA_PPS_SIGNAL_REGISTER  I2C1SDAPPS
            #define I2C_MASTER_SDA_PPS_SIGNAL_VALUE     0b00010100

            #define I2C_MASTER_LAT_REGISTER             LATC
            #define I2C_MASTER_TRIS_REGISTER            TRISC
            #define I2C_MASTER_ANSEL_REGISTER           ANSELC
            #define I2C_MASTER_SLRCON_REGISTER          SLRCONC
            #define I2C_MASTER_ODCON_REGISTER           ODCONC
            #define I2C_MASTER_LAT_MASK                 0x11100111

            #define I2C_MASTER_SCL_I2CPORT_REGISTER     RC3I2Cbits
            #define I2C_MASTER_SDA_I2CPORT_REGISTER     RC4I2Cbits
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

        I2C_MASTER_SCL_PPS_PORT_REGISTER   = I2C_MASTER_SCL_PPS_PORT_VALUE;
        I2C_MASTER_SCL_PPS_SIGNAL_REGISTER = I2C_MASTER_SCL_PPS_SIGNAL_VALUE;
        I2C_MASTER_SDA_PPS_PORT_REGISTER   = I2C_MASTER_SDA_PPS_PORT_VALUE;
        I2C_MASTER_SDA_PPS_SIGNAL_REGISTER = I2C_MASTER_SDA_PPS_SIGNAL_VALUE;

        I2C_MASTER_LAT_REGISTER    &= I2C_MASTER_LAT_REGISTER;    // clear write latches
        I2C_MASTER_TRIS_REGISTER   &= I2C_MASTER_LAT_REGISTER;    // make pins output
        I2C_MASTER_ANSEL_REGISTER  &= I2C_MASTER_LAT_REGISTER;    // make pins digital
        I2C_MASTER_SLRCON_REGISTER &= I2C_MASTER_LAT_REGISTER;    // no GPIO slew rate limiting
        I2C_MASTER_ODCON_REGISTER  |= ~I2C_MASTER_LAT_REGISTER;   // make pins open-drain

        I2C_MASTER_SCL_I2CPORT_REGISTER.SLEW = 1;   // I2C specific slew rate limiting is enabled
        I2C_MASTER_SDA_I2CPORT_REGISTER.SLEW = 1;   // I2C specific slew rate limiting is enabled
        I2C_MASTER_SCL_I2CPORT_REGISTER.PU = 0b00;  // external pull-ups used
        I2C_MASTER_SDA_I2CPORT_REGISTER.PU = 0b00;  // external pull-ups used
        I2C_MASTER_SCL_I2CPORT_REGISTER.TH = 0b01;  // SMBus 3.0 (1.35 V) input threshold
        I2C_MASTER_SDA_I2CPORT_REGISTER.TH = 0b01;  // SMBus 3.0 (1.35 V) input threshold

        // Lock PPS
        PPSLOCK = 0x55;
        PPSLOCK = 0xAA;
        PPSLOCKbits.PPSLOCKED = 1;

        // Setup I2C
        I2C_MASTER_I2CCON1_REGISTER.ACKCNT = 1;    // Acknowledge value transmitted after received data, when I2CxCNT = 0 - NACK
        I2C_MASTER_I2CCON2_REGISTER.FME = 0;       // 100 kHz - SCL is sampled high twice before driving SCL low. (FSCL = FI2CXCLK/5)
        I2C_MASTER_I2CCON2_REGISTER.ABD = 0;       // Received address data is loaded only into the I2CxADB; Transmitted address data is loaded from the I2CxADB0/1 registers.
        I2C_MASTER_I2CCON2_REGISTER.BFRET = 0b00;  // 8 I2C Clock pulses (bus free)

        I2C_MASTER_I2CCLK_REGISTER.CLK = 0b0011;   // MFINTOSC (500 kHz)

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
#endif


bool i2c_master_readRegisterBytes(const uint8_t deviceAddress, const uint8_t registerAddress, uint8_t* readData, const uint8_t readCount) {
    #if defined(_16F1454) || defined(_16F1455)

        if (!i2c_master_16f_startWrite(deviceAddress)) { return false; }
        if (!i2c_master_16f_writeByte(registerAddress)) { return false; }

        i2c_master_16f_restart();
        for (uint8_t i = 0; i < readCount; i++) {
            if (!i2c_master_16f_readByte(readData)) { return false; }
            readData++;
        }

        i2c_master_16f_stop();
        return true;

    #elif defined(_18F25K83) || defined(_18F26K83)

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

    #endif
}


bool i2c_master_writeRegisterBytes(const uint8_t deviceAddress, const uint8_t registerAddress, const uint8_t* data, const uint8_t count) {
    #if defined(_16F1454) || defined(_16F1455)

        if (!i2c_master_16f_startWrite(deviceAddress)) { return false; }

        if (!i2c_master_16f_writeByte(registerAddress)) { return false; }
        for (uint8_t i = 0; i < count; i++) {
            if (!i2c_master_16f_writeByte(*data)) { return false; }
            data++;
        }

        i2c_master_16f_stop();
        return true;

    #elif defined(_18F25K83) || defined(_18F26K83)

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

    #endif
}

bool i2c_master_writeRegisterZeroBytes(const uint8_t deviceAddress, const uint8_t registerAddress, const uint8_t zeroCount) {
    #if defined(_16F1454) || defined(_16F1455)

        if (!i2c_master_16f_startWrite(deviceAddress)) { return false; }

        if (!i2c_master_16f_writeByte(registerAddress)) { return false; }
        for (uint8_t i = 0; i < zeroCount; i++) {
            if (!i2c_master_16f_writeByte(0)) { return false; }
        }

        i2c_master_16f_stop();
        return true;

    #elif defined(_18F25K83) || defined(_18F26K83)

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

    #endif
}


bool i2c_master_writeBytes(const uint8_t deviceAddress, const uint8_t* data, const uint8_t count) {
    return i2c_master_writeRegisterBytes(deviceAddress, *data, data + 1, count - 1);
}

bool i2c_master_writeZeroBytes(const uint8_t deviceAddress, const uint8_t zeroCount) {
    return i2c_master_writeRegisterZeroBytes(deviceAddress, 0, zeroCount - 1);
}
