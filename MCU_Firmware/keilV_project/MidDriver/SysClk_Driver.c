#include "SysClk_Driver.h"
#include "Delay_Driver.h"

void SYS_CLK_PLL_Init(void){
    Set_Flash_WS(8); //flash WS
    HXTAL_DISABLE;
    #ifdef CLK_320M_EN
    Set_PLL(XTAL_40M, 0,4, 0);//hxtal_freq,prediv,mdiv,posdiv   (((hxtal_freq / (1 + prediv)) * (8 + mdiv)) / (2 + posdiv * 2))
    HXTAL_ENABLE;
    PLL_ENABLE;
//    Cnt_Delay_ms(10);
    Set_SysClk_Div(0, 2);   //hclk=320M, pclk=160M
    PLL_System_Clock_Update();
    USCI_UART_Init(&USART0);
//    printf("sys_clk = %dHz\n", sys_clk);
//    printf("ANACFG0=%08X\n",AON_SYSC->ANA_CFG0);
//    printf("ANACFG1=%08X\n",AON_SYSC->ANA_CFG1);
    #endif
}

/*
0xF=345M  0xE=333M  0xD=320M  0xC=306M  0xB=293M  0xA=279M  0x9=264M  0x8=249M
0x7=266M  0x6=255M  0x5=245M  0x4=233M  0x3=220M  0x2=209M  0x1=197M  0x0=185M
*/
void SYS_CLK_HRC_Init(void){
    HXTAL_DISABLE;
    PLL_DISABLE;
    Set_HRC(0x8, 0, 1);
    Set_SysClk_Div(0, 1);   //hclk=243MHz, pclk=121MHz
    HXTAL_Caliber_LRC(XTAL_40M, 100);
    LRC_Caliber_HRC(XTAL_40M, 100);
    HRC_System_Clock_Update();
//    Cnt_Delay_ms(10);
    USCI_UART_Init(&USART0);
    // printf("sys_clk = %dHz\n", sys_clk);
}

















