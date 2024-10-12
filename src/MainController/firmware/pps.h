/* Josip Medved <jmedved@jmedved.com> * www.medo64.com * MIT License */
// 2024-10-11: Initial version

/**
 * Handling UART output
 *
 * Defines used:
 *   _PPS_UART1_RX_SELECTION <value>  Which pin is to be used for UART1 RX (e.g. RB7: _PPS_UART1_RX_SELECTION = 0b01111)
 *   _PPS_UART1_TX_SELECTION <value>  Which pin is to be used for UART1 TX (e.g. RB6: _PPS_UART1_TX_SELECTION = 0b01110)
 *   _PPS_UART2_RX_SELECTION <value>  Which pin is to be used for UART2 RX (e.g. RB7: _PPS_UART1_RX_SELECTION = 0b01111)
 *   _PPS_UART2_TX_SELECTION <value>  Which pin is to be used for UART2 TX (e.g. RB6: _PPS_UART1_TX_SELECTION = 0b01110)
 */

#pragma once
#include <stdbool.h>

void pps_init(void);
