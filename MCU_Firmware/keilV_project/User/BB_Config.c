#include "App_Def.h"
#include "BB_Config.h"
#include <stdio.h>
#include <string.h>
#include "RF_SPI_Config.h"

uint32_t TX_SEQ[NUM_TX+1]={0,1+(1<<BB_BIT4_POS),0+(1<<BB_BIT4_POS),2+(1<<BB_BIT4_POS),3+(1<<BB_BIT4_POS)};
uint32_t *prepCfg1_TX_Seq = (uint32_t *)(BB_REGB_BASE+0x00C);
STRUCT_BB_PREP bb_prep;
STRUCT_BB_CFAR_CFG0 cfarCfg0_st;
BB_REGB_T *bb_regb_str=(BB_REGB_T *)(BB_REGB);
STRUCT_FFT2D_ZP fft2d_zp;
/********************************************dataPrep*****************************************************/
void BB_PrepConfig(STRUCT_BB_PREP *bb_prep_st){
//    bb_prep_st->cfg0.tx_num = NUM_TX-1;
		bb_prep_st->cfg0.tx_num = 1-1;
    bb_prep_st->cfg0.rx_num = NUM_RX-1;
//    bb_prep_st->cfg0.chirp_num = NUM_CHIRP-1+1;
		bb_prep_st->cfg0.chirp_num = NUM_CHIRP1_TX-1+1;
    bb_prep_st->cfg0.samp_pt = NUMPT_LOG;
    bb_prep_st->cfg0.circ_en = PREP_CFG0_CIRC_DIS;
    bb_prep_st->cfg0.tim_mode = PREP_CFG0_TIME_MODE_RAMPSYNC ; //PREP_CFG0_TIME_MODE_RAMPSYNC
    bb_prep_st->cfg0.samp_edge = PREP_CFG0_SAMP_EDGE_N;
    bb_prep_st->cfg0.cnt_edge = PREP_CFG0_CNT_EDGE_P;
    bb_prep_st->cfg0.tim_rma = ADC_OFFSET_LEN;
    #ifdef DOWN_FAC_DIV2
    bb_prep_st->cfg0.cic_sec = PREP_CFG0_CIC_SEC1;
    bb_prep_st->cfg0.down_fac = PREP_CFG0_DOWN_FAC_2;
    #endif
    #ifdef DOWN_FAC_DIV1
    bb_prep_st->cfg0.cic_sec = PREP_CFG0_CIC_SEC1;
    bb_prep_st->cfg0.down_fac = PREP_CFG0_DOWN_FAC_2;
    #endif
    bb_prep_st->cfg0.max_removal_dis =     PREP_CFG0_MAX_REMOVAL_DIS_EN;
    #ifdef ADC_BIT8
    bb_prep_st->cfg0.adc_addr_mode = PREP_CFG0_ADC_ADDR_MODE_8BIT;
    #endif
    #ifdef ADC_BIT16
    bb_prep_st->cfg0.adc_addr_mode = PREP_CFG0_ADC_ADDR_MODE_16BIT;
    #endif
    bb_prep_st->cfg0.adc_dsize = PREP_CFG0_ADC_DSIZE_12BIT; //PREP_CFG0_ADC_DSIZE_14BIT
    bb_prep_st->cfg0.adc_clk_mode = PREP_CFG0_ADC_CLK_MODE_N;

    bb_prep_st->cfg1.tx_ant_mode = PREP_CFG1_TX_ANT_MODE_AUTO;//PREP_CFG1_TX_ANT_MODE_AUTO PREP_CFG1_TX_ANT_MODE_MANUAL
    bb_prep_st->cfg1.tx_ant_seq = 1; //0
    bb_prep_st->cfg1.rx_ant_seq7 = 7;
    bb_prep_st->cfg1.rx_ant_seq6 = 6;
    bb_prep_st->cfg1.rx_ant_seq5 = 5;
    bb_prep_st->cfg1.rx_ant_seq4 = 4;
//    bb_prep_st->cfg1.rx_ant_seq3 = 3;
//    bb_prep_st->cfg1.rx_ant_seq2 = 2;
//    bb_prep_st->cfg1.rx_ant_seq1 = 1;
//    bb_prep_st->cfg1.rx_ant_seq0 = 0;
    bb_prep_st->cfg1.rx_ant_seq3 = 0;
    bb_prep_st->cfg1.rx_ant_seq2 = 1;
    bb_prep_st->cfg1.rx_ant_seq1 = 2;
    bb_prep_st->cfg1.rx_ant_seq0 = 3;

    bb_prep_st->cfg2.interInc = ADC_MAX_LEN*ADC_BYTE;
//    bb_prep_st->cfg2.jumpInc = bb_prep_st->cfg2.interInc*NUM_ANT;
		bb_prep_st->cfg2.jumpInc = bb_prep_st->cfg2.interInc*NUM_RX;
//    bb_prep_st->cfg2.interInc = PREP_CFG2_JUMPINC_1M/NUM_CHIRP/NUM_ANT;
//    bb_prep_st->cfg2.jumpInc = PREP_CFG2_JUMPINC_1M/NUM_CHIRP;
    #ifdef COMPARE_DATA
            bb_prep_st->cfg3.prep_baddr = BB_DBUF0_BASE; //ADC_SAMPLE_ADDR BB_DBUF0_BASE
    #else
            bb_prep_st->cfg3.prep_baddr = ADC_SAMPLE_ADDR-NUM_RX*ADC_BYTE*ADC_MAX_LEN; //ADC_SAMPLE_ADDR BB_DBUF0_BASE
//		bb_prep_st->cfg3.prep_baddr = ADC_SAMPLE_ADDR; //ADC_SAMPLE_ADDR BB_DBUF0_BASE
    #endif

    /*write prep reg*/
    bb_regb_str->PREP_CFG0 = ((uint64_t)bb_prep_st->cfg0.adc_clk_mode    << BB_BIT0_POS) |
                           ((uint64_t)bb_prep_st->cfg0.adc_dsize       << BB_BIT1_POS) |
                                               ((uint64_t)bb_prep_st->cfg0.adc_addr_mode   << BB_BIT3_POS) |
                             ((uint64_t)bb_prep_st->cfg0.max_removal_dis << BB_BIT4_POS) |
                             ((uint64_t)bb_prep_st->cfg0.down_fac        << BB_BIT8_POS) |
                             ((uint64_t)bb_prep_st->cfg0.cic_sec         << BB_BIT12_POS)|
                             ((uint64_t)bb_prep_st->cfg0.tim_rma         << BB_BIT16_POS)|
                             ((uint64_t)bb_prep_st->cfg0.cnt_edge        << BB_BIT26_POS)|
                             ((uint64_t)bb_prep_st->cfg0.samp_edge       << BB_BIT27_POS)|
                             ((uint64_t)bb_prep_st->cfg0.tim_mode        << BB_BIT28_POS)|
                                                     ((uint64_t)bb_prep_st->cfg0.circ_en         << BB_BIT30_POS)|
                             ((uint64_t)bb_prep_st->cfg0.samp_pt         << BB_BIT40_POS)|
                             ((uint64_t)bb_prep_st->cfg0.chirp_num       << BB_BIT44_POS)|
                             ((uint64_t)bb_prep_st->cfg0.rx_num          << BB_BIT56_POS)|
                             ((uint64_t)bb_prep_st->cfg0.tx_num          << BB_BIT60_POS);
    bb_regb_str->PREP_CFG1 = ((uint64_t)bb_prep_st->cfg1.rx_ant_seq0 << BB_BIT0_POS) |
                             ((uint64_t)bb_prep_st->cfg1.rx_ant_seq1 << BB_BIT4_POS) |
                             ((uint64_t)bb_prep_st->cfg1.rx_ant_seq2 << BB_BIT8_POS) |
                             ((uint64_t)bb_prep_st->cfg1.rx_ant_seq3 << BB_BIT12_POS)|
                             ((uint64_t)bb_prep_st->cfg1.rx_ant_seq4 << BB_BIT16_POS)|
                             ((uint64_t)bb_prep_st->cfg1.rx_ant_seq5 << BB_BIT20_POS)|
                             ((uint64_t)bb_prep_st->cfg1.rx_ant_seq6 << BB_BIT24_POS)|
                             ((uint64_t)bb_prep_st->cfg1.rx_ant_seq7 << BB_BIT28_POS)|
                                                     ((uint64_t)bb_prep_st->cfg1.tx_ant_seq  << BB_BIT32_POS)|
                             ((uint64_t)bb_prep_st->cfg1.tx_ant_mode << BB_BIT36_POS);

    bb_regb_str->PREP_CFG2 = ((uint64_t)bb_prep_st->cfg2.interInc << BB_BIT0_POS) |
                             ((uint64_t)bb_prep_st->cfg2.jumpInc  << BB_BIT32_POS) ;
    #ifdef COMPARE_DATA
        bb_regb_str->PREP_CFG2 = 0;
    #endif
    bb_regb_str->PREP_CFG3 = (uint64_t)bb_prep_st->cfg3.prep_baddr << BB_BIT0_POS;
}

