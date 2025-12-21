#ifndef __RF_SPI_CONFIG_H
#define __RF_SPI_CONFIG_H


#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "RC7701N32.h"
#include "GPIO_Driver.h"
#include "Delay_Driver.h"
#include "SPI_Driver.h"
#include "App_Def.h"
//#define RF_RC7701N32
//#define RF_RC7711B
#define RF_RC6011B








typedef struct{
    uint8_t num;
    uint8_t PA_Buf[8];
}STRUCT_PA;






extern  uint8_t waveStartFlag;
extern STRUCT_PA pa_str;

extern uint16_t RF_ReadRegSPI(uint8_t seqNum,uint8_t u8Addr);
void RF_WriteRegSPI(uint8_t seqNum,uint8_t u8Addr, uint16_t u16Data);
extern uint16_t RF_ReadWaveSPI(uint8_t seqNum,uint8_t Row_Addr, uint8_t Col_Addr);
void RF_WriteWaveSPI(uint8_t seqNum,uint8_t Row_Addr, uint8_t Col_Addr, uint16_t u16Data);
void RF_WriteCmdSPI(uint8_t seqNum,uint8_t u8Cmd);

void RF_SPI_Init(void);
void RF_CHIPEN_Disable(void);
void RF_CHIPEN(void);

void waveGenStart(void);
void waveGenStop(void);

void RF_PA_Set(uint8_t sPA_In,STRUCT_PA *sPA_ST);
void RF_PA_FASTSW(uint8_t paNum);
void RF_WriteRegSPI_Fast(uint8_t seqNum,uint8_t u8Addr, uint16_t u16Data);



void RF_Clock40M_En(void);












#endif

