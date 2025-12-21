#ifndef __BB_CONFIG_H
#define __BB_CONFIG_H

#include "SKY32B750.h"
#include "BB_Driver.h"
#include "USART_Driver.h"
#include "main.h"

//#define BB_CLOCK_EN               (Write_M32(0x50000000, 0xFFFFFFFF))  // enable all clock
#define BB_CLOCK_EN               (SYSC->CLK_EN|=RADAR_BB_EN_Msk)  //
#define BB_CLOCK_DIS               (SYSC->CLK_EN&=~RADAR_BB_EN_Msk)  //

#define OPGATE_EN                 (1)
#define OPGATE_DIS                (0)

typedef struct{
    uint64_t txNum_zp0[NUM_TX];
    uint64_t txNum_zp1[NUM_TX];
    uint64_t txNum_zp2[NUM_TX];
    uint64_t txNum_zp3[NUM_TX];
}STRUCT_FFT2D_ZP;

extern uint32_t TX_SEQ[NUM_TX+1];
extern uint32_t *prepCfg1_TX_Seq;
extern BB_REGB_T *bb_regb_str;
extern STRUCT_BB_PREP bb_prep;
extern STRUCT_FFT2D_ZP fft2d_zp;
extern STRUCT_BB_CFAR_CFG0 cfarCfg0_st;

void BB_PrepConfig(STRUCT_BB_PREP *bb_prep_st);
void BB_FFT2D_ZP_Init(void);
void BB_FFT_ZP_Clear(void);
void BB_CFAR_CFG0(STRUCT_BB_CFAR_CFG0 *sCfarCfg0);
void BB_CFAR_Init(void);
void BB_CFAR_Vel_Init(void);


void BB_IRQ_Init(void);


void BB_waitISR(uint64_t val);
void BB_waitESR(uint64_t val);
void BB_WBUF_W16(uint32_t WBUF_Addr,uint16_t *Data,uint32_t len);
void BB_WBUF_W32(uint32_t WBUF_Addr,uint32_t *Data,uint32_t len);
void BB_WBUF_W64(uint32_t WBUF_Addr,uint64_t *Data,uint32_t len);
void BB_fix2pf(uint32_t *In,uint32_t *out,uint32_t num);
void BB_float2pf(uint32_t *In,uint32_t *out,uint32_t num);












extern uint8_t BB_ReadReg_M8(uint32_t sAddr);
void BB_WriteReg_M8(uint32_t sAddr,uint8_t sData);
extern uint16_t BB_ReadReg_M16(uint32_t sAddr);
void BB_WriteReg_M16(uint32_t sAddr,uint16_t sData);
extern uint32_t BB_ReadReg_M32(uint32_t sAddr);
void BB_WriteReg_M32(uint32_t sAddr,uint32_t sData);
extern uint64_t BB_ReadReg_M64(uint32_t sAddr);
void BB_WriteReg_M64(uint32_t sAddr,uint64_t sData);

void BB_PA_Loop(void);

void BB_FUNC_Handle(uint16_t sCmd);






















#endif

