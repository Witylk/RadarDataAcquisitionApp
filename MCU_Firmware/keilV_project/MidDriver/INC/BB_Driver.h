#ifndef __BB_DRIVER_H
#define __BB_DRIVER_H

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include "core_cm7.h"
/**********************************BIT Pos**********************************/
#define BB_BIT63_POS                           (63ULL)
#define BB_BIT62_POS                           (62ULL)
#define BB_BIT61_POS                           (61ULL)
#define BB_BIT60_POS                           (60ULL)
#define BB_BIT59_POS                           (59ULL)
#define BB_BIT58_POS                           (58ULL)
#define BB_BIT57_POS                           (57ULL)
#define BB_BIT56_POS                           (56ULL)
#define BB_BIT55_POS                           (55ULL)
#define BB_BIT54_POS                           (54ULL)
#define BB_BIT53_POS                           (53ULL)
#define BB_BIT52_POS                           (52ULL)
#define BB_BIT51_POS                           (51ULL)
#define BB_BIT50_POS                           (50ULL)
#define BB_BIT49_POS                           (49ULL)
#define BB_BIT48_POS                           (48ULL)
#define BB_BIT47_POS                           (47ULL)
#define BB_BIT46_POS                           (46ULL)
#define BB_BIT45_POS                           (45ULL)
#define BB_BIT44_POS                           (44ULL)
#define BB_BIT43_POS                           (43ULL)
#define BB_BIT42_POS                           (42ULL)
#define BB_BIT41_POS                           (41ULL)
#define BB_BIT40_POS                           (40ULL)
#define BB_BIT39_POS                           (39ULL)
#define BB_BIT38_POS                           (38ULL)
#define BB_BIT37_POS                           (37ULL)
#define BB_BIT36_POS                           (36ULL)
#define BB_BIT35_POS                           (35ULL)
#define BB_BIT34_POS                           (34ULL)
#define BB_BIT33_POS                           (33ULL)
#define BB_BIT32_POS                           (32ULL)
#define BB_BIT31_POS                           (31ULL)
#define BB_BIT30_POS                           (30ULL)
#define BB_BIT29_POS                           (29ULL)
#define BB_BIT28_POS                           (28ULL)
#define BB_BIT27_POS                           (27ULL)
#define BB_BIT26_POS                           (26ULL)
#define BB_BIT25_POS                           (25ULL)
#define BB_BIT24_POS                           (24ULL)
#define BB_BIT23_POS                           (23ULL)
#define BB_BIT22_POS                           (22ULL)
#define BB_BIT21_POS                           (21ULL)
#define BB_BIT20_POS                           (20ULL)
#define BB_BIT19_POS                           (19ULL)
#define BB_BIT18_POS                           (18ULL)
#define BB_BIT17_POS                           (17ULL)
#define BB_BIT16_POS                           (16ULL)
#define BB_BIT15_POS                           (15ULL)
#define BB_BIT14_POS                           (14ULL)
#define BB_BIT13_POS                           (13ULL)
#define BB_BIT12_POS                           (12ULL)
#define BB_BIT11_POS                           (11ULL)
#define BB_BIT10_POS                           (10ULL)
#define BB_BIT9_POS                            (9ULL)
#define BB_BIT8_POS                            (8ULL)
#define BB_BIT7_POS                            (7ULL)
#define BB_BIT6_POS                            (6ULL)
#define BB_BIT5_POS                            (5ULL)
#define BB_BIT4_POS                            (4ULL)
#define BB_BIT3_POS                            (3ULL)
#define BB_BIT2_POS                            (2ULL)
#define BB_BIT1_POS                            (1ULL)
#define BB_BIT0_POS                            (0ULL)

/*********************************PREP*******************************************/
#define PREP_CFG0_ADC_CLK_MODE_P              (1)
#define PREP_CFG0_ADC_CLK_MODE_N              (0)

#define PREP_CFG0_ADC_DSIZE_8BIT              (0)
#define PREP_CFG0_ADC_DSIZE_10BIT             (1)
#define PREP_CFG0_ADC_DSIZE_12BIT             (2)
#define PREP_CFG0_ADC_DSIZE_14BIT             (3)

#define PREP_CFG0_ADC_ADDR_MODE_8BIT          (0)
#define PREP_CFG0_ADC_ADDR_MODE_16BIT         (1)

#define PREP_CFG0_MAX_REMOVAL_DIS_EN              (0)
#define PREP_CFG0_MAX_REMOVAL_DIS_DIS         (1)

#define PREP_CFG0_DOWN_FAC_1                          (0)
#define PREP_CFG0_DOWN_FAC_2                           (1)
#define PREP_CFG0_DOWN_FAC_3                          (2)
#define PREP_CFG0_DOWN_FAC_4                           (3)
#define PREP_CFG0_DOWN_FAC_5                          (4)
#define PREP_CFG0_DOWN_FAC_6                           (5)
#define PREP_CFG0_DOWN_FAC_7                          (6)
#define PREP_CFG0_DOWN_FAC_8                           (7)

#define PREP_CFG0_CIC_SEC0                             (0)
#define PREP_CFG0_CIC_SEC1                             (1)
#define PREP_CFG0_CIC_SEC2                             (2)
#define PREP_CFG0_CIC_SEC3                             (3)


#define PREP_CFG0_CNT_EDGE_N                  (0)
#define PREP_CFG0_CNT_EDGE_P                  (1)

