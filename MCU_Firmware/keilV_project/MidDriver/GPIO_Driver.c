#include "GPIO_Driver.h"
#include "main.h"
void GPIO_Init(void){

    GPIO_SetMode(CHIPEN_GPIO_Port, CHIPEN_Pin, GPIO_PMODE_OUTPUT);
    GPIO_SetMode(SPI0_CS1_GPIO_Port, SPI0_CS1_Pin, GPIO_PMODE_OUTPUT);
//    GPIO_SetMode(SPI0_CS2_GPIO_Port, SPI0_CS2_Pin, GPIO_PMODE_OUTPUT);
//    GPIO_SetMode(SPI0_CS3_GPIO_Port, SPI0_CS3_Pin, GPIO_PMODE_OUTPUT);
//    GPIO_SetMode(SPI0_CS4_GPIO_Port, SPI0_CS4_Pin, GPIO_PMODE_OUTPUT);

//    GPIO_SetMode(TEST_GPIO_Port, TEST_Pin, GPIO_PMODE_OUTPUT);
//	while(1){
//		Cnt_Delay_ms(100);
//		TEST_WritePin = 0;
//				Cnt_Delay_ms(100);
//		TEST_WritePin = 1;
//	}
    CHPIEN_WritePin = 0;
    SPI0_CS1_WritePin = 1;
//    SPI0_CS2_WritePin = 0;
//    SPI0_CS3_WritePin = 0;
//    SPI0_CS4_WritePin = 0;
//    TEST_WritePin = 0;



}