void BB_FFT1D_ZP_Init(void){ //range

}
void BB_FFT2D_ZP_Init(void){ //chirp
    uint32_t sCmd=NUM_CHIRP*NUM_TX;
    switch(sCmd){
        case 32:
            break;
        case 64:
            for(uint32_t txNow=0;txNow<NUM_TX;txNow++){
                fft2d_zp.txNum_zp0[txNow] = 0;
                for(uint32_t chirpNow=0;chirpNow<NUM_CHIRP;chirpNow++){
                    fft2d_zp.txNum_zp0[txNow] = fft2d_zp.txNum_zp0[txNow]|(uint64_t)(1<<txNow)<<(chirpNow*NUM_TX);
                }
            }
            break;
        case 256:
            for(uint32_t txNow=0;txNow<NUM_TX;txNow++){
                fft2d_zp.txNum_zp0[txNow] = 0;
                fft2d_zp.txNum_zp1[txNow] = 0;
                fft2d_zp.txNum_zp2[txNow] = 0;
                fft2d_zp.txNum_zp3[txNow] = 0;
                for(uint32_t chirpNow=0;chirpNow<NUM_CHIRP/2;chirpNow++){
                    fft2d_zp.txNum_zp0[txNow] = fft2d_zp.txNum_zp0[txNow]|(uint64_t)(1<<txNow)<<(chirpNow*NUM_TX);
                    fft2d_zp.txNum_zp1[txNow] = fft2d_zp.txNum_zp1[txNow]|(uint64_t)(1<<txNow)<<(chirpNow*NUM_TX);
                    fft2d_zp.txNum_zp2[txNow] = fft2d_zp.txNum_zp2[txNow]|(uint64_t)(1<<txNow)<<(chirpNow*NUM_TX);
                    fft2d_zp.txNum_zp3[txNow] = fft2d_zp.txNum_zp3[txNow]|(uint64_t)(1<<txNow)<<(chirpNow*NUM_TX);
                }
            }
            break;
    }
}
void BB_FFT_ZP_Clear(void){
    bb_regb_str->FFT_SRC_ZP00=0xFFFFFFFFFFFFFFFFULL;
    bb_regb_str->FFT_SRC_ZP01=0xFFFFFFFFFFFFFFFFULL;
    bb_regb_str->FFT_SRC_ZP02=0xFFFFFFFFFFFFFFFFULL;
    bb_regb_str->FFT_SRC_ZP03=0xFFFFFFFFFFFFFFFFULL;
    bb_regb_str->FFT_SRC_ZP04=0xFFFFFFFFFFFFFFFFULL;
    bb_regb_str->FFT_SRC_ZP05=0xFFFFFFFFFFFFFFFFULL;
    bb_regb_str->FFT_SRC_ZP06=0xFFFFFFFFFFFFFFFFULL;
    bb_regb_str->FFT_SRC_ZP07=0xFFFFFFFFFFFFFFFFULL;
}
/**************************************************P2****************************************************************/