#define PREP_CFG0_SAMP_EDGE_N                 (0)
#define PREP_CFG0_SAMP_EDGE_P                 (1)

#define PREP_CFG0_CIRC_EN                     (1)
#define PREP_CFG0_CIRC_DIS                    (0)
#define PREP_CFG0_TIME_MODE_DATA                    (0)
#define PREP_CFG0_TIME_MODE_RAMPSYNC            (1)
#define PREP_CFG0_TIME_MODE_RAW                    (2)

#define PREP_CFG1_TX_ANT_MODE_AUTO            (0)
#define PREP_CFG1_TX_ANT_MODE_MANUAL          (1)


typedef struct{
    uint8_t tx_num;
    uint8_t rx_num;
    uint16_t chirp_num;
    uint8_t samp_pt;
    uint8_t circ_en;
    uint8_t tim_mode;
    uint8_t samp_edge;
    uint8_t cnt_edge;
    uint16_t tim_rma;
    uint8_t cic_sec;
    uint8_t down_fac;
    uint8_t max_removal_dis;
    uint8_t adc_addr_mode;
    uint8_t adc_dsize;
    uint8_t adc_clk_mode;
}PREP_CFG0;
typedef struct{
    uint8_t tx_ant_mode;
    uint8_t tx_ant_seq;
    uint8_t rx_ant_seq7;
    uint8_t rx_ant_seq6;
    uint8_t rx_ant_seq5;
    uint8_t rx_ant_seq4;
    uint8_t rx_ant_seq3;
    uint8_t rx_ant_seq2;
    uint8_t rx_ant_seq1;
    uint8_t rx_ant_seq0;
}PREP_CFG1;
typedef struct{
    uint32_t jumpInc;
    uint32_t interInc;
}PREP_CFG2;
typedef struct{
    uint32_t prep_baddr;
}PREP_CFG3;
typedef struct
{
    PREP_CFG0 cfg0;
    PREP_CFG1 cfg1;
    PREP_CFG2 cfg2;
    PREP_CFG3 cfg3;
}STRUCT_BB_PREP;
/********************************* ZO *******************************************/
#define BB_ZO_ZLEN_3                           (0) //insert 3
#define BB_ZO_ZLEN_4                           (1) //insert 4
#define BB_ZO_ZLEN_5                           (2) //insert 5
#define BB_ZO_ZLEN_6                           (3) //insert 6
#define BB_ZO_MODE_1                           (1)
#define BB_ZO_MODE_2                           (2)
#define BB_ZO_MODE_3                           (3)

#define BB_ZO_Cfg0(zLen,mulFac,mode,thresSum)                                                                  (bb_regb_str->ZO_CFG0 = \
                                                                                                                                                                                     (uint64_t)(zLen)<<BB_BIT24_POS|\
                                                                                               (uint64_t)(mulFac)<<BB_BIT20_POS|\
                                                                                                                                                                                             (uint64_t)(mode)<<BB_BIT16_POS|\
                                                                                                                                                                                             (uint64_t)(thresSum)<<BB_BIT0_POS)
#define BB_ZO_THRES_SUM_GET                     ((uint32_t)bb_regb_str->ZO_CFG0&0xFFFF)
#define BB_ZO_STA0_absSum                                        (bb_regb_str->ZO_STA0&(uint64_t)0xFFFFFFF)
/*********************************DMA*******************************************/
#define DMA_CFG0_TWICEMODE_NORMAL              (0)
#define DMA_CFG0_TWICEMODE_TIWCE               (1)
#define DMA_CFG0_MINMAX_MODE_FIX               (1)
#define DMA_CFG0_MINMAX_MODE_ABS               (0)
#define DMA_CFG0_FLT2PF_LSF_EN_LEFT            (1)
#define DMA_CFG0_FLT2PF_LSF_EN_RIGHT           (0)

#define DMA_CFG0_WMODE_ORI                     (0)
#define DMA_CFG0_WMODE_PF2CONJ                 (1)
#define DMA_CFG0_WMODE_FIX2PF                  (4)
#define DMA_CFG0_WMODE_F2PF                    (5)
#define DMA_CFG0_DSIZE_8BIT                    (0)
#define DMA_CFG0_DSIZE_16BIT                                     (1)
#define DMA_CFG0_DSIZE_32BIT                                     (2)
#define DMA_CFG0_DSIZE_64BIT                                     (3)

#define BB_DMA_Cfg0(twiceMode,minmaxMode,flt2pf_lsf_en,flt2pf_sfb,jumpCnt,interCnt,intraCnt,wmode,dsize) (bb_regb_str->DMA_CFG0 = \
                                                                                                                                                                                             (uint64_t)(twiceMode)<<BB_BIT61_POS|\
                                                                                                                                                                                     (uint64_t)(minmaxMode)<<BB_BIT60_POS|\
                                                                                               (uint64_t)(flt2pf_lsf_en)<<BB_BIT59_POS|\
                                                                                                                                                                                             (uint64_t)(flt2pf_sfb)<<BB_BIT52_POS|\
                                                                                               (uint64_t)(jumpCnt)<<BB_BIT32_POS|\
                                                                                               (uint64_t)(interCnt)<<BB_BIT20_POS|\
                                                                                               (uint64_t)(intraCnt)<<BB_BIT8_POS|\
                                                                                               (uint64_t)(wmode)<<BB_BIT4_POS|\
                                                                                               (uint64_t)(dsize)<<BB_BIT0_POS)
