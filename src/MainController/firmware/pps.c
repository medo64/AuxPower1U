#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "app.h"

void pps_init(void) {
    // unlock PPS
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0;

    // UART 1: RX
    #if defined(_PPS_UART1_RX_SELECTION)
        U1RXPPS = _PPS_UART1_RX_SELECTION;
        #if _PPS_UART1_RX_SELECTION == 0b01000
            ANSELBbits.ANSELB0 = 0;
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
            RB0PPS = 0b010011;
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


    // UART 1: RX
    #if defined(_PPS_UART2_RX_SELECTION)
        U2RXPPS = _PPS_UART2_RX_SELECTION;
        #if _PPS_UART2_RX_SELECTION == 0b01000
            ANSELBbits.ANSELB0 = 0;
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
            RB0PPS = 0b010110;
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

    // Lock PPS
    //PPSLOCK = 0x55;
    //PPSLOCK = 0xAA;
    //PPSLOCKbits.PPSLOCKED = 1;  // cannot be unlocked ever again
}
