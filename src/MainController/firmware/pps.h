/* Josip Medved <jmedved@jmedved.com> * www.medo64.com * MIT License */
// 2024-10-11: Initial version

/**
 * Handling UART output
 *
 * Defines used:
 *   _PPS_UART1_RX_SELECTION <value>  Which pin is to be used for UART1 RX (e.g. RC7: _PPS_UART1_RX_SELECTION = 0b10111)
 *   _PPS_UART1_TX_SELECTION <value>  Which pin is to be used for UART1 TX (e.g. RC6: _PPS_UART1_TX_SELECTION = 0b10110)
 *   _PPS_UART2_RX_SELECTION <value>  Which pin is to be used for UART2 RX (e.g. RB7: _PPS_UART2_RX_SELECTION = 0b01111)
 *   _PPS_UART2_TX_SELECTION <value>  Which pin is to be used for UART2 TX (e.g. RB6: _PPS_UART2_TX_SELECTION = 0b01110)
 *   _PPS_I2C1_SCL_SELECTION <value>  Which pin is to be used for I2C1 SCL (e.g. RC3: _PPS_I2C1_SCL_SELECTION = 0b10011)
 *   _PPS_I2C1_SDA_SELECTION <value>  Which pin is to be used for I2C1 DAT (e.g. RC4: _PPS_I2C1_SDA_SELECTION = 0b10100)
 *   _PPS_I2C2_SCL_SELECTION <value>  Which pin is to be used for I2C2 SCL (e.g. RB1: _PPS_I2C2_SCL_SELECTION = 0b01001)
 *   _PPS_I2C2_SDA_SELECTION <value>  Which pin is to be used for I2C2 DAT (e.g. RB2: _PPS_I2C2_SDA_SELECTION = 0b01010)
 */

#pragma once

void pps_init(void);
