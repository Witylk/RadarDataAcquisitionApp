#include "DMA_Driver.h"

void (*dma_callback[DMA_CHANNEL_MAX])(void);
dma_channel_config_t dmaConfigStr;
DMA_CH_T *g_dma_ch;
void DMA_Config_Init(void){
    DMA_Clear_All_Irq();
    DMA_Init();
}

void DMA_Start_TransferM2M_32(uint32_t *srcAddr,uint32_t *dstAddr,uint32_t Len,enum dma_channel_number channel){

    DMA_CLK_ENABLE;

    dmaConfigStr.sar = (uint32_t)&srcAddr[0];
    dmaConfigStr.dar = (uint32_t)&dstAddr[0];
    dmaConfigStr.ctl_src_tr_width = DMA_TRANS_WIDTH_32;
    dmaConfigStr.ctl_dst_tr_width = DMA_TRANS_WIDTH_32;
    dmaConfigStr.ctl_dinc =  DMA_ADDR_INCREMENT;
    dmaConfigStr.ctl_sinc =  DMA_ADDR_INCREMENT;
    dmaConfigStr.ctl_dst_msize = DMA_MSIZE_1;
    dmaConfigStr.ctl_src_msize = DMA_MSIZE_1;
    dmaConfigStr.ctl_tt_fc = DMA_MEM2MEM_DMA;
    dmaConfigStr.ctl_block_ts = Len;

    DMA_Disable();
    DMA_Channel_Configure(channel, &dmaConfigStr, NULL);
    DMA_Enable();
    DMA_Channel_Enable(DMA_CHANNEL0);
    while((DMA_INT->RawBlock&(0x01<<channel))!= (0x01<<channel)){}
    DMA_INT->ClearBlock  = (0x01<<channel);
}

void DMA_Start_TransferM2M_SRAM2K(uint32_t srcAddr,uint32_t dstAddr,uint32_t Len,enum dma_channel_number channel){
		uint32_t sBlockMod=(Len/4)%512; //yushu
		uint32_t sTimes=Len/2048;
		uint32_t sAddrOffset=0;
    DMA_CLK_ENABLE;
		SRAM3_CLK_ENABLE;
//    dmaConfigStr.sar = srcAddr;
//    dmaConfigStr.dar = dstAddr;
    dmaConfigStr.ctl_src_tr_width = DMA_TRANS_WIDTH_32;
    dmaConfigStr.ctl_dst_tr_width = DMA_TRANS_WIDTH_32;
    dmaConfigStr.ctl_dinc =  DMA_ADDR_INCREMENT;
    dmaConfigStr.ctl_sinc =  DMA_ADDR_INCREMENT;
    dmaConfigStr.ctl_dst_msize = DMA_MSIZE_1;
    dmaConfigStr.ctl_src_msize = DMA_MSIZE_1;
		dmaConfigStr.ctl_sms             = DMA_MASTER1;
		dmaConfigStr.ctl_dms             = DMA_MASTER1;
    dmaConfigStr.ctl_tt_fc = DMA_MEM2MEM_DMA;
//		    dmaConfigStr.ctl_block_ts = Len/4;  //128 max
		dmaConfigStr.cfg_hs_sel_src      = DMA_HS_HARDWARE;
		dmaConfigStr.cfg_hs_sel_dst      = DMA_HS_HARDWARE;
		dmaConfigStr.ctl_int_en          = 0;
		
		
		DMA_Channel_Configure(channel, &dmaConfigStr, NULL);
		DMA_Clear_Irq(channel, DMA_IRQ_ALL);
		g_dma_ch    = DMA_Num2Ch(channel);
//		DMA_Mask_Irq(channel, DMA_IRQ_TFR);
//		DMA_Unmask_Irq(channel, DMA_IRQ_TFR);
		g_dma_ch->HCTL=512;
		for(uint32_t timNow=0;timNow<sTimes;timNow++){
			DMA_Disable();
			g_dma_ch->SAR=srcAddr+sAddrOffset;
			g_dma_ch->DAR=0x2000F800;
			DMA_Enable();
			DMA_Channel_Enable(channel);
			while(((DMA_INT->RawTfr) & (1<<channel))==0);
			DMA_Clear_Irq(channel, DMA_IRQ_TFR);
			g_dma_ch->SAR=0x2000F800;
			g_dma_ch->DAR=dstAddr+sAddrOffset;
			DMA_Enable();
			DMA_Channel_Enable(channel);
			while(((DMA_INT->RawTfr) & (1<<channel))==0);
			DMA_Clear_Irq(channel, DMA_IRQ_TFR);
			sAddrOffset=sAddrOffset+2048;			
		}		
		if(sBlockMod){
			DMA_Disable();
			g_dma_ch->SAR=srcAddr+sAddrOffset;
			g_dma_ch->DAR=dstAddr+sAddrOffset;
			g_dma_ch->HCTL=sBlockMod;
			DMA_Enable();
			DMA_Channel_Enable(channel);	
			while(((DMA_INT->RawTfr) & (1<<channel))==0);
			DMA_Clear_Irq(channel, DMA_IRQ_TFR);			
		}
}
