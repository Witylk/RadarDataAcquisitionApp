#include "IRQ_Handler.h"
uint32_t SysTick_Counter = 0;
uint32_t firstChirpCnt = 0;
uint8_t PA_Cnt = 0;
uint32_t rampCnt=0;
void BB_IRQHandler(void){
	#if 1
		if(bb_regb_str->ISR&BB_ISR_PREP_RAMP_END){
			bb_regb_str->ISR = BB_ISR_PREP_RAMP_END;			//必须放前面	
			FADC_SPI_CLK_DISABLE;
			if(firstChirpCnt==0)	{
				BB_OPCLEAR(BB_CLEAR_PREP);
				firstChirpCnt = 1;
			}		
			rampCnt++;
			if(rampCnt==NUM_CHIRP_QUARTERx3){
				bb_regb_str->PREP_CFG0=bb_regb_str->PREP_CFG0&((uint64_t)0xFF000FFFFFFFFFFF);
				bb_regb_str->PREP_CFG0=bb_regb_str->PREP_CFG0|((uint64_t)NUM_CHIRP_TX<<BB_BIT44_POS);
			}			
		}
		if(bb_regb_str->ISR&BB_ISR_RAMP_PEDGE){
			bb_regb_str->ISR = BB_ISR_RAMP_PEDGE;			//必须放前面
			for(uint32_t i=0;i<100;i++){
				__NOP();
			}
			FADC_SPI_CLK_ENABLE;
		}	


//		RF_PA_FASTSW(sCnt);	
//		prepCfg1_TX_Seq[0]=TX_SEQ[PA_Cnt];
//		if(PA_Cnt>=NUM_TX){
//			PA_Cnt = 0;
//		}
//		if(firstChirpCnt==0)	{
//			BB_OPCLEAR(BB_CLEAR_PREP);
//			firstChirpCnt = 1;
//		}						
	#endif

//		bb_regb_str->ISR = BB_ISR_PREP_RAMP_END;			//必须放前面			
//		firstChirpCnt++;
}
	
void SysTick_Handler(void){
//	uint8_t sTemp=TEST_WritePin;
//	TEST_WritePin = 1-sTemp;
	SysTick_Counter++;
}

