#include "RTC_Driver.h"
#include "rtc.h"
#include "USART_Driver.h"
#include "Delay_Driver.h"

void Pre_Config(void)
{
//    PORT01_PINMUX->PORT_MUX0 = 0x00000000;
//    PORT01_PINMUX->PORT_MUX1 = 0x00000000;
//    PORT01_PINMUX->PORT_MUX2 = 0x00000000;
//    PORT01_PINMUX->PORT_MUX3 = 0x00000000;

//    PORT23_PINMUX->PORT_MUX0 = 0x00000000;
//    PORT23_PINMUX->PORT_MUX1 = 0x00000000;
//    PORT23_PINMUX->PORT_MUX2 = 0x00000000;
//    PORT23_PINMUX->PORT_MUX3 = 0x00000000; //GPIO3.14、GPIO3.15的PINMUX默认为0，是SWD模式，配成非0非1的值才能工作为普通IO模式。

//    PORT01_PIN_CTRL->RE = 0xFFFFFFFF;
//    PORT23_PIN_CTRL->RE = 0xFFFFFFFF;
//    PORT01_PIN_CTRL->UE = 0x000c0400;//由于射频芯片GPIO0.10和GPIO1.0灌电流，需要特别将这几个IO拉高来保证静态功耗测试正常
//    PORT23_PIN_CTRL->UE = 0x00010000;
//    PORT01_PIN_CTRL->AE = 0x0;
//    PORT23_PIN_CTRL->AE = 0x0;
	
	
//		PORT01_PIN_CTRL->RE =0;
//		PORT23_PIN_CTRL->RE =0;
		GPIO0->PMODE=0;
//		GPIO1->PMODE=0;
		GPIO2->PMODE=0;
//		GPIO3->PMODE=0;
//    LXTAL_DISABLE;
		PLL_DISABLE;
		HXTAL_DISABLE;
        ALL_IOLATCH_ENABLE;
		CLR_WKUN_MASK0(GPIO1_3_WKU_MSK);
		WDT_Disable();
 
    SYSC->CLK_EN |= BIT16;//always_on_pclk_en
}
void After_Config(void){
		SYSC->CLK_EN=0x1E218181;
		ALL_IOLATCH_DISABLE;
	    WDT_Enable();
		GPIO0->PMODE=0x400;
		GPIO2->PMODE=0x400;
//	printf("testWakeUp\n");
}
void DeepSleep_Config(void){
	Pre_Config();
	Deep_Sleep();
	After_Config();
//	Cnt_Delay_ms(100);
}
void RTC_Init(void){
	CLR_WKUP_MASK2(AON_WKU_RTC_MASK_Msk);
	RTC_Enable(SOURCE_RC32K);
	#if 0 //test RTC wakeup

	 Cnt_Delay_ms(5000);
	
//	Power_Down();
	while(1){
		DeepSleep_Config();
		printf("DeepSleep\n");
		Cnt_Delay_ms(1000);
	}
	#endif
}