/**************************************************CFAR****************************************************************/
void BB_CFAR_CFG0(STRUCT_BB_CFAR_CFG0 *sCfarCfg0){
    bb_regb_str->CFAR_CFG0 = (uint64_t)sCfarCfg0->wrapDirA  <<BB_BIT61_POS|
                             (uint64_t)sCfarCfg0->wrapDirB  <<BB_BIT60_POS|
                                                     (uint64_t)sCfarCfg0->sumMode   <<BB_BIT56_POS|
                                                     (uint64_t)sCfarCfg0->interCnt  <<BB_BIT44_POS|
                                                     (uint64_t)sCfarCfg0->intraCnt  <<BB_BIT32_POS|
                                                     (uint64_t)sCfarCfg0->thresDiv  <<BB_BIT24_POS|
                                                     (uint64_t)sCfarCfg0->divFac    <<BB_BIT20_POS|
                                                     (uint64_t)sCfarCfg0->mulFac    <<BB_BIT12_POS|
                                                     (uint64_t)sCfarCfg0->searchSize<<BB_BIT8_POS|
                                                     (uint64_t)sCfarCfg0->guardSize <<BB_BIT4_POS|
                                                     (uint64_t)sCfarCfg0->pdEn      <<BB_BIT2_POS|
                                                     (uint64_t)sCfarCfg0->cfarMode  <<BB_BIT0_POS;
}

