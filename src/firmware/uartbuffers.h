#pragma once

#define UARTBUFFERS_RX_MAX  64
uint8_t UartRxBuffer[UARTBUFFERS_RX_MAX];
uint8_t UartRxBufferCount = 0;

#define uartbuffers_rxAppend(DATA)  if (UartRxBufferCount < UARTBUFFERS_RX_MAX) { UartRxBuffer[UartRxBufferCount] = DATA; UartRxBufferCount++; }
#define uartbuffers_rxReset()       UartRxBufferCount = 0;

#define UARTBUFFERS_TX_MAX  250
uint8_t UartTxBuffer[UARTBUFFERS_TX_MAX];
uint8_t UartTxBufferStart = 0;
uint8_t UartTxBufferCount = 0;

#define uartbuffers_txAppend(DATA)  if (UartTxBufferCount < UARTBUFFERS_TX_MAX) { UartTxBuffer[UartTxBufferCount] = DATA; UartTxBufferCount++; }
#define uartbuffers_txReset()       UartTxBufferCount = 0; UartTxBufferStart = 0;