#define BB_DMA_Cfg1(srcJumpInc,srcInterInc)       (bb_regb_str->DMA_CFG1    = (uint64_t)(srcJumpInc )<<BB_BIT32_POS | (uint64_t)(srcInterInc)<<BB_BIT0_POS  )
#define BB_DMA_Cfg2(srcIntraInc,srcBaseAddr)      (bb_regb_str->DMA_CFG2    = (uint64_t)(srcIntraInc)<<BB_BIT32_POS |(uint64_t)(srcBaseAddr)<<BB_BIT0_POS  )
#define BB_DMA_Cfg3(dstJumpInc,dstInterInc)       (bb_regb_str->DMA_CFG3    = (uint64_t)(dstJumpInc )<<BB_BIT32_POS | (uint64_t)(dstInterInc)<<BB_BIT0_POS  )
#define BB_DMA_Cfg4(dstIntraInc,dstBaseAddr)      (bb_regb_str->DMA_CFG4    = (uint64_t)(dstIntraInc)<<BB_BIT32_POS |(uint64_t)(dstBaseAddr)<<BB_BIT0_POS  )
#define BB_DMA_STA0_totalMaxVal                   (bb_regb_str->DMA_STA0>>BB_BIT32_POS)
#define BB_DMA_STA0_totalMinVal                                        (bb_regb_str->DMA_STA0&(uint64_t)0xFFFFFFFF)
#define BB_DMA_STA1_maxJumpIdx                    (bb_regb_str->DMA_STA1>>BB_BIT32_POS)
#define BB_DMA_STA1_maxInterIdx                                        ((bb_regb_str->DMA_STA1>>BB_BIT12_POS)&(uint64_t)0xFFF)
#define BB_DMA_STA1_maxIntraIdx                                        (bb_regb_str->DMA_STA1&(uint64_t)0xFFF)
#define BB_DMA_STA2_minJumpIdx                    (bb_regb_str->DMA_STA2>>BB_BIT32_POS)
#define BB_DMA_STA2_minInterIdx                                        ((bb_regb_str->DMA_STA2>>BB_BIT12_POS)&(uint64_t)0xFFF)
#define BB_DMA_STA2_minIntraIdx                                        (bb_regb_str->DMA_STA2&(uint64_t)0xFFF)

/*********************************P1MUL*******************************************/
#define P1MUL_CFG0_NCOMODE_COS                  (1)
#define P1MUL_CFG0_NCOMODE_SIN                  (2)
#define P1MUL_CFG0_NCOMODE_COS_SIN              (3)
#define BB_P1MUL_Cfg0(ncoMode,nco,ap)                           (bb_regb_str->P1MUL_CFG0  = (uint64_t)(ncoMode) <<BB_BIT56_POS|((uint64_t)(nco) << BB_BIT32_POS) | (ap))
#define BB_P1MUL_Cfg1(dstIntraInc,dstBaseAddr)                  (bb_regb_str->P1MUL_CFG1  = ((uint64_t)(dstIntraInc) << BB_BIT32_POS) | (dstBaseAddr))
/*********************************FFT*******************************************/
#define FFTPT_16                                          (0)
#define FFTPT_32                                          (1)
#define FFTPT_64                                          (2)
#define FFTPT_128                                         (3)
#define FFTPT_256                                         (4)
#define FFTPT_512                                         (5)
#define FFTPT_1024                                        (6)
#define FFTPT_2048                                        (7)
#define FFTPT_4096                                        (8)

#define FFT_UNLOAD_EN                                     (1)
#define FFT_UNLOAD_DIS                                    (0)

#define FFT_MODE_FORWARD                                  (0)
#define FFT_MODE_INVERSE                                  (1)

#define FFT_CFG1_OUTLSFEN_LEFT                 (1)
#define FFT_CFG1_OUTLSFEN_RIGHT                (0)

#define FFT_CFG1_INLSFEN_LEFT                  (1)
#define FFT_CFG1_INLSFEN_RIGHT                 (0)

#define FFT_CFG1_WIN_SIZE_14BIT                    (0)
#define FFT_CFG1_WIN_SIZE_32BIT                    (1)

#define FFT_CFG1_WIN_EN                            (1)
#define FFT_CFG1_WIN_DIS                           (0)

#define FFT_CMD0_CLEAR_IBUF                              (0x15)
#define FFT_CMD0_CLEAR_STA1                              (0x16)



#define BB_FFT_CMD0(sCmd)                      (bb_regb_str->FFT_CMD0 = (uint64_t)(sCmd))
#define BB_FFT_Cfg0(rsfOutput,rfInput,useA,useB,inPt,fftPt,unloadEn,mode)           (bb_regb_str->FFT_CFG0=(uint64_t)(rsfOutput)<<BB_BIT58_POS|(uint64_t)(rfInput)<<BB_BIT56_POS|(uint64_t)(useA)<<BB_BIT44_POS|(uint64_t)(useB)<<BB_BIT32_POS|(uint64_t)(inPt)<<BB_BIT12_POS|(uint64_t)(fftPt)<<BB_BIT8_POS|(uint64_t)(unloadEn)<<BB_BIT4_POS|(uint64_t)(mode)<<BB_BIT0_POS)
#define BB_FFT_Cfg1(winBaseAddr,outLsfEn,outSfb,inLsfEn,inSfb,winSize,winEn)        (bb_regb_str->FFT_CFG1 = \
                                                                                    (uint64_t)(winBaseAddr) << BB_BIT32_POS |\
                                                                                    (uint64_t)(outLsfEn)    << BB_BIT19_POS |\
                                                                                                                                                                        (uint64_t)(outSfb)      << BB_BIT12_POS |\
                                                                                                                                                                        (uint64_t)(inLsfEn)     << BB_BIT11_POS |\
                                                                                                                                                                        (uint64_t)(inSfb)       << BB_BIT4_POS  |\
                                                                                                                                                                        (uint64_t)(winSize)     << BB_BIT1_POS  |\
                                                                                                                                                                        (uint64_t)(winEn)       << BB_BIT0_POS )
