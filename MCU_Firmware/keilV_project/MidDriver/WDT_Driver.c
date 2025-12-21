#include "WDT_Driver.h"



void WDT_RstNow(void){

  RTC_DISABLE;
    WDT->CMP = 0;
    WDT->STR = 0xF;                        // bit0:clear
}



void WDT_Init(uint32_t val)
{
    RTC_DISABLE;
    WDT->CMP = (uint16_t)val;
    WDT->STR = 0xF;                    // bit0:clear

}
void Close_Wdt(void)
{
    WDT_CLEAR_COUNTER;

//    NVIC_DisableIRQ(WDT_IRQn);
    WDT_Disable();
}







