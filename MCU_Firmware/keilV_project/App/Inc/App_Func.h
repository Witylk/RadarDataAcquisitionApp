/* Define to prevent recursive inclusion -------------------------------------*/
#include "App_Def.h"

#ifndef _APP_FUNC_H
#define _APP_FUNC_H




/*Local*/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
/*Driver*/
#include "IRQ_Handler.h"
#include "OSPI_Driver.h"
/*User*/
#include "BB_Config.h"
#include "ParamConfig.h"








void Func_Init(void);
void Func01_Handle(void);
void Func02_Handle(void);
void Func03_Handle(void);
void Func04_Handle(void);





#endif