#define BB_FFT_Cfg2(dstIntraInc,dstBaseAddr)                                        (bb_regb_str->FFT_CFG2    =  (uint64_t)(dstIntraInc) << BB_BIT32_POS|(uint64_t)(dstBaseAddr) << BB_BIT0_POS )
#define BB_FFT_STA0_curMaxVal                                                       (bb_regb_str->FFT_STA0>>BB_BIT32_POS)
#define BB_FFT_STA0_curMaxIdx                                                                            (bb_regb_str->FFT_STA0&(uint64_t)0xFFF)
/*********************************P2*******************************************/
#define P2_CFG0_DIV1                           (0)
#define P2_CFG0_DIV2                           (1)
#define P2_CFG0_DIV4                           (2)
#define P2_CFG0_DIV8                           (3)
#define P2_CFG0_DIV16                          (4)
#define P2_CFG0_DIV32                          (5)
#define P2_CFG0_DIV64                          (6)
#define P2_CFG0_DIV128                         (7)
#define P2_CFG0_SUBMODE_CPX_SUM_MUL                         (0)
#define P2_CFG0_SUBMODE_ABS_SUM                (1)
#define P2_CFG0_SUBMODE_CPX_MINUS              (2)
#define P2_CFG0_MODE_ACC                                  (0)
#define P2_CFG0_MODE_ADD                                  (1)
#define P2_CFG0_MODE_MUL                                  (2)
#define P2_CFG0_MODE_MAC                                  (3)
#define BB_P2_CFG0(jumpCnt,interCnt,intraCnt,rsfBit,sub_mode,mode)     (bb_regb_str->P2_CFG0 = (uint64_t)(jumpCnt)<<BB_BIT44_POS |\
                                                                       (uint64_t)(interCnt) << BB_BIT32_POS |\
                                                                                                                                             (uint64_t)(intraCnt) << BB_BIT16_POS |\
                                                                                                                                             (uint64_t)(rsfBit) << BB_BIT8_POS |\
                                                                                                                                             (uint64_t)(sub_mode) << BB_BIT4_POS |\
                                                                                                                                             (uint64_t)(mode) << BB_BIT0_POS )
#define BB_P2_CFG1(src0JumpInc,src0InterInc)                           (bb_regb_str->P2_CFG1 = (uint64_t)(src0JumpInc) << BB_BIT32_POS |(uint64_t)(src0InterInc) << BB_BIT0_POS)
#define BB_P2_CFG2(src0IntraInc,src0BaseAddr)                          (bb_regb_str->P2_CFG2 = (uint64_t)(src0IntraInc) << BB_BIT32_POS |(uint64_t)(src0BaseAddr) << BB_BIT0_POS)
#define BB_P2_CFG3(src1JumpInc,src1InterInc)                           (bb_regb_str->P2_CFG3 = (uint64_t)(src1JumpInc)  << BB_BIT32_POS |(uint64_t)(src1InterInc) << BB_BIT0_POS)
#define BB_P2_CFG4(src1IntraInc,src1BaseAddr)                          (bb_regb_str->P2_CFG4 = (uint64_t)(src1IntraInc)  << BB_BIT32_POS |(uint64_t)(src1BaseAddr) << BB_BIT0_POS)
#define BB_P2_CFG5(dstJumpInc,dstInterInc)                             (bb_regb_str->P2_CFG5 = (uint64_t)(dstJumpInc)  << BB_BIT32_POS |(uint64_t)(dstInterInc) << BB_BIT0_POS)
#define BB_P2_CFG6(dstIntraInc,dstBaseAddr)                            (bb_regb_str->P2_CFG6 = (uint64_t)(dstIntraInc)  << BB_BIT32_POS |(uint64_t)(dstBaseAddr) << BB_BIT0_POS)

#define P2_CFG7_SRC0CONJ_EN                     (1)
#define P2_CFG7_SRC0CONJ_DIS                    (0)
#define P2_CFG7_SRC0LSF_LEFT                    (1)
#define P2_CFG7_SRC0LSF_RIGHT                   (0)
#define P2_CFG7_SRC1CONJ_EN                     (1)
#define P2_CFG7_SRC1CONJ_DIS                    (0)
#define P2_CFG7_SRC1LSF_LEFT                    (1)
#define P2_CFG7_SRC1LSF_RIGHT                   (0)
#define P2_CFG7_DSTCONJ_EN                      (1)
#define P2_CFG7_DSTCONJ_DIS                     (0)
#define P2_CFG7_DSTLSF_LEFT                     (1)
#define P2_CFG7_DSTLSF_RIGHT                    (0)