void BB_CFAR_Init(void){
        cfarCfg0_st.wrapDirA = CFAR_CFG0_WRAPDIR_INC;
        cfarCfg0_st.wrapDirB = CFAR_CFG0_WRAPDIR_INC;
        cfarCfg0_st.sumMode = CFAR_CFG0_SUMMODE_ABS;
        cfarCfg0_st.interCnt = USE_RANGE;
        cfarCfg0_st.intraCnt = NUM_VEL;
        cfarCfg0_st.thresDiv = CFAR_CFG0_THRESDIV4;           // = searchSize num
        cfarCfg0_st.divFac = CFAR_CFG0_DIVFAC1;
        cfarCfg0_st.mulFac = r2_str.cfarThV;
        cfarCfg0_st.searchSize = r2_str.cfarThVS;
        cfarCfg0_st.guardSize = r2_str.cfarThVG;
        cfarCfg0_st.totalSize = cfarCfg0_st.searchSize+cfarCfg0_st.guardSize;
        cfarCfg0_st.pdEn = CFAR_CFG0_PEAKDET_DIS; //CFAR_CFG0_PEAKDET_DIS CFAR_CFG0_PEAKDET_EN
        cfarCfg0_st.cfarMode = CFAR_CFG0_CFARMODE_SO;
        BB_CFAR_CFG0(&cfarCfg0_st); //STRUCT_CFAR_CFG0
        BB_CFAR_CFG1(CFAR_MAX_NUM,NUM_BYTE32); //interInc
        BB_CFAR_CFG2(NUM_BIG_CHIRP_BYTES,FFT2D_CACHE_ADDR+USE_RANGE*CFAR_ANT*NUM_BYTE32);//intraInc,intraAddr
        BB_CFAR_CFG3((FFT2D_CACHE_ADDR+USE_RANGE*CFAR_ANT*NUM_BYTE32+(NUM_VEL-cfarCfg0_st.totalSize)*NUM_BIG_CHIRP_BYTES),FFT2D_CACHE_ADDR+USE_RANGE*CFAR_ANT*NUM_BYTE32);//wrapAddrA,wrapAddrB
        BB_CFAR_CFG4(CFAR_CACHE_ADDR);//dstBaseAddr
}

