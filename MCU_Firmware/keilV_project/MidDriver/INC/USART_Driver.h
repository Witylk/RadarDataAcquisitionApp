/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _USART_DRIVER_H
#define _USART_DRIVER_H
#include "usci.h"
#include <stdio.h>
#include <string.h>


//USART
#define USART0_TX_Pin 2
#define USART0_TX_Port PORT01_PINMUX
#define USART0_RX_Pin 3
#define USART0_RX_Port PORT01_PINMUX


#define USART_REC_MAX_LEN 50
#define USART_MAX_TIMEOUT_NUM 2000




typedef struct
{
    uint8_t RecCount;
    uint32_t RecLen;
    uint8_t RecFinish;
    uint8_t RecData;
    uint8_t RecBuff[USART_REC_MAX_LEN];
    uint8_t timeoutStart;
    uint32_t timeoutCnt;
    uint32_t writeCnt;
}STRUCT_USART;

extern usci_handler_t  USART0;
extern uint8_t usci_uart_rx_it(usci_handler_t *usci);
extern STRUCT_USART strUsart;












void USART_Init(void);
void USART_Transmit_Bytes(usci_handler_t *usci, uint8_t *tx_buf, int32_t size);
int sendchar(int ch);
#endif /*_USART_DRIVER_H */

/************************ (C) COPYRIGHT  *****END OF FILE****/