#define BB_P2_CFG7(src0ConjEn,src0LsfEn,src0Sfb,src1ConjEn,src1LsfEn,src1Sfb,dstConjEn,dstLsfEn,dstSfb) (bb_regb_str->P2_CFG7 = \
                                                                                                                                                                                                                (uint64_t)(src0ConjEn) << BB_BIT23_POS|\
                                                                                                                                                                                                                (uint64_t)(src0LsfEn)  << BB_BIT22_POS|\
                                                                                                                                                                                                                (uint64_t)(src0Sfb)    << BB_BIT16_POS|\
                                                                                                                                                                                                                (uint64_t)(src1ConjEn) << BB_BIT15_POS|\
                                                                                                                                                                                                                (uint64_t)(src1LsfEn)  << BB_BIT14_POS|\
                                                                                                                                                                                                                (uint64_t)(src1Sfb)    << BB_BIT8_POS |\
                                                                                                                                                                                                                (uint64_t)(dstConjEn)  << BB_BIT7_POS |\
                                                                                                        (uint64_t)(dstLsfEn)   << BB_BIT6_POS |\
                                                                                                                                                                                                                (uint64_t)(dstSfb)     << BB_BIT0_POS)
typedef struct{
    uint8_t src0ConjEn;
    uint8_t src0LsfEn ;
    uint8_t src0Sfb   ;
    uint8_t src1ConjEn;
    uint8_t src1LsfEn ;
    uint8_t src1Sfb   ;
    uint8_t dstConjEn ;
    uint8_t dstLsfEn  ;
    uint8_t dstSfb    ;
}STRUCT_BB_P2_CFG7;
/*********************************CFAR*******************************************/
typedef struct{
    uint8_t wrapDirA;
    uint8_t wrapDirB;
    uint8_t sumMode;
    uint16_t interCnt;
    uint16_t intraCnt;
    uint8_t thresDiv;
    uint8_t divFac;
    uint8_t mulFac;
    uint8_t searchSize;
    uint8_t guardSize;
    uint8_t totalSize;
    uint8_t pdEn;
    uint8_t cfarMode;
}STRUCT_BB_CFAR_CFG0;
#define CFAR_CFG0_WRAPDIR_INC                      (0)
#define CFAR_CFG0_WRAPDIR_DEC                      (1)
#define CFAR_CFG0_SUMMODE_ABS                    (0)
#define CFAR_CFG0_SUMMODE_LOG2                   (2)
#define CFAR_CFG0_SUMMODE_LOG10                  (3)
#define CFAR_CFG0_PEAKDET_EN                     (1)
#define CFAR_CFG0_PEAKDET_DIS                    (0)
#define CFAR_CFG0_CFARMODE_GO                    (0) //max
#define CFAR_CFG0_CFARMODE_SO                    (1) //min
#define CFAR_CFG0_CFARMODE_CA                    (2) //mean
#define CFAR_CFG0_THRESDIV1                      (0)
#define CFAR_CFG0_THRESDIV2                                             (1)
#define CFAR_CFG0_THRESDIV4                                             (2)
#define CFAR_CFG0_THRESDIV8                                             (3)
#define CFAR_CFG0_THRESDIV16                                         (4)
#define CFAR_CFG0_DIVFAC1                        (0)
#define CFAR_CFG0_DIVFAC2                        (1)
#define CFAR_CFG0_DIVFAC4                        (2)
#define CFAR_CFG0_DIVFAC8                        (3)
#define CFAR_CFG0_DIVFAC16                       (4)
#define CFAR_CFG0_DIVFAC32                       (5)
#define BB_CFAR_CFG1(targetLim,srcInterInc)         (bb_regb_str->CFAR_CFG1 = (uint64_t)(targetLim)<<BB_BIT32_POS |(uint64_t)(srcInterInc )<<BB_BIT0_POS)
#define BB_CFAR_CFG2(srcIntraInc,srcBaseAddr)         (bb_regb_str->CFAR_CFG2 = (uint64_t)(srcIntraInc) <<BB_BIT32_POS|(uint64_t)(srcBaseAddr)<<BB_BIT0_POS )
#define BB_CFAR_CFG3(wrapAddrA,wrapAddrB)        (bb_regb_str->CFAR_CFG3 = (uint64_t)(wrapAddrA) <<BB_BIT32_POS|(uint64_t)(wrapAddrB) <<BB_BIT0_POS )
#define BB_CFAR_CFG4(dstBaseAddr)                (bb_regb_str->CFAR_CFG4 = (uint64_t)(dstBaseAddr) <<BB_BIT0_POS )

typedef struct{
    uint16_t interIdx;
    uint16_t intraIdx;
    uint32_t thresABS;
}BB_CFARRST_T;

