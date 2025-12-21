/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _WDT_DRIVER_H
#define _WDT_DRIVER_H
#include <stdint.h>
#include "IRQ_Handler.h"
#include <stdio.h>

#include "SKY32B750.h"

void WDT_RstNow(void);
void WDT_Init(uint32_t val);
void Close_Wdt(void);

#define WDT_FEED_DOGS()           (WDT->STR = 0x1)                        // bit0:clear












#endif /*_WDT_DRIVER_H */

/************************ (C) COPYRIGHT  *****END OF FILE****/























