/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _DMA_DRIVER_H
#define _DMA_DRIVER_H
#include "dma.h"

void DMA_Config_Init(void);
void DMA_Start_TransferM2M_32(uint32_t *srcAddr,uint32_t *dstAddr,uint32_t Len,enum dma_channel_number channel);
void DMA_Start_TransferM2M_SRAM2K(uint32_t srcAddr,uint32_t dstAddr,uint32_t Len,enum dma_channel_number channel);
#endif /*_DMA_DRIVER_H */

/************************ (C) COPYRIGHT  *****END OF FILE****/