/*********************************DPK*******************************************/
#define DPK_CFG0_MODE_NORMAL                     (0)
#define DPK_CFG0_MODE_MAX                        (1)
#define DPK_CFG0_MODE_MINUS                      (2)
typedef struct{
    uint32_t cutPow;
    uint32_t idx;
    uint32_t real;
    uint32_t imag;
    uint32_t res;
    uint32_t pFloat;
}BB_DPKRST_T;
#define BB_DPK_CFG0(preData,preIdx,dpkTime,lenBW,mode) (bb_regb_str->DPK_CFG0 = (uint64_t)(preData)<<BB_BIT32_POS|(uint64_t)(preIdx )<<BB_BIT12_POS|(uint64_t)(dpkTime)<<BB_BIT8_POS |(uint64_t)(lenBW  )<<BB_BIT4_POS |(uint64_t)(mode   )<<BB_BIT0_POS )
#define BB_DPK_CFG1( srcIntraInc_a, srcBaseAddr_a) (bb_regb_str->DPK_CFG1 = (uint64_t)(srcIntraInc_a) <<BB_BIT32_POS|(uint64_t)(srcBaseAddr_a) <<BB_BIT0_POS)
#define BB_DPK_CFG2( srcIntraInc_b, srcBaseAddr_b) (bb_regb_str->DPK_CFG2 = (uint64_t)(srcIntraInc_b) <<BB_BIT32_POS|(uint64_t)(srcBaseAddr_b) <<BB_BIT0_POS)
#define BB_DPK_CFG3( refBaseAddr,dstBaseAddr) (bb_regb_str->DPK_CFG3 = (uint64_t)(refBaseAddr) <<BB_BIT32_POS|dstBaseAddr)
/***********************************buf_cfg0****************************************/
#define BUF_CFG0_WR_FLT2PF_LSF_EN_LEFT           (1)
#define BUF_CFG0_WR_FLT2PF_LSF_EN_RIGHT          (0)

#define BUF_CFG0_CONVERT_CPX_MODE_FIX            (0)
#define BUF_CFG0_CONVERT_CPX_MODE_FLOAT          (1)

#define BUF_CFG0_RD_POWMODE_ABS                  (0)
#define BUF_CFG0_RD_POWMODE_LOG2                 (2)
#define BUF_CFG0_RD_POWMODE_LOG10                (3)

#define BUF_CFG0(wr_flt2pf_lsf_en,wr_flt2pf_sfb,convert_cpx_mode,rdPowMode)      (bb_regb_str->BUF_CFG0 = (uint64_t)(wr_flt2pf_lsf_en)<<BB_BIT15_POS|\
                                                                                                                                                                                                                    (uint64_t)(wr_flt2pf_sfb)<<BB_BIT8_POS|\
                                                                                                                                                                                                                    (uint64_t)(convert_cpx_mode)<<BB_BIT4_POS|\
                                                                                                          (uint64_t)(rdPowMode))

