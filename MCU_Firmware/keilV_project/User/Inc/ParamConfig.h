/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _PARAMCONFIG_H
#define _PARAMCONFIG_H

#include <stdint.h>
#include <string.h>

#include "App_Def.h"
#include "USART_Driver.h"

//#include "stm32g4xx_ll_iwdg.h"
#define SAMPLE_LEN           (1024)     //采样点数，DownFac前面
#define ADC_SAMPLING_RATE    (20.0f)    //采样率 MHz
#define BANDWIDTH_6G5        (6500000000) //hz
#define BANDWIDTH_3G2        (3220000000) //hz
#define SWEEPING_TIME        (55)       //us
#define CHIRP_TIME           ((20+SWEEPING_TIME+5)*NUM_TX/1e6)
#define FREQ_INIT            (58.5e9)

enum
{
    STANDBY_TIME = 0,
    PROG_FUNC,
    WORK_MODE,
    CFARTHVG,
    CFARTHVS,
    CFARTHV,
    DPKTH,
    NUMTLV,
		MEAN,
		CHIRPGAP,
		BW,
};

#define FLASH_PARAM_HEAD_ADDR		(INFOFLASH_ADDR_BEGIN)  //0x80000
#define FLASH_STANDBY_TIME_ADDR	(FLASH_PARAM_HEAD_ADDR + STANDBY_TIME * 4)
#define FLASH_PROG_FUNC_ADDR		(FLASH_PARAM_HEAD_ADDR + PROG_FUNC * 4)
#define FLASH_WORK_MODE_ADDR    (FLASH_PARAM_HEAD_ADDR + WORK_MODE * 4)
#define FLASH_CFARTHVG_ADDR 		(FLASH_PARAM_HEAD_ADDR + CFARTHVG * 4)
#define FLASH_CFARTHVS_ADDR 		(FLASH_PARAM_HEAD_ADDR + CFARTHVS * 4)
#define FLASH_CFARTHV_ADDR 			(FLASH_PARAM_HEAD_ADDR + CFARTHV * 4)
#define FLASH_DPKTH_ADDR 			  (FLASH_PARAM_HEAD_ADDR + DPKTH * 4)
#define FLASH_NUMTLV_ADDR			  (FLASH_PARAM_HEAD_ADDR + NUMTLV * 4)
#define FLASH_MEAN_ADDR         (FLASH_PARAM_HEAD_ADDR + MEAN * 4)
#define FLASH_CHIRPGAP_ADDR     (FLASH_PARAM_HEAD_ADDR + CHIRPGAP * 4)
#define FLASH_BW_ADDR           (FLASH_PARAM_HEAD_ADDR + BW * 4)

typedef struct
{
	uint8_t WorkMode;
	float RangeRes;
	float VelRes;

	uint32_t Standby_Time;
	uint32_t Proj_Func;
	uint32_t BautRate;
	/*app*/
	uint8_t cfarThVG;
	uint8_t cfarThVS;
  uint8_t cfarThV;
	uint8_t numTLV;
	uint32_t dpkThres;
	uint8_t meanEn;
	
	uint32_t chirpGap;
	/*add by xin @20240618*/
	
	uint8_t bandWidth;
	uint8_t TYPE;
	uint8_t VER_H;
	uint8_t VER_M;
	uint8_t VER_L;
	
	
} R2_STRUCT;

extern R2_STRUCT r2_str;
void ParamConfigInit(void);
void Read_Param_From_Flash(void);

#endif /*_PARAMCONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