void BB_CFAR_Vel_Init(void){
        cfarCfg0_st.wrapDirA = CFAR_CFG0_WRAPDIR_INC;
        cfarCfg0_st.wrapDirB = CFAR_CFG0_WRAPDIR_INC;
        cfarCfg0_st.sumMode = CFAR_CFG0_SUMMODE_ABS;
        cfarCfg0_st.interCnt = USE_RANGE;
        cfarCfg0_st.intraCnt = NUM_VEL;
        cfarCfg0_st.thresDiv = CFAR_CFG0_THRESDIV4;           // = searchSize num
        cfarCfg0_st.divFac = CFAR_CFG0_DIVFAC1;
        cfarCfg0_st.mulFac = r2_str.cfarThV;
        cfarCfg0_st.searchSize = r2_str.cfarThVS;
        cfarCfg0_st.guardSize = r2_str.cfarThVG;
        cfarCfg0_st.totalSize = cfarCfg0_st.searchSize+cfarCfg0_st.guardSize;
        cfarCfg0_st.pdEn = 0; //CFAR_CFG0_PEAKDET_DIS CFAR_CFG0_PEAKDET_EN
        cfarCfg0_st.cfarMode = CFAR_CFG0_CFARMODE_SO;
        BB_CFAR_CFG0(&cfarCfg0_st); //STRUCT_CFAR_CFG0
        BB_CFAR_CFG1(CFAR_MAX_NUM,NUM_BYTE32); //interInc
        BB_CFAR_CFG2(USE_RANGE * NUM_BYTE32, BB_DBUF0_BASE);//intraInc,intraAddr
        BB_CFAR_CFG3((BB_DBUF0_BASE+(NUM_VEL-cfarCfg0_st.totalSize)*USE_RANGE * NUM_BYTE32),BB_DBUF0_BASE);//wrapAddrA,wrapAddrB
        BB_CFAR_CFG4(CFAR_CACHE_ADDR); //dstBaseAddr
}
/**************************************************DPK****************************************************************/

/**************************************************IRQ****************************************************************/
void BB_IRQ_Init(void){
    NVIC_EnableIRQ(BB_IRQn);
//    BB_IMR_Init(BB_IMR_PREP_RAMP_END);
    BB_IMR_Init(BB_IMR_PREP_RAMP_END|BB_IMR_RAMP_PEDGE);
}





void BB_waitISR(uint64_t val){
    while( (bb_regb_str->ISR & val)==0 );
        bb_regb_str->ISR = val;
}
void BB_waitESR(uint64_t val){
    while( (bb_regb_str->ESR & val)==0 );
      bb_regb_str->ESR     = val; //all clear
}
void BB_WBUF_W16(uint32_t WBUF_Addr,uint16_t *Data,uint32_t len){
    for(uint32_t i=0; i<len; i++){
        Write_M16(WBUF_Addr+2*i, Data[i]);
    }
}
void BB_WBUF_W32(uint32_t WBUF_Addr,uint32_t *Data,uint32_t len){
    for(uint32_t i=0; i<len; i++){
        Write_M32(WBUF_Addr+4*i, Data[i]);
    }
}
void BB_WBUF_W64(uint32_t WBUF_Addr,uint64_t *Data,uint32_t len){
    for(uint32_t i=0; i<len; i++){
        Write_M64(WBUF_Addr+8*i, Data[i]);
    }
}

