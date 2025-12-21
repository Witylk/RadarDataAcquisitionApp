/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SPI_DRIVER_H
#define _SPI_DRIVER_H
#include "usci.h"
#include <stdio.h>
//SPI
#define SPI0_SCLK_PinMsk GPIO_PIN_13
#define SPI0_SCLK_PinNum 13
#define SPI0_SCLK_Port GPIO0

#define SPI0_MOSI_PinMsk GPIO_PIN_14
#define SPI0_MOSI_PinNum 14
#define SPI0_MOSI_Port GPIO0

#define SPI0_MISO_PinMsk GPIO_PIN_15
#define SPI0_MISO_PinNum 15
#define SPI0_MISO_Port GPIO0



#define USCI_CMD_CLK_ON    USCI_U7816_CMD_CLK_ON
#define USCI_CMD_CLK_OFF   USCI_U7816_CMD_CLK_OFF



extern usci_handler_t SPI0;

void SPI_Init(void);
void SPI_TestHandle(void);
uint16_t RF_ReadRegSPI(uint8_t seqNum,uint8_t u8Addr);
uint16_t RF_ReadRegSPI_Ext(uint8_t seqNum, uint8_t u8Addr, uint16_t *pu16Rdata);

void RF_WriteRegSPI_Fast(uint8_t seqNum,uint8_t u8Addr, uint16_t u16Data);
uint8_t RF_WriteRegSPI_Fast_Ext(uint8_t seqNum, uint8_t u8Addr, uint16_t u16Data, uint8_t check_en);

uint16_t RF_ReadWaveSPI(uint8_t seqNum,uint8_t Row_Addr, uint8_t Col_Addr);
uint16_t RF_ReadWaveSPI_Ext(uint8_t seqNum, uint8_t Row_Addr, uint8_t Col_Addr,uint16_t *pu16Rdata);

void RF_WriteWaveSPI(uint8_t seqNum,uint8_t Row_Addr, uint8_t Col_Addr, uint16_t u16Data);
uint8_t RF_WriteWaveSPI_Ext(uint8_t seqNum,uint8_t Row_Addr, uint8_t Col_Addr, uint16_t u16Data,uint8_t check_en);

void RF_WriteCmdSPI(uint8_t seqNum,uint8_t u8Cmd);
void RF_WriteCmdSPI_Ext(uint8_t seqNum,uint8_t u8Cmd);
#endif /*_SPI_DRIVER_H */

/************************ (C) COPYRIGHT  *****END OF FILE****/
