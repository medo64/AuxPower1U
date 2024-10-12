#include <xc.h>
#include "app.h"

void pps_init(void) {
    // unlock PPS
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0;

    // UART 1: RX
    #if defined(_PPS_UART1_RX_SELECTION)
        U1RXPPS = _PPS_UART1_RX_SELECTION;  // redirect input
        #if _PPS_UART1_RX_SELECTION == 0b01000
            ANSELBbits.ANSELB0 = 0;  // digital pin
        #elif _PPS_UART1_RX_SELECTION == 0b01001
            ANSELBbits.ANSELB1 = 0;
        #elif _PPS_UART1_RX_SELECTION == 0b01010
            ANSELBbits.ANSELB2 = 0;
        #elif _PPS_UART1_RX_SELECTION == 0b01011
            ANSELBbits.ANSELB3 = 0;
        #elif _PPS_UART1_RX_SELECTION == 0b01100
            ANSELBbits.ANSELB4 = 0;
        #elif _PPS_UART1_RX_SELECTION == 0b01101
            ANSELBbits.ANSELB5 = 0;
        #elif _PPS_UART1_RX_SELECTION == 0b01110
            ANSELBbits.ANSELB6 = 0;
        #elif _PPS_UART1_RX_SELECTION == 0b01111
            ANSELBbits.ANSELB7 = 0;
        #elif _PPS_UART1_RX_SELECTION == 0b10000
            ANSELCbits.ANSELC0 = 0;
        #elif _PPS_UART1_RX_SELECTION == 0b10001
            ANSELCbits.ANSELC1 = 0;
        #elif _PPS_UART1_RX_SELECTION == 0b10010
            ANSELCbits.ANSELC2 = 0;
        #elif _PPS_UART1_RX_SELECTION == 0b10011
            ANSELCbits.ANSELC3 = 0;
        #elif _PPS_UART1_RX_SELECTION == 0b10100
            ANSELCbits.ANSELC4 = 0;
        #elif _PPS_UART1_RX_SELECTION == 0b10101
            ANSELCbits.ANSELC5 = 0;
        #elif _PPS_UART1_RX_SELECTION == 0b10110
            ANSELCbits.ANSELC6 = 0;
        #elif _PPS_UART1_RX_SELECTION == 0b10111
            ANSELCbits.ANSELC7 = 0;
        #else
            #error "Cannot set UART2 RX PPS"
        #endif
    #endif

    // UART 1: TX
    #if defined(_PPS_UART1_TX_SELECTION)
        #if _PPS_UART1_TX_SELECTION == 0b01000
            RB0PPS = 0b010011;  // redirect output
        #elif _PPS_UART1_TX_SELECTION == 0b01001
            RB1PPS = 0b010011;
        #elif _PPS_UART1_TX_SELECTION == 0b01010
            RB2PPS = 0b010011;
        #elif _PPS_UART1_TX_SELECTION == 0b01011
            RB3PPS = 0b010011;
        #elif _PPS_UART1_TX_SELECTION == 0b01100
            RB4PPS = 0b010011;
        #elif _PPS_UART1_TX_SELECTION == 0b01101
            RB5PPS = 0b010011;
        #elif _PPS_UART1_TX_SELECTION == 0b01110
            RB6PPS = 0b010011;
        #elif _PPS_UART1_TX_SELECTION == 0b01111
            RB7PPS = 0b010011;
        #elif _PPS_UART1_TX_SELECTION == 0b10000
            RC0PPS = 0b010011;
        #elif _PPS_UART1_TX_SELECTION == 0b10001
            RC1PPS = 0b010011;
        #elif _PPS_UART1_TX_SELECTION == 0b10010
            RC2PPS = 0b010011;
        #elif _PPS_UART1_TX_SELECTION == 0b10011
            RC3PPS = 0b010011;
        #elif _PPS_UART1_TX_SELECTION == 0b10100
            RC4PPS = 0b010011;
        #elif _PPS_UART1_TX_SELECTION == 0b10101
            RC5PPS = 0b010011;
        #elif _PPS_UART1_TX_SELECTION == 0b10110
            RC6PPS = 0b010011;
        #elif _PPS_UART1_TX_SELECTION == 0b10111
            RC7PPS = 0b010011;
        #else
            #error "Cannot set UART1 TX PPS"
        #endif
    #endif


    // UART 2: RX
    #if defined(_PPS_UART2_RX_SELECTION)
        U2RXPPS = _PPS_UART2_RX_SELECTION;  // redirect input
        #if _PPS_UART2_RX_SELECTION == 0b01000
            ANSELBbits.ANSELB0 = 0;  // digital pin
        #elif _PPS_UART2_RX_SELECTION == 0b01001
            ANSELBbits.ANSELB1 = 0;
        #elif _PPS_UART2_RX_SELECTION == 0b01010
            ANSELBbits.ANSELB2 = 0;
        #elif _PPS_UART2_RX_SELECTION == 0b01011
            ANSELBbits.ANSELB3 = 0;
        #elif _PPS_UART2_RX_SELECTION == 0b01100
            ANSELBbits.ANSELB4 = 0;
        #elif _PPS_UART2_RX_SELECTION == 0b01101
            ANSELBbits.ANSELB5 = 0;
        #elif _PPS_UART2_RX_SELECTION == 0b01110
            ANSELBbits.ANSELB6 = 0;
        #elif _PPS_UART2_RX_SELECTION == 0b01111
            ANSELBbits.ANSELB7 = 0;
        #elif _PPS_UART2_RX_SELECTION == 0b10000
            ANSELCbits.ANSELC0 = 0;
        #elif _PPS_UART2_RX_SELECTION == 0b10001
            ANSELCbits.ANSELC1 = 0;
        #elif _PPS_UART2_RX_SELECTION == 0b10010
            ANSELCbits.ANSELC2 = 0;
        #elif _PPS_UART2_RX_SELECTION == 0b10011
            ANSELCbits.ANSELC3 = 0;
        #elif _PPS_UART2_RX_SELECTION == 0b10100
            ANSELCbits.ANSELC4 = 0;
        #elif _PPS_UART2_RX_SELECTION == 0b10101
            ANSELCbits.ANSELC5 = 0;
        #elif _PPS_UART2_RX_SELECTION == 0b10110
            ANSELCbits.ANSELC6 = 0;
        #elif _PPS_UART2_RX_SELECTION == 0b10111
            ANSELCbits.ANSELC7 = 0;
        #else
            #error "Cannot set UART2 RX PPS"
        #endif
    #endif

    // UART 2: TX
    #if defined(_PPS_UART2_TX_SELECTION)
        #if _PPS_UART2_TX_SELECTION == 0b01000
            RB0PPS = 0b010110;  // redirect output
        #elif _PPS_UART2_TX_SELECTION == 0b01001
            RB1PPS = 0b010110;
        #elif _PPS_UART2_TX_SELECTION == 0b01010
            RB2PPS = 0b010110;
        #elif _PPS_UART2_TX_SELECTION == 0b01011
            RB3PPS = 0b010110;
        #elif _PPS_UART2_TX_SELECTION == 0b01100
            RB4PPS = 0b010110;
        #elif _PPS_UART2_TX_SELECTION == 0b01101
            RB5PPS = 0b010110;
        #elif _PPS_UART2_TX_SELECTION == 0b01110
            RB6PPS = 0b010110;
        #elif _PPS_UART2_TX_SELECTION == 0b01111
            RB7PPS = 0b010110;
        #elif _PPS_UART2_TX_SELECTION == 0b10000
            RC0PPS = 0b010110;
        #elif _PPS_UART2_TX_SELECTION == 0b10001
            RC1PPS = 0b010110;
        #elif _PPS_UART2_TX_SELECTION == 0b10010
            RC2PPS = 0b010110;
        #elif _PPS_UART2_TX_SELECTION == 0b10011
            RC3PPS = 0b010110;
        #elif _PPS_UART2_TX_SELECTION == 0b10100
            RC4PPS = 0b010110;
        #elif _PPS_UART2_TX_SELECTION == 0b10101
            RC5PPS = 0b010110;
        #elif _PPS_UART2_TX_SELECTION == 0b10110
            RC6PPS = 0b010110;
        #elif _PPS_UART2_TX_SELECTION == 0b10111
            RC7PPS = 0b010110;
        #else
            #error "Cannot set UART2 TX PPS"
        #endif
    #endif

    // I2C 1: SCL
    #if defined(_PPS_I2C1_SCL_SELECTION)
        I2C1SCLPPS = _PPS_I2C1_SCL_SELECTION;  // redirect input
        #if _PPS_I2C1_SCL_SELECTION == 0b01000
            RB0PPS = 0b00100001;     // redirect output
            LATBbits.LATB0 = 0;      // clear write latches
            TRISBbits.TRISB0 = 0;    // make pins output
            ANSELBbits.ANSELB0 = 0;  // make pins digital
            SLRCONBbits.SLRB1 = 0;   // no GPIO slew rate limiting
            ODCONBbits.ODCB0 = 1;    // make pins open-drain
            RB0I2Cbits.SLEW = 1;     // I2C specific slew rate limiting is enabled   
            RB0I2Cbits.PU = 0b00;    // external pull-ups used
            RB0I2Cbits.TH = 0b01;    // SMBus 3.0 (1.35 V) input threshold
        #elif _PPS_I2C1_SCL_SELECTION == 0b01001
            RB1PPS = 0b00100001;
            LATBbits.LATB1 = 0;
            TRISBbits.TRISB1 = 0;
            ANSELBbits.ANSELB1 = 0;
            SLRCONBbits.SLRB1 = 0;
            ODCONBbits.ODCB1 = 1;
            RB1I2Cbits.SLEW = 1;
            RB1I2Cbits.PU = 0b00;
            RB1I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SCL_SELECTION == 0b01010
            RB2PPS = 0b00100001;
            LATBbits.LATB2 = 0;
            TRISBbits.TRISB2 = 0;
            ANSELBbits.ANSELB2 = 0;
            SLRCONBbits.SLRB2 = 0;
            ODCONBbits.ODCB2 = 1;
            RB2I2Cbits.SLEW = 1;
            RB2I2Cbits.PU = 0b00;
            RB2I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SCL_SELECTION == 0b01011
            RB3PPS = 0b00100001;
            LATBbits.LATB3 = 0;
            TRISBbits.TRISB3 = 0;
            ANSELBbits.ANSELB3 = 0;
            SLRCONBbits.SLRB3 = 0;
            ODCONBbits.ODCB3 = 1;
            RB3I2Cbits.SLEW = 1;
            RB3I2Cbits.PU = 0b00;
            RB3I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SCL_SELECTION == 0b01100
            RB4PPS = 0b00100001;
            LATBbits.LATB4 = 0;
            TRISBbits.TRISB4 = 0;
            ANSELBbits.ANSELB4 = 0;
            SLRCONBbits.SLRB4 = 0;
            ODCONBbits.ODCB4 = 1;
            RB4I2Cbits.SLEW = 1;
            RB4I2Cbits.PU = 0b00;
            RB4I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SCL_SELECTION == 0b01101
            RB5PPS = 0b00100001;
            LATBbits.LATB5 = 0;
            TRISBbits.TRISB5 = 0;
            ANSELBbits.ANSELB5 = 0;
            SLRCONBbits.SLRB5 = 0;
            ODCONBbits.ODCB5 = 1;
            RB5I2Cbits.SLEW = 1;
            RB5I2Cbits.PU = 0b00;
            RB5I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SCL_SELECTION == 0b01110
            RB6PPS = 0b00100001;
            LATBbits.LATB6 = 0;
            TRISBbits.TRISB6 = 0;
            ANSELBbits.ANSELB6 = 0;
            SLRCONBbits.SLRB6 = 0;
            ODCONBbits.ODCB6 = 1;
            RB6I2Cbits.SLEW = 1;
            RB6I2Cbits.PU = 0b00;
            RB6I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SCL_SELECTION == 0b01111
            RB7PPS = 0b00100001;
            LATBbits.LATB7 = 0;
            TRISBbits.TRISB7 = 0;
            ANSELBbits.ANSELB7 = 0;
            SLRCONBbits.SLRB7 = 0;
            ODCONBbits.ODCB7 = 1;
            RB7I2Cbits.SLEW = 1;
            RB7I2Cbits.PU = 0b00;
            RB7I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SCL_SELECTION == 0b10000
            RC0PPS = 0b00100001;
            LATCbits.LATC0 = 0;
            TRISCbits.TRISC0 = 0;
            ANSELCbits.ANSELC0 = 0;
            SLRCONCbits.SLRC0 = 0;
            ODCONCbits.ODCC0 = 1;
            RC0I2Cbits.SLEW = 1;
            RC0I2Cbits.PU = 0b00;
            RC0I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SCL_SELECTION == 0b10001
            RC1PPS = 0b00100001;
            LATCbits.LATC1 = 0;
            TRISCbits.TRISC1 = 0;
            ANSELCbits.ANSELC1 = 0;
            SLRCONCbits.SLRC1 = 0;
            ODCONCbits.ODCC1 = 1;
            RC1I2Cbits.SLEW = 1;
            RC1I2Cbits.PU = 0b00;
            RC1I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SCL_SELECTION == 0b10010
            RC2PPS = 0b00100001;
            LATCbits.LATC2 = 0;
            TRISCbits.TRISC2 = 0;
            ANSELCbits.ANSELC2 = 0;
            SLRCONCbits.SLRC2 = 0;
            ODCONCbits.ODCC2 = 1;
            RC2I2Cbits.SLEW = 1;
            RC2I2Cbits.PU = 0b00;
            RC2I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SCL_SELECTION == 0b10011
            RC3PPS = 0b00100001;
            LATCbits.LATC3 = 0;
            TRISCbits.TRISC3 = 0;
            ANSELCbits.ANSELC3 = 0;
            SLRCONCbits.SLRC3 = 0;
            ODCONCbits.ODCC3 = 1;
            RC3I2Cbits.SLEW = 1;
            RC3I2Cbits.PU = 0b00;
            RC3I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SCL_SELECTION == 0b10100
            RC4PPS = 0b00100001;
            LATCbits.LATC4 = 0;
            TRISCbits.TRISC4 = 0;
            ANSELCbits.ANSELC4 = 0;
            SLRCONCbits.SLRC4 = 0;
            ODCONCbits.ODCC4 = 1;
            RC4I2Cbits.SLEW = 1;
            RC4I2Cbits.PU = 0b00;
            RC4I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SCL_SELECTION == 0b10101
            RC5PPS = 0b00100001;
            LATCbits.LATC5 = 0;
            TRISCbits.TRISC5 = 0;
            ANSELCbits.ANSELC5 = 0;
            SLRCONCbits.SLRC5 = 0;
            ODCONCbits.ODCC5 = 1;
            RC5I2Cbits.SLEW = 1;
            RC5I2Cbits.PU = 0b00;
            RC5I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SCL_SELECTION == 0b10110
            RC6PPS = 0b00100001;
            LATCbits.LATC6 = 0;
            TRISCbits.TRISC6 = 0;
            ANSELCbits.ANSELC6 = 0;
            SLRCONCbits.SLRC6 = 0;
            ODCONCbits.ODCC6 = 1;
            RC6I2Cbits.SLEW = 1;
            RC6I2Cbits.PU = 0b00;
            RC6I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SCL_SELECTION == 0b10111
            RC7PPS = 0b00100001;
            LATCbits.LATC7 = 0;
            TRISCbits.TRISC7 = 0;
            ANSELCbits.ANSELC7 = 0;
            SLRCONCbits.SLRC7 = 0;
            ODCONCbits.ODCC7 = 1;
            RC7I2Cbits.SLEW = 1;
            RC7I2Cbits.PU = 0b00;
            RC7I2Cbits.TH = 0b01;
        #else
            #error "Cannot set I2C1 SCL PPS"
        #endif
    #endif

    // I2C 1: SDA
    #if defined(_PPS_I2C1_SDA_SELECTION)
        I2C1SDAPPS = _PPS_I2C1_SDA_SELECTION;  // redirect input
        #if _PPS_I2C1_SDA_SELECTION == 0b01000
            RB0PPS = 0b00100010;     // redirect output
            LATBbits.LATB0 = 0;      // clear write latches
            TRISBbits.TRISB0 = 0;    // make pins output
            ANSELBbits.ANSELB0 = 0;  // make pins digital
            SLRCONBbits.SLRB1 = 0;   // no GPIO slew rate limiting
            ODCONBbits.ODCB0 = 1;    // make pins open-drain
            RB0I2Cbits.SLEW = 1;     // I2C specific slew rate limiting is enabled   
            RB0I2Cbits.PU = 0b00;    // external pull-ups used
            RB0I2Cbits.TH = 0b01;    // SMBus 3.0 (1.35 V) input threshold
        #elif _PPS_I2C1_SDA_SELECTION == 0b01001
            RB1PPS = 0b00100010;
            LATBbits.LATB1 = 0;
            TRISBbits.TRISB1 = 0;
            ANSELBbits.ANSELB1 = 0;
            SLRCONBbits.SLRB1 = 0;
            ODCONBbits.ODCB1 = 1;
            RB1I2Cbits.SLEW = 1;
            RB1I2Cbits.PU = 0b00;
            RB1I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SDA_SELECTION == 0b01010
            RB2PPS = 0b00100010;
            LATBbits.LATB2 = 0;
            TRISBbits.TRISB2 = 0;
            ANSELBbits.ANSELB2 = 0;
            SLRCONBbits.SLRB2 = 0;
            ODCONBbits.ODCB2 = 1;
            RB2I2Cbits.SLEW = 1;
            RB2I2Cbits.PU = 0b00;
            RB2I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SDA_SELECTION == 0b01011
            RB3PPS = 0b00100010;
            LATBbits.LATB3 = 0;
            TRISBbits.TRISB3 = 0;
            ANSELBbits.ANSELB3 = 0;
            SLRCONBbits.SLRB3 = 0;
            ODCONBbits.ODCB3 = 1;
            RB3I2Cbits.SLEW = 1;
            RB3I2Cbits.PU = 0b00;
            RB3I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SDA_SELECTION == 0b01100
            RB4PPS = 0b00100010;
            LATBbits.LATB4 = 0;
            TRISBbits.TRISB4 = 0;
            ANSELBbits.ANSELB4 = 0;
            SLRCONBbits.SLRB4 = 0;
            ODCONBbits.ODCB4 = 1;
            RB4I2Cbits.SLEW = 1;
            RB4I2Cbits.PU = 0b00;
            RB4I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SDA_SELECTION == 0b01101
            RB5PPS = 0b00100010;
            LATBbits.LATB5 = 0;
            TRISBbits.TRISB5 = 0;
            ANSELBbits.ANSELB5 = 0;
            SLRCONBbits.SLRB5 = 0;
            ODCONBbits.ODCB5 = 1;
            RB5I2Cbits.SLEW = 1;
            RB5I2Cbits.PU = 0b00;
            RB5I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SDA_SELECTION == 0b01110
            RB6PPS = 0b00100010;
            LATBbits.LATB6 = 0;
            TRISBbits.TRISB6 = 0;
            ANSELBbits.ANSELB6 = 0;
            SLRCONBbits.SLRB6 = 0;
            ODCONBbits.ODCB6 = 1;
            RB6I2Cbits.SLEW = 1;
            RB6I2Cbits.PU = 0b00;
            RB6I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SDA_SELECTION == 0b01111
            RB7PPS = 0b00100010;
            LATBbits.LATB7 = 0;
            TRISBbits.TRISB7 = 0;
            ANSELBbits.ANSELB7 = 0;
            SLRCONBbits.SLRB7 = 0;
            ODCONBbits.ODCB7 = 1;
            RB7I2Cbits.SLEW = 1;
            RB7I2Cbits.PU = 0b00;
            RB7I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SDA_SELECTION == 0b10000
            RC0PPS = 0b00100010;
            LATCbits.LATC0 = 0;
            TRISCbits.TRISC0 = 0;
            ANSELCbits.ANSELC0 = 0;
            SLRCONCbits.SLRC0 = 0;
            ODCONCbits.ODCC0 = 1;
            RC0I2Cbits.SLEW = 1;
            RC0I2Cbits.PU = 0b00;
            RC0I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SDA_SELECTION == 0b10001
            RC1PPS = 0b00100010;
            LATCbits.LATC1 = 0;
            TRISCbits.TRISC1 = 0;
            ANSELCbits.ANSELC1 = 0;
            SLRCONCbits.SLRC1 = 0;
            ODCONCbits.ODCC1 = 1;
            RC1I2Cbits.SLEW = 1;
            RC1I2Cbits.PU = 0b00;
            RC1I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SDA_SELECTION == 0b10010
            RC2PPS = 0b00100010;
            LATCbits.LATC2 = 0;
            TRISCbits.TRISC2 = 0;
            ANSELCbits.ANSELC2 = 0;
            SLRCONCbits.SLRC2 = 0;
            ODCONCbits.ODCC2 = 1;
            RC2I2Cbits.SLEW = 1;
            RC2I2Cbits.PU = 0b00;
            RC2I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SDA_SELECTION == 0b10011
            RC3PPS = 0b00100010;
            LATCbits.LATC3 = 0;
            TRISCbits.TRISC3 = 0;
            ANSELCbits.ANSELC3 = 0;
            SLRCONCbits.SLRC3 = 0;
            ODCONCbits.ODCC3 = 1;
            RC3I2Cbits.SLEW = 1;
            RC3I2Cbits.PU = 0b00;
            RC3I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SDA_SELECTION == 0b10100
            RC4PPS = 0b00100010;
            LATCbits.LATC4 = 0;
            TRISCbits.TRISC4 = 0;
            ANSELCbits.ANSELC4 = 0;
            SLRCONCbits.SLRC4 = 0;
            ODCONCbits.ODCC4 = 1;
            RC4I2Cbits.SLEW = 1;
            RC4I2Cbits.PU = 0b00;
            RC4I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SDA_SELECTION == 0b10101
            RC5PPS = 0b00100010;
            LATCbits.LATC5 = 0;
            TRISCbits.TRISC5 = 0;
            ANSELCbits.ANSELC5 = 0;
            SLRCONCbits.SLRC5 = 0;
            ODCONCbits.ODCC5 = 1;
            RC5I2Cbits.SLEW = 1;
            RC5I2Cbits.PU = 0b00;
            RC5I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SDA_SELECTION == 0b10110
            RC6PPS = 0b00100010;
            LATCbits.LATC6 = 0;
            TRISCbits.TRISC6 = 0;
            ANSELCbits.ANSELC6 = 0;
            SLRCONCbits.SLRC6 = 0;
            ODCONCbits.ODCC6 = 1;
            RC6I2Cbits.SLEW = 1;
            RC6I2Cbits.PU = 0b00;
            RC6I2Cbits.TH = 0b01;
        #elif _PPS_I2C1_SDA_SELECTION == 0b10111
            RC7PPS = 0b00100010;
            LATCbits.LATC7 = 0;
            TRISCbits.TRISC7 = 0;
            ANSELCbits.ANSELC7 = 0;
            SLRCONCbits.SLRC7 = 0;
            ODCONCbits.ODCC7 = 1;
            RC7I2Cbits.SLEW = 1;
            RC7I2Cbits.PU = 0b00;
            RC7I2Cbits.TH = 0b01;
        #else
            #error "Cannot set I2C1 SDA PPS"
        #endif
    #endif

    // I2C 2: SCL
    #if defined(_PPS_I2C2_SCL_SELECTION)
        I2C2SCLPPS = _PPS_I2C2_SCL_SELECTION;  // redirect input
        #if _PPS_I2C2_SCL_SELECTION == 0b01000
            RB0PPS = 0b00100011;     // redirect output
            LATBbits.LATB0 = 0;      // clear write latches
            TRISBbits.TRISB0 = 0;    // make pins output
            ANSELBbits.ANSELB0 = 0;  // make pins digital
            SLRCONBbits.SLRB1 = 0;   // no GPIO slew rate limiting
            ODCONBbits.ODCB0 = 1;    // make pins open-drain
            RB0I2Cbits.SLEW = 1;     // I2C specific slew rate limiting is enabled   
            RB0I2Cbits.PU = 0b00;    // external pull-ups used
            RB0I2Cbits.TH = 0b01;    // SMBus 3.0 (1.35 V) input threshold
        #elif _PPS_I2C2_SCL_SELECTION == 0b01001
            RB1PPS = 0b00100011;
            LATBbits.LATB1 = 0;
            TRISBbits.TRISB1 = 0;
            ANSELBbits.ANSELB1 = 0;
            SLRCONBbits.SLRB1 = 0;
            ODCONBbits.ODCB1 = 1;
            RB1I2Cbits.SLEW = 1;
            RB1I2Cbits.PU = 0b00;
            RB1I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SCL_SELECTION == 0b01010
            RB2PPS = 0b00100011;
            LATBbits.LATB2 = 0;
            TRISBbits.TRISB2 = 0;
            ANSELBbits.ANSELB2 = 0;
            SLRCONBbits.SLRB2 = 0;
            ODCONBbits.ODCB2 = 1;
            RB2I2Cbits.SLEW = 1;
            RB2I2Cbits.PU = 0b00;
            RB2I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SCL_SELECTION == 0b01011
            RB3PPS = 0b00100011;
            LATBbits.LATB3 = 0;
            TRISBbits.TRISB3 = 0;
            ANSELBbits.ANSELB3 = 0;
            SLRCONBbits.SLRB3 = 0;
            ODCONBbits.ODCB3 = 1;
            RB3I2Cbits.SLEW = 1;
            RB3I2Cbits.PU = 0b00;
            RB3I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SCL_SELECTION == 0b01100
            RB4PPS = 0b00100011;
            LATBbits.LATB4 = 0;
            TRISBbits.TRISB4 = 0;
            ANSELBbits.ANSELB4 = 0;
            SLRCONBbits.SLRB4 = 0;
            ODCONBbits.ODCB4 = 1;
            RB4I2Cbits.SLEW = 1;
            RB4I2Cbits.PU = 0b00;
            RB4I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SCL_SELECTION == 0b01101
            RB5PPS = 0b00100011;
            LATBbits.LATB5 = 0;
            TRISBbits.TRISB5 = 0;
            ANSELBbits.ANSELB5 = 0;
            SLRCONBbits.SLRB5 = 0;
            ODCONBbits.ODCB5 = 1;
            RB5I2Cbits.SLEW = 1;
            RB5I2Cbits.PU = 0b00;
            RB5I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SCL_SELECTION == 0b01110
            RB6PPS = 0b00100011;
            LATBbits.LATB6 = 0;
            TRISBbits.TRISB6 = 0;
            ANSELBbits.ANSELB6 = 0;
            SLRCONBbits.SLRB6 = 0;
            ODCONBbits.ODCB6 = 1;
            RB6I2Cbits.SLEW = 1;
            RB6I2Cbits.PU = 0b00;
            RB6I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SCL_SELECTION == 0b01111
            RB7PPS = 0b00100011;
            LATBbits.LATB7 = 0;
            TRISBbits.TRISB7 = 0;
            ANSELBbits.ANSELB7 = 0;
            SLRCONBbits.SLRB7 = 0;
            ODCONBbits.ODCB7 = 1;
            RB7I2Cbits.SLEW = 1;
            RB7I2Cbits.PU = 0b00;
            RB7I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SCL_SELECTION == 0b10000
            RC0PPS = 0b00100011;
            LATCbits.LATC0 = 0;
            TRISCbits.TRISC0 = 0;
            ANSELCbits.ANSELC0 = 0;
            SLRCONCbits.SLRC0 = 0;
            ODCONCbits.ODCC0 = 1;
            RC0I2Cbits.SLEW = 1;
            RC0I2Cbits.PU = 0b00;
            RC0I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SCL_SELECTION == 0b10001
            RC1PPS = 0b00100011;
            LATCbits.LATC1 = 0;
            TRISCbits.TRISC1 = 0;
            ANSELCbits.ANSELC1 = 0;
            SLRCONCbits.SLRC1 = 0;
            ODCONCbits.ODCC1 = 1;
            RC1I2Cbits.SLEW = 1;
            RC1I2Cbits.PU = 0b00;
            RC1I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SCL_SELECTION == 0b10010
            RC2PPS = 0b00100011;
            LATCbits.LATC2 = 0;
            TRISCbits.TRISC2 = 0;
            ANSELCbits.ANSELC2 = 0;
            SLRCONCbits.SLRC2 = 0;
            ODCONCbits.ODCC2 = 1;
            RC2I2Cbits.SLEW = 1;
            RC2I2Cbits.PU = 0b00;
            RC2I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SCL_SELECTION == 0b10011
            RC3PPS = 0b00100011;
            LATCbits.LATC3 = 0;
            TRISCbits.TRISC3 = 0;
            ANSELCbits.ANSELC3 = 0;
            SLRCONCbits.SLRC3 = 0;
            ODCONCbits.ODCC3 = 1;
            RC3I2Cbits.SLEW = 1;
            RC3I2Cbits.PU = 0b00;
            RC3I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SCL_SELECTION == 0b10100
            RC4PPS = 0b00100011;
            LATCbits.LATC4 = 0;
            TRISCbits.TRISC4 = 0;
            ANSELCbits.ANSELC4 = 0;
            SLRCONCbits.SLRC4 = 0;
            ODCONCbits.ODCC4 = 1;
            RC4I2Cbits.SLEW = 1;
            RC4I2Cbits.PU = 0b00;
            RC4I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SCL_SELECTION == 0b10101
            RC5PPS = 0b00100011;
            LATCbits.LATC5 = 0;
            TRISCbits.TRISC5 = 0;
            ANSELCbits.ANSELC5 = 0;
            SLRCONCbits.SLRC5 = 0;
            ODCONCbits.ODCC5 = 1;
            RC5I2Cbits.SLEW = 1;
            RC5I2Cbits.PU = 0b00;
            RC5I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SCL_SELECTION == 0b10110
            RC6PPS = 0b00100011;
            LATCbits.LATC6 = 0;
            TRISCbits.TRISC6 = 0;
            ANSELCbits.ANSELC6 = 0;
            SLRCONCbits.SLRC6 = 0;
            ODCONCbits.ODCC6 = 1;
            RC6I2Cbits.SLEW = 1;
            RC6I2Cbits.PU = 0b00;
            RC6I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SCL_SELECTION == 0b10111
            RC7PPS = 0b00100011;
            LATCbits.LATC7 = 0;
            TRISCbits.TRISC7 = 0;
            ANSELCbits.ANSELC7 = 0;
            SLRCONCbits.SLRC7 = 0;
            ODCONCbits.ODCC7 = 1;
            RC7I2Cbits.SLEW = 1;
            RC7I2Cbits.PU = 0b00;
            RC7I2Cbits.TH = 0b01;
        #else
            #error "Cannot set I2C2 SCL PPS"
        #endif
    #endif

    // I2C 2: SDA
    #if defined(_PPS_I2C2_SDA_SELECTION)
        I2C2SDAPPS = _PPS_I2C2_SDA_SELECTION;  // redirect input
        #if _PPS_I2C2_SDA_SELECTION == 0b01000
            RB0PPS = 0b00100100;     // redirect output
            LATBbits.LATB0 = 0;      // clear write latches
            TRISBbits.TRISB0 = 0;    // make pins output
            ANSELBbits.ANSELB0 = 0;  // make pins digital
            SLRCONBbits.SLRB1 = 0;   // no GPIO slew rate limiting
            ODCONBbits.ODCB0 = 1;    // make pins open-drain
            RB0I2Cbits.SLEW = 1;     // I2C specific slew rate limiting is enabled   
            RB0I2Cbits.PU = 0b00;    // external pull-ups used
            RB0I2Cbits.TH = 0b01;    // SMBus 3.0 (1.35 V) input threshold
        #elif _PPS_I2C2_SDA_SELECTION == 0b01001
            RB1PPS = 0b00100100;
            LATBbits.LATB1 = 0;
            TRISBbits.TRISB1 = 0;
            ANSELBbits.ANSELB1 = 0;
            SLRCONBbits.SLRB1 = 0;
            ODCONBbits.ODCB1 = 1;
            RB1I2Cbits.SLEW = 1;
            RB1I2Cbits.PU = 0b00;
            RB1I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SDA_SELECTION == 0b01010
            RB2PPS = 0b00100100;
            LATBbits.LATB2 = 0;
            TRISBbits.TRISB2 = 0;
            ANSELBbits.ANSELB2 = 0;
            SLRCONBbits.SLRB2 = 0;
            ODCONBbits.ODCB2 = 1;
            RB2I2Cbits.SLEW = 1;
            RB2I2Cbits.PU = 0b00;
            RB2I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SDA_SELECTION == 0b01011
            RB3PPS = 0b00100100;
            LATBbits.LATB3 = 0;
            TRISBbits.TRISB3 = 0;
            ANSELBbits.ANSELB3 = 0;
            SLRCONBbits.SLRB3 = 0;
            ODCONBbits.ODCB3 = 1;
            RB3I2Cbits.SLEW = 1;
            RB3I2Cbits.PU = 0b00;
            RB3I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SDA_SELECTION == 0b01100
            RB4PPS = 0b00100100;
            LATBbits.LATB4 = 0;
            TRISBbits.TRISB4 = 0;
            ANSELBbits.ANSELB4 = 0;
            SLRCONBbits.SLRB4 = 0;
            ODCONBbits.ODCB4 = 1;
            RB4I2Cbits.SLEW = 1;
            RB4I2Cbits.PU = 0b00;
            RB4I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SDA_SELECTION == 0b01101
            RB5PPS = 0b00100100;
            LATBbits.LATB5 = 0;
            TRISBbits.TRISB5 = 0;
            ANSELBbits.ANSELB5 = 0;
            SLRCONBbits.SLRB5 = 0;
            ODCONBbits.ODCB5 = 1;
            RB5I2Cbits.SLEW = 1;
            RB5I2Cbits.PU = 0b00;
            RB5I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SDA_SELECTION == 0b01110
            RB6PPS = 0b00100100;
            LATBbits.LATB6 = 0;
            TRISBbits.TRISB6 = 0;
            ANSELBbits.ANSELB6 = 0;
            SLRCONBbits.SLRB6 = 0;
            ODCONBbits.ODCB6 = 1;
            RB6I2Cbits.SLEW = 1;
            RB6I2Cbits.PU = 0b00;
            RB6I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SDA_SELECTION == 0b01111
            RB7PPS = 0b00100100;
            LATBbits.LATB7 = 0;
            TRISBbits.TRISB7 = 0;
            ANSELBbits.ANSELB7 = 0;
            SLRCONBbits.SLRB7 = 0;
            ODCONBbits.ODCB7 = 1;
            RB7I2Cbits.SLEW = 1;
            RB7I2Cbits.PU = 0b00;
            RB7I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SDA_SELECTION == 0b10000
            RC0PPS = 0b00100100;
            LATCbits.LATC0 = 0;
            TRISCbits.TRISC0 = 0;
            ANSELCbits.ANSELC0 = 0;
            SLRCONCbits.SLRC0 = 0;
            ODCONCbits.ODCC0 = 1;
            RC0I2Cbits.SLEW = 1;
            RC0I2Cbits.PU = 0b00;
            RC0I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SDA_SELECTION == 0b10001
            RC1PPS = 0b00100100;
            LATCbits.LATC1 = 0;
            TRISCbits.TRISC1 = 0;
            ANSELCbits.ANSELC1 = 0;
            SLRCONCbits.SLRC1 = 0;
            ODCONCbits.ODCC1 = 1;
            RC1I2Cbits.SLEW = 1;
            RC1I2Cbits.PU = 0b00;
            RC1I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SDA_SELECTION == 0b10010
            RC2PPS = 0b00100100;
            LATCbits.LATC2 = 0;
            TRISCbits.TRISC2 = 0;
            ANSELCbits.ANSELC2 = 0;
            SLRCONCbits.SLRC2 = 0;
            ODCONCbits.ODCC2 = 1;
            RC2I2Cbits.SLEW = 1;
            RC2I2Cbits.PU = 0b00;
            RC2I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SDA_SELECTION == 0b10011
            RC3PPS = 0b00100100;
            LATCbits.LATC3 = 0;
            TRISCbits.TRISC3 = 0;
            ANSELCbits.ANSELC3 = 0;
            SLRCONCbits.SLRC3 = 0;
            ODCONCbits.ODCC3 = 1;
            RC3I2Cbits.SLEW = 1;
            RC3I2Cbits.PU = 0b00;
            RC3I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SDA_SELECTION == 0b10100
            RC4PPS = 0b00100100;
            LATCbits.LATC4 = 0;
            TRISCbits.TRISC4 = 0;
            ANSELCbits.ANSELC4 = 0;
            SLRCONCbits.SLRC4 = 0;
            ODCONCbits.ODCC4 = 1;
            RC4I2Cbits.SLEW = 1;
            RC4I2Cbits.PU = 0b00;
            RC4I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SDA_SELECTION == 0b10101
            RC5PPS = 0b00100100;
            LATCbits.LATC5 = 0;
            TRISCbits.TRISC5 = 0;
            ANSELCbits.ANSELC5 = 0;
            SLRCONCbits.SLRC5 = 0;
            ODCONCbits.ODCC5 = 1;
            RC5I2Cbits.SLEW = 1;
            RC5I2Cbits.PU = 0b00;
            RC5I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SDA_SELECTION == 0b10110
            RC6PPS = 0b00100100;
            LATCbits.LATC6 = 0;
            TRISCbits.TRISC6 = 0;
            ANSELCbits.ANSELC6 = 0;
            SLRCONCbits.SLRC6 = 0;
            ODCONCbits.ODCC6 = 1;
            RC6I2Cbits.SLEW = 1;
            RC6I2Cbits.PU = 0b00;
            RC6I2Cbits.TH = 0b01;
        #elif _PPS_I2C2_SDA_SELECTION == 0b10111
            RC7PPS = 0b00100100;
            LATCbits.LATC7 = 0;
            TRISCbits.TRISC7 = 0;
            ANSELCbits.ANSELC7 = 0;
            SLRCONCbits.SLRC7 = 0;
            ODCONCbits.ODCC7 = 1;
            RC7I2Cbits.SLEW = 1;
            RC7I2Cbits.PU = 0b00;
            RC7I2Cbits.TH = 0b01;
        #else
            #error "Cannot set I2C2 SDA PPS"
        #endif
    #endif

    // Lock PPS
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 1;  // cannot be unlocked ever again
}