void BB_CpxFix2pf(uint32_t *In,uint32_t *out,uint32_t intraCnt){
    if(intraCnt<=4096){
        BB_OPCLEAR(BB_CLEAR_DMA);

        BB_DMA_Cfg0(DMA_CFG0_TWICEMODE_NORMAL,DMA_CFG0_MINMAX_MODE_ABS,DMA_CFG0_FLT2PF_LSF_EN_RIGHT,0,0,0,intraCnt-1,DMA_CFG0_WMODE_FIX2PF,DMA_CFG0_DSIZE_64BIT);
        BB_DMA_Cfg1(0,0);//srcJumpInc,srcInterInc
        BB_DMA_Cfg2(NUM_BYTE64,&In[0]);//srcIntraInc,srcBaseAddr
        BB_DMA_Cfg3(0,0);//dstJumpInc,dstInterInc
        BB_DMA_Cfg4(NUM_BYTE32,&out[0]); //dstIntraInc,dstBaseAddr
        BB_OPGATE_EN(BB_GATE_DMA);
        BB_ISR_CLEAR(BB_ISR_DMA_JUMP_END);
        BB_OPTRIG(BB_TRIG_DMA);
        BB_waitISR(BB_ISR_DMA_JUMP_END);
        BB_OPGATE_DIS(BB_GATE_DMA);
    }
}

void BB_CpxFlt2pf_short(uint32_t *In,uint32_t *out,uint32_t intraCnt){
    if(intraCnt<=4096)
    {
        BB_DMA_Cfg0(DMA_CFG0_TWICEMODE_NORMAL,DMA_CFG0_MINMAX_MODE_ABS,DMA_CFG0_FLT2PF_LSF_EN_RIGHT,0,0,0,intraCnt-1,DMA_CFG0_WMODE_F2PF,DMA_CFG0_DSIZE_64BIT);
        BB_DMA_Cfg1(0,0);//srcJumpInc,srcInterInc
        BB_DMA_Cfg2(NUM_BYTE64,&In[0]);//srcIntraInc,srcBaseAddr
        BB_DMA_Cfg3(0,0);//dstJumpInc,dstInterInc
        BB_DMA_Cfg4(NUM_BYTE32,&out[0]); //dstIntraInc,dstBaseAddr
        BB_OPCLEAR(BB_CLEAR_DMA);
        BB_OPGATE_EN(BB_GATE_DMA);
        BB_ISR_CLEAR(BB_ISR_DMA_JUMP_END);
        BB_OPTRIG(BB_TRIG_DMA);
        BB_waitISR(BB_ISR_DMA_JUMP_END);
        BB_OPGATE_DIS(BB_GATE_DMA);
    }
}

void BB_CpxFlt2pf_long(uint32_t *In,uint32_t *out,uint32_t intraCnt,uint32_t interCnt){
    BB_DMA_Cfg0(DMA_CFG0_TWICEMODE_NORMAL,DMA_CFG0_MINMAX_MODE_ABS,DMA_CFG0_FLT2PF_LSF_EN_RIGHT,0,0,interCnt-1,intraCnt-1,DMA_CFG0_WMODE_F2PF,DMA_CFG0_DSIZE_64BIT);
    BB_DMA_Cfg1(0,intraCnt*NUM_BYTE64);//srcJumpInc,srcInterInc
    BB_DMA_Cfg2(NUM_BYTE64,&In[0]);//srcIntraInc,srcBaseAddr
    BB_DMA_Cfg3(0,intraCnt*NUM_BYTE32);//dstJumpInc,dstInterInc
    BB_DMA_Cfg4(NUM_BYTE32,&out[0]); //dstIntraInc,dstBaseAddr
    BB_OPCLEAR(BB_CLEAR_DMA);
    BB_OPGATE_EN(BB_GATE_DMA);
    BB_ISR_CLEAR(BB_ISR_DMA_JUMP_END);
    BB_OPTRIG(BB_TRIG_DMA);
    BB_waitISR(BB_ISR_DMA_JUMP_END);
    BB_OPGATE_DIS(BB_GATE_DMA);
}














uint16_t flt_max_search(uint16_t len,float *din){
    uint16_t maxIdx = 0;
    float maxVal=din[0];
    for (int i=1;i<len;i++){
        if(din[i]>maxVal){
            maxIdx=i;
            maxVal=din[i];
        }
    }
    return maxIdx;
}







