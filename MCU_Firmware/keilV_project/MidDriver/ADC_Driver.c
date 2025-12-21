#include "ADC_Driver.h"
#include "main.h"

void ADC_Init(void){
//	uint8_t addr=0;
//	uint8_t rtn;

//	Write_M32(0x50000000, 0x0);
//	NVIC->ICER[0]= 0xFFFFFFFF;
//	NVIC->ICER[1]= 0xFFFFFFFF;
//	RF_Clock40M_En();
	#if 0
	uint8_t testr=0x55;


	RF_SPI_Init();
	AON_SYSC->CLK_EN &= ~AON_CFG_PU_XTAL40M_Msk;
	AON_SYSC->CLK_EN |= AON_CFG_PU_XTAL40M_Msk;
	AON_SYSC->CLK_EN |= AON_CFG_PU_PLL_Msk;
	AON_SYSC->ANA_CFG0 = 0xA0181058; //23:1=div2
	AON_SYSC->ANA_CFG1 = 0x2C004A; //16£ºpll post div
	printf("1 ANA_CFG0=%08X\n",AON_SYSC->ANA_CFG0);
//	AON_SYSC->ANA_CFG0 = 0xA7981050; //23:1=div2 0xA7181050:±äµçÁ÷
	printf("2 ANA_CFG0=%08X\n",AON_SYSC->ANA_CFG0);
//	RF_CHIPEN_Disable();
//	AON_SYSC->ANA_CFG0 = 0xA0181050; //23:1=div2
//	printf("3 ANA_CFG0=%08X\n",AON_SYSC->ANA_CFG0);
//	AON_SYSC->CLK_EN &= ~AON_CFG_PU_PLL_Msk;
	while(1){
			USART_Transmit_Bytes(&USART0,&testr,1);
			Cnt_Delay_ms(100);
	}
	#endif
	FADC_GRP0_LDO_ENABLE;
	FADC_GRP1_LDO_ENABLE;
//		AON_SYSC->ANA_CFG1 |=ANA_CFG1_ADC_CLK_REV_Msk;
	Cnt_Delay_ms(1);
	FADC_CLK_ENABLE;
	HXTAL_ENABLE;

//	AON_SYSC->ANA_CFG1 = 0x2d004a;
//	AON_SYSC->ANA_CFG1 = 0x0D004A; //0xAD004A    0xAD004A: register sync:high         0x2D004A : register sync:low
//	AON_SYSC->ANA_CFG1 = 0x0F004A; //SYNC  0d004a
	// AON_SYSC->ANA_CFG1&=~(1<<21); //SYNC   0:out   1:in
	SEL_PAD_GEN_SYNC;       //Select PAD Gen Sync
	SEL_GPIO1_0_GEN_SYNC;   //Select GPIO1.0 Gen Sync
	FADC_SPI_CLK_DIV(0x01);
	FADC_SPI_CLK_ENABLE;
//	printf("cfg0=%08X,cfg1=%08X\n",AON_SYSC->ANA_CFG0,AON_SYSC->ANA_CFG1);
//	printf("read ADC_SPI_CONF:\n");
	FADC_SPI_WriteSingle(0x00,0x16,0x00);
	FADC_SPI_WriteSingle(0x00,0x18,0x00);
//	for(addr=0;addr<9;addr++){
//		rtn=FADC_SPI_ReadSingle(0xFE,addr+0x10);
//		printf("addr=%02X,val=%02X\n",addr+0x10,rtn);
//	}
	FADC_SPI_WriteSingle(0x00,0x16,0x82); //0X82   manual:not care sync   trigpol: ~sync
	#if 0
	printf("read ADC_SPI_CONF 2:\n");
	for(uint8_t cnt=0,chn=0;cnt<8;cnt++){
		for(addr=0;addr<9;addr++){
			chn=0xFF&(~(1<<cnt));
			rtn=FADC_SPI_ReadSingle(chn,addr+0x10);
			printf("chn=%02X,channel=%d,addr=%02X,val=%02X\n",chn,cnt,addr+0x10,rtn);
		}
		printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	}
	#endif
	// PORT23_PIN_CTRL->AE = 0xFFFF; //open input for analog 2.0-2.15
	GPIO_AE_Ctrl(GPIO2, 0x00FF, ANALOG_MODE);
	FADC_SPI_WriteSingle(0xFE, 0x18, 0x40); //Set Vref 1.1V
	FADC_SPI_WriteSingle(0xEF, 0x18, 0x40); //Set Vref 1.1V
	FADC_SPI_WriteSingle(0xFD,0x18,0xE0); //4channel input buf en  0-3
//	FADC_SPI_WriteSingle(0xDF,0x18,0xE0); //4channel input buf en  4-7
	//GPIO1.1
	#if 0
	FADC_SPI_WriteSingle(0x77,0x18,0x00);
	PORT01_PIN_CTRL->AE = 0x20000;
	PORT01_PIN_CTRL->AT = 0x20000;
	FADC_SPI_WriteSingle(0xF7,0x18,0xE0);
	#endif

	//bb
	#if 0
	RF_SPI_Init();
	SYSC->CLK_EN |= RADAR_BB_EN_Msk;
	SYSC->RST &= ~0x200000;
	SYSC->RST |= 0x200000;
	BB_OPGATE_DIS(BB_GATE_PREP);
	BB_OPCLEAR(BB_CLEAR_PREP);
	BB_PrepConfig(&bb_prep);
	memset((uint8_t *)ADC_SAMPLE_ADDR,0,10*1024);
	waveGenStart();
	Cnt_Delay_ms(10);
	BB_ISR_CLEAR(BB_ISR_PREP_FRAME_END|BB_ISR_RAMP_NEDGE);
	BB_OPGATE_EN(BB_GATE_PREP);
  BB_waitISR(BB_ISR_PREP_FRAME_END);
//	RF_CHIPEN_Disable();
	USART_Transmit_Bytes(&USART0,(uint8_t *)ADC_SAMPLE_ADDR,ADC_MAX_LEN*ADC_BYTE);
//	USART_Transmit_Bytes(&USART0,(uint8_t *)ADC_SAMPLE_ADDR,ADC_MAX_LEN*ADC_BYTE*NUM_ANT);
	while(1);
	#endif
}
