#include "main.h"

void Sys_Init(void)
{
//		Set_CoreLDO_Voltage(0);
    LRC_DISABLE;   // Close LRC
    //AON_SYSC->ANA_CFG0 = 0xA0181050; //default:0xA0181050
    //AON_SYSC->ANA_CFG1 = 0x2D004A; //default:0x2D004A
    SET_WKUP_MASK0(0xFFFFFFFF);
    SET_WKUN_MASK0(0xFFFFFFFF);
    CLR_WKUP_MASK0(GPIO1_10_WKU_MSK);  //GPIO1.10 Wakeup Enable
    CLR_WKUN_MASK0(GPIO1_10_WKU_MSK);  //GPIO1.10 Wakeup Enable
    SET_WKUP_MASK1(0xFFFFFFFF);
    SET_WKUN_MASK1(0xFFFFFFFF);
    SET_WKUP_MASK2(0xFFFFFFFF);

    Set_Flash_WS(0xF);  //flash ws
		USART_Init(); //systemClock/4
    GPIO_Init();
    SPI_Init();	
	  RF_Clock40M_En();      //开40M
		
		SYS_CLK_HRC_Init();    //hclk=243MHz, pclk=121MHz
		SYS_CLK_PLL_Init();
	
		SRAM1_CLK_ENABLE;
		SRAM2_CLK_ENABLE;
//		OSPI_MasterInit();
		ADC_Init();
		RTC_Init();
		WDT_Set_ms(10000);
}/*}}}*/

void App_Init(void){
    ParamConfigInit();
    Func_Init();
		if(r2_str.Proj_Func==2){ //ospi不能用这个
//			SysTick_Init();
		}
}
void App_Func_Loop(void)
{
    switch(r2_str.Proj_Func)
    {
        case 0x01:
          
            Func01_Handle(); //sample data
            break;
        case 0x02: //各种功能代码，需要宏定义切换
            Func02_Handle(); //sample data
            break;
        case 0x03:
            Func03_Handle(); //print import data
            break;
        default:
            break;
    }
}
void App_Loop(void)
{
    switch(r2_str.WorkMode)
    {
        case 0:  //Config Mode
            RecProtocol_Loop();
            break;
        case 1:  //Work Mode				
            App_Func_Loop();
            break;
        case 2:     //Import Mode
            if(    strUsart.writeCnt>=IMPORT_MAX_LEN){
                strUsart.writeCnt = 0;
                r2_str.WorkMode = 0;
                printf("AT+OK\n");
//                USART_Transmit_Bytes(&USART0, (uint8_t*)IMPORT_ADDR,IMPORT_MAX_LEN);
            }
            break;
        default:
//            printf("wm=%d\r\n",r2_str->WorkMode);
            break;
    }
		WDT_CLEAR_COUNTER;
}

int main(void)
{
		Set_CoreLDO_Voltage(2);
    Sys_Init();
    MPU_LocalConfig();
    App_Init();
    while (1)
    {
				//printf("hello world\n");
        App_Loop();
    }
}/*}}}*/
// vim:fdm=marker
