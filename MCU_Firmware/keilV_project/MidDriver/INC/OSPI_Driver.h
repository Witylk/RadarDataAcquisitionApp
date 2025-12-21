/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _OSPI_DRIVER_H
#define _OSPI_DRIVER_H
#include "ospi.h"


extern uint8_t ospi_sendover_flag;
extern uint8_t ospi_receiveover_flag;




void OSPI_MasterInit(void);
void OSPI_SlaveInit(void);
void OSPI_Trans64(uint64_t *srcAddr,uint32_t len);
void OSPI_Receive64(uint32_t *dstAddr,uint32_t len);

void OSPI_TransDMA64(uint32_t *srcAddr,uint32_t len);












#endif /*_OSPI_DRIVER_H */

/************************ (C) COPYRIGHT  *****END OF FILE****/