/*********************************BB reg*******************************************/
typedef struct
{
    __IO uint64_t PREP_CFG0;        // 0x000
    __IO uint64_t PREP_CFG1;        // 0x008
    __IO uint64_t PREP_CFG2;        // 0x010
    __IO uint64_t PREP_CFG3;        // 0x018
         uint64_t res0[2];          // 0x020, 0x028
    __IO uint64_t ZO_CFG0;          // 0x030
      __IO uint64_t ZO_STA0;          // 0x038
         uint64_t res1[2];          // 0x040, 0x048
    __IO uint64_t DMA_CFG0;         // 0x050
    __IO uint64_t DMA_CFG1;         // 0x058
    __IO uint64_t DMA_CFG2;         // 0x060
    __IO uint64_t DMA_CFG3;         // 0x068
    __IO uint64_t DMA_CFG4;         // 0x070
        __IO uint64_t DMA_STA0;         // 0x078
        __IO uint64_t DMA_STA1;         // 0x080
        __IO uint64_t DMA_STA2;         // 0x088
    __IO uint64_t P1MUL_CFG0;       // 0x090
    __IO uint64_t P1MUL_CFG1;       // 0x098
         uint64_t res3[2];          // 0x0A0, 0x0A8
    __IO uint64_t P2_CFG0;          // 0x0B0
    __IO uint64_t P2_CFG1;          // 0x0B8
    __IO uint64_t P2_CFG2;          // 0x0C0
    __IO uint64_t P2_CFG3;          // 0x0C8
    __IO uint64_t P2_CFG4;          // 0x0D0
    __IO uint64_t P2_CFG5;          // 0x0D8
    __IO uint64_t P2_CFG6;          // 0x0E0
    __IO uint64_t P2_CFG7;          // 0x0E8
         uint64_t res4[2];          // 0x0F0, 0x0F8
    __IO uint64_t CFAR_CFG0;        // 0x100
    __IO uint64_t CFAR_CFG1;        // 0x108
    __IO uint64_t CFAR_CFG2;        // 0x110
    __IO uint64_t CFAR_CFG3;        // 0x118
    __IO uint64_t CFAR_CFG4;        // 0x120
    __IO uint64_t CFAR_STA0;        // 0x128
         uint64_t res5[2];          // 0x130, 0x138
    __IO uint64_t DPK_CFG0;         // 0x140
    __IO uint64_t DPK_CFG1;         // 0x148
    __IO uint64_t DPK_CFG2;         // 0x150
    __IO uint64_t DPK_CFG3;         // 0x158
    __IO uint64_t DPK_CFG4;         // 0x160
         uint64_t res6[3];          // 0x168, 0x170, 0x178
    __IO uint64_t BUF_CFG0;         // 0x180
         uint64_t res7[15];         // 0x188, 0x190, 0x198, 0x1A0, 0x1A8, 0x1B0, 0x1B8, 0x1C0, 0x1C8, 0x1D0, 0x1D8, 0x1E0, 0x1E8, 0x1F0, 0x1F8
    __IO uint64_t FFT_CFG0;         // 0x200
    __IO uint64_t FFT_CFG1;         // 0x208
    __IO uint64_t FFT_CFG2;         // 0x210
    __IO uint64_t FFT_CMD0;         // 0x218
    __IO uint64_t FFT_SRC_ZP00;     // 0x220
    __IO uint64_t FFT_SRC_ZP01;     // 0x228
    __IO uint64_t FFT_SRC_ZP02;     // 0x230
    __IO uint64_t FFT_SRC_ZP03;     // 0x238
    __IO uint64_t FFT_SRC_ZP04;     // 0x240
    __IO uint64_t FFT_SRC_ZP05;     // 0x248
    __IO uint64_t FFT_SRC_ZP06;     // 0x250
    __IO uint64_t FFT_SRC_ZP07;     // 0x258
    __IO uint64_t FFT_SRC_ZP08;     // 0x260
    __IO uint64_t FFT_SRC_ZP09;     // 0x268
    __IO uint64_t FFT_SRC_ZP10;     // 0x270
    __IO uint64_t FFT_SRC_ZP11;     // 0x278
    __IO uint64_t FFT_SRC_ZP12;     // 0x280
    __IO uint64_t FFT_SRC_ZP13;     // 0x288
    __IO uint64_t FFT_SRC_ZP14;     // 0x290
    __IO uint64_t FFT_SRC_ZP15;     // 0x298
    __IO uint64_t FFT_STA0;         // 0x2A0
    __IO uint64_t FFT_STA1;         // 0x2A8
         uint64_t res8[10];         // 0x2B0, 0x2B8, 0x2C0, 0x2C8, 0x2D0, 0x2D8, 0x2E0, 0x2E8, 0x2F0, 0x2F8
    __IO uint64_t IMR;              // 0x300
         uint64_t res9[1];          // 0x3A8
    __IO uint64_t EMR;              // 0x310
         uint64_t resA[1];          // 0x318
    __IO uint64_t ISR;              // 0x320
         uint64_t resB[1];          // 0x328
    __IO uint64_t ESR;              // 0x330
         uint64_t resC[25];         // 0x338, 0x340, 0x348, 0x350, 0x358, 0x360, 0x368, 0x370, 0x378, 0x380, 0x388, 0x390, 0x398, 0x3A0, 0x3A8, 0x3B0, 0x3B8, 0x3C0, 0x3C8, 0x3D0, 0x3D8, 0x3E0, 0x3E8, 0x3F0, 0x3F8
    __IO uint64_t OP_GATE;          // 0x400
    __IO uint64_t OP_TRIG;          // 0x408
    __IO uint64_t OP_CLEAR;         // 0x410
} BB_REGB_T;
#define BB_REGB      ((BB_REGB_T  *)   BB_REGB_BASE)
#define BB_SIZE8        (0x0)
#define BB_SIZE16       (0x1)
#define BB_SIZE32       (0x2)
#define BB_SIZE64       (0x3)
/*********************************IMR*******************************************/
#define BB_IMR_P1MUL_END            (1<<16)
#define BB_IMR_FFT4_UNLOAD_END      (1<<15)
#define BB_IMR_FFT4_CALC_END        (1<<14)
#define BB_IMR_DPK_END              (1<<13)
#define BB_IMR_CFAR_END             (1<<12)
#define BB_IMR_P2_JUMP_END          (1<<11)
#define BB_IMR_P2_INTER_END         (1<<10)
#define BB_IMR_P2_INTRA_END         (1<<9)
#define BB_IMR_DMA_JUMP_END         (1<<8)
#define BB_IMR_DMA_INTER_END        (1<<7)
#define BB_IMR_DMA_INTRA_END        (1<<6)
#define BB_IMR_RAMP_NEDGE           (1<<5)
#define BB_IMR_RAMP_PEDGE           (1<<4)
#define BB_IMR_PREP_FRAME_END       (1<<3)
#define BB_IMR_PREP_QUARTER_END     (1<<2)
#define BB_IMR_PREP_RAMP_END           (1<<1)
#define BB_IMR_CLEAR(val)           (bb_regb_str->IMR |= (uint64_t)val)
#define BB_IMR_SET(val)             (bb_regb_str->IMR&=~((uint64_t)val))
#define BB_IMR_Init(val)            (bb_regb_str->IMR=~((uint64_t)val))
/*********************************EMR*******************************************/
#define BB_EMR_INVALID_ADDR          (1<<40ULL)
#define BB_EMR_WBUF_CRASH            (1<<39ULL)
#define BB_EMR_DBUF1_CRASH           (1<<38ULL)
#define BB_EMR_DBUF0_CRASH           (1<<37ULL)
#define BB_EMR_ABUF4_CRASH           (1<<36ULL)
#define BB_EMR_ABUF3_CRASH           (1<<35ULL)
#define BB_EMR_ABUF2_CRASH           (1<<34ULL)
#define BB_EMR_ABUF1_CRASH           (1<<33ULL)
#define BB_EMR_ABUF0_CRASH           (1<<32ULL)
#define BB_EMR_DMA_CONVERT_OVF       (1<<8)
#define BB_EMR_BUF_OVF               (1<<7)
#define BB_EMR_DPK_OVF               (1<<6)
#define BB_EMR_CFAR_OVF              (1<<5)
#define BB_EMR_P2_OVF                (1<<4)
#define BB_EMR_FFT_OVF               (1<<3)
#define BB_EMR_CALIB_MUL1_OVF        (1<<2)
#define BB_EMR_CALIB_MUL0_OVF        (1<<1)
#define BB_EMR_ZO_THRES_EXCESS       (1<<0)
#define BB_EMR_Init(val)             (bb_regb_str->EMR&=~(uint64_t)(val))
/*********************************ISR*******************************************/
#define BB_ISR_P1MUL_END                (1<<16)
#define BB_ISR_FFT_UNLOAD_END       (1<<15)
#define BB_ISR_FFT_CALC_END         (1<<14)
#define BB_ISR_DPK_END              (1<<13)
#define BB_ISR_CFAR_END             (1<<12)
#define BB_ISR_P2_JUMP_END          (1<<11)
#define BB_ISR_P2_INTER_END         (1<<10)
#define BB_ISR_P2_INTRA_END         (1<<9)
#define BB_ISR_DMA_JUMP_END         (1<<8)
#define BB_ISR_DMA_INTER_END        (1<<7)
#define BB_ISR_DMA_INTRA_END        (1<<6)
#define BB_ISR_RAMP_NEDGE           (1<<5)
#define BB_ISR_RAMP_PEDGE           (1<<4)
#define BB_ISR_PREP_FRAME_END       (1<<3)
#define BB_ISR_PREP_QUARTER_END     (1<<2)
#define BB_ISR_PREP_RAMP_END       (1<<1)
#define BB_ISR_CLEAR(val)           (bb_regb_str->ISR = (uint64_t)(val))
#define BB_ISR_CLEAR_ALL            (bb_regb_str->ISR = (uint64_t)(0xFFFFFFFFFFFFFFFFULL))
/*********************************ESR*******************************************/
#define BB_ESR_INVALID_ADDR          (1<<40ULL)
#define BB_ESR_WBUF_CRASH            (1<<39ULL)
#define BB_ESR_DBUF1_CRASH           (1<<38ULL)
#define BB_ESR_DBUF0_CRASH           (1<<37ULL)
#define BB_ESR_ABUF4_CRASH           (1<<36ULL)
#define BB_ESR_ABUF3_CRASH           (1<<35ULL)
#define BB_ESR_ABUF2_CRASH           (1<<34ULL)
#define BB_ESR_ABUF1_CRASH           (1<<33ULL)
#define BB_ESR_ABUF0_CRASH           (1<<32ULL)
#define BB_ESR_DMA_CONVERT_OVF       (1<<8)
#define BB_ESR_BUF_OVF               (1<<7)
#define BB_ESR_DPK_OVF               (1<<6)
#define BB_ESR_CFAR_OVF              (1<<5)
#define BB_ESR_P2_OVF                (1<<4)
#define BB_ESR_FFT_OVF               (1<<3)
#define BB_ESR_CALIB_MUL1_OVF        (1<<2)
#define BB_ESR_CALIB_MUL0_OVF        (1<<1)
#define BB_ESR_ZO_THRES_EXCESS       (1<<0)
#define BB_ESR_CLEAR(val)           (bb_regb_str->ESR = (uint64_t)(val))
/*********************************OP_GATE*******************************************/
#define BB_GATE_DPK     (1<<6)
#define BB_GATE_CFAR    (1<<5)
#define BB_GATE_P2      (1<<4)
#define BB_GATE_FFT     (1<<3)
#define BB_GATE_P1MUL   (1<<2)
#define BB_GATE_DMA     (1<<1)
#define BB_GATE_PREP    (1<<0)
#define BB_OPGATE_EN(val) (bb_regb_str->OP_GATE &= ~(uint64_t)(val))
#define BB_OPGATE_DIS(val) (bb_regb_str->OP_GATE |= ((uint64_t)(val)))
/*********************************OP_TRIG*******************************************/
#define BB_TRIG_DPK            (1<<4)
#define BB_TRIG_CFAR           (1<<3)
#define BB_TRIG_P2             (1<<2)
#define BB_TRIG_FFT            (1<<1)
#define BB_TRIG_DMA            (1<<0)
#define BB_OPTRIG(val)         (bb_regb_str->OP_TRIG |= (uint64_t)(val))
/*********************************OP_CLEAR*******************************************/
#define BB_CLEAR_DPK           (1<<6)
#define BB_CLEAR_CFAR          (1<<5)
#define BB_CLEAR_P2            (1<<4)
#define BB_CLEAR_FFT           (1<<3)
#define BB_CLEAR_P1MUL         (1<<2)
#define BB_CLEAR_DMA           (1<<1)
#define BB_CLEAR_PREP          (1<<0)
#define BB_OPCLEAR(val)        (bb_regb_str->OP_CLEAR |= (uint64_t)(val))

/*BUF0*/
#define BB_ABS_OFFSET   (0x400000)
#define BB_ABUF0_CPX_BASE  (BB_ABUF_BASE+0x800000)
#define BB_ABUF1_CPX_BASE  (BB_ABUF_BASE+0x800000+512*1024)
#define BB_ABUF2_CPX_BASE  (BB_ABUF_BASE+0x800000+1024*1024)
#define BB_ABUF3_CPX_BASE  (BB_ABUF_BASE+0x800000+1536*1024)
#define BB_ABUF4_CPX_BASE  (BB_ABUF_BASE+0x800000+2048*1024)
#define BB_DBUF0_CPX_BASE  (BB_ABUF_BASE+0x800000+6144*1024)
#define BB_DBUF1_CPX_BASE  (BB_ABUF_BASE+0x800000+6144*1024+128*1024)
#define BB_WBUF_CPX_BASE   (BB_ABUF_BASE+0x800000+7168*1024)


#ifdef __cplusplus
}
#endif












#endif

