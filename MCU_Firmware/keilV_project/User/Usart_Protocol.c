#include "Usart_Protocol.h"
#include "main.h"
#include <complex.h>
#include <stdlib.h>

STRUCT_UDATA udata;

//static unsigned char HexToChar(unsigned char bChar)
//{
//	if((bChar>=0x30)&&(bChar<=0x39))
//	{
//		bChar -= 0x30;
//	}
//	else if((bChar>=0x41)&&(bChar<=0x46)) // Capital
//	{
//		bChar -= 0x37;
//	}
//	else if((bChar>=0x61)&&(bChar<=0x66)) //littlecase
//	{
//		bChar -= 0x57;
//	}
//	else
//	{
//		bChar = 0xff;
//	}
//	return bChar;
//}



int calculateSlope(float arr[], int n) {
    float differenceSum = 0.0;
    int count = 0;
    int Risesign = 0;
    for (int i = 1; i < n; i++) {
        float difference = arr[i] - arr[i - 1];

        if (difference > 1.0|| arr[i]< arr[i - 1]) {
            continue;  // 跳过巨大差值的数字
        }
        count++;
        differenceSum += difference;
    }
    if (count >= 5 && differenceSum > 0.5)Risesign = 1;
    return Risesign;

}


char AsciiToDec(unsigned char * pAscii, unsigned char * pDec, int nLen)
{
	unsigned char Nibble[2] = {0};
	int i = 0;
	int j = 0;

	if (nLen % 2)
	{
		Nibble[0] = *pAscii ++;
		if (Nibble[0] >= '0' && Nibble[0] <= '9')
		{
			pDec[0] = Nibble[0] - '0';
		}
		else
			return 1;
		for (i = 0; i < (nLen - 1) / 2; i ++)
		{
			Nibble[0] = *pAscii ++;
			Nibble[1] = *pAscii ++;
			for (j = 0; j < 2; j ++)
			{
				if (Nibble[j] >= '0' && Nibble[j] <= '9')
					Nibble [j] = Nibble[j] - '0';
				else
					return 1;

			}
			pDec[i + 1] = Nibble[0] * 10;
			pDec[i + 1] += Nibble[1];
		}
	}
	else
	{
		for (i = 0; i < nLen / 2; i ++)
		{
			Nibble[0] = *pAscii ++;
			Nibble[1] = *pAscii ++;
			for (j = 0; j < 2; j ++)
			{
				if (Nibble[j] >= '0' && Nibble[j] <= '9')
					Nibble [j] = Nibble[j] - '0';
				else
					return 1;

			}
			pDec[i] = Nibble[0] * 10;
			pDec[i] += Nibble[1];
		}
	}
	return 0;
}

void Word2Byte(uint32_t w_val, uint8_t* b_val)
{
	b_val[0] = w_val & 0xFF;
	b_val[1] = (w_val >> 8) & 0xFF;
	b_val[2] = (w_val >> 16) & 0xFF;
	b_val[3] = (w_val >> 24) & 0xFF;
}

static char Protocol_Analysis_Handle(void)
{
	uint8_t AtoH[10] = {0};
	uint8_t wtbuf[16] = {0};
	uint8_t systick_flag = 0;
	if(strUsart.RecFinish==1)
	{
        RTC_DISABLE;
		if(GET_SYSTICK_STATE == 1)
		{
			systick_flag = 1;
			SYSTICK_DISABLE;
		}
		memset(&udata,0,sizeof(udata));
		strUsart.RecLen--;     //È¥µô\n
		//RF
		if(memcmp(strUsart.RecBuff,"AT",AT_LEN)==0&&(strUsart.RecLen==AT_LEN)){
			printf("AT+OK\n");

		}
		else if(memcmp(strUsart.RecBuff,"AT+START",AT_START_LEN)==0&&(strUsart.RecLen==AT_START_LEN)){
			r2_str.WorkMode = 1;
		}
		else if(memcmp(strUsart.RecBuff,"AT+RESET",AT_RESET_LEN)==0&&(strUsart.RecLen==AT_RESET_LEN)){
//			NVIC_SystemReset();
			WDT_RstNow();
			while(1);
		}
		else if(memcmp(strUsart.RecBuff,"AT+IMPORT",AT_IMPORT_LEN)==0){
			printf("AT+OK\n");
			strUsart.writeCnt = 0;
			r2_str.WorkMode = 2;
		}
		else if(memcmp(strUsart.RecBuff, "AT+TIME=", 8) == 0 && (strUsart.RecLen - 8) > 0 && (strUsart.RecLen - 8) < 6)
		{
			if (AsciiToDec(strUsart.RecBuff + 8, AtoH, strUsart.RecLen - 8) == 0)
			{
				if ((strUsart.RecLen - 8) == 1 || (strUsart.RecLen - 8) == 2)  r2_str.Standby_Time = AtoH[0];
				if ((strUsart.RecLen - 8) == 3 || (strUsart.RecLen - 8) == 4)  r2_str.Standby_Time = AtoH[0] * 100 + AtoH[1];		//100-9999
				if ((strUsart.RecLen - 8) == 5) 	r2_str.Standby_Time = AtoH[0] * 10000 + AtoH[1] * 100 + AtoH[2];	//10000-99999
				if (r2_str.Standby_Time <= ProtocolTimeMax)
				{
					Word2Byte(r2_str.Standby_Time, wtbuf);
					if(EFC_ReWrite(FLASH_STANDBY_TIME_ADDR, 4, wtbuf) == OK)
					{
						r2_str.Standby_Time = Read_M32(FLASH_STANDBY_TIME_ADDR);
						printf("AT+TIME=%d\r\n", r2_str.Standby_Time);
					}
					
				}
			}
		}
		else if (memcmp(strUsart.RecBuff, "AT+PROG=", 8) == 0 && (strUsart.RecLen - 8) == 2)
		{
			if (AsciiToDec(strUsart.RecBuff + 8, AtoH, strUsart.RecLen - 8) == 0)
			{
				r2_str.Proj_Func = AtoH[0];
				if (r2_str.Proj_Func >= ProtocolProgFuncMin && r2_str.Proj_Func <= ProtocolProgFuncMax)
				{
					Word2Byte(r2_str.Proj_Func, wtbuf);
					if(EFC_ReWrite(FLASH_PROG_FUNC_ADDR, 4, wtbuf) == OK)
					{
						r2_str.Proj_Func = Read_M32(FLASH_PROG_FUNC_ADDR);
						printf("AT+Proj=%02d\r\n", r2_str.Proj_Func);
					}

				}
			}
		}
		else if (memcmp(strUsart.RecBuff, "AT+CFARTHV=", 11) == 0 && (strUsart.RecLen - 11) > 0 && (strUsart.RecLen - 11) < 3)
		{
			if (AsciiToDec(strUsart.RecBuff + 11, AtoH, strUsart.RecLen - 11) == 0)
			{
				r2_str.cfarThV = AtoH[0];
				if (r2_str.cfarThV > ProtocolCFARTHMin && r2_str.cfarThV < ProtocolCFARTHMax)
				{
					Word2Byte(r2_str.cfarThV, wtbuf);
					if(EFC_ReWrite(FLASH_CFARTHV_ADDR, 4, wtbuf) == OK)
					{
						r2_str.cfarThV = Read_M32(FLASH_CFARTHV_ADDR);
						printf("AT+cfarTHV=%d\r\n", r2_str.cfarThV);
					}

				}
			}
		}
		else if (memcmp(strUsart.RecBuff, "AT+CFARTHVG=", 12) == 0 && (strUsart.RecLen - 12) > 0 && (strUsart.RecLen - 12) < 3)
		{
			if (AsciiToDec(strUsart.RecBuff + 12, AtoH, strUsart.RecLen - 12) == 0)
			{
				r2_str.cfarThVG = AtoH[0];
				if (r2_str.cfarThVG < ProtocolCFARTHMax)
				{
					Word2Byte(r2_str.cfarThVG, wtbuf);
					if(EFC_ReWrite(FLASH_CFARTHVG_ADDR, 4, wtbuf) == OK)
					{
						r2_str.cfarThVG = Read_M32(FLASH_CFARTHVG_ADDR);
						printf("AT+cfarTHVG=%d\r\n", r2_str.cfarThVG);
					}

				}
			}
		}
		else if (memcmp(strUsart.RecBuff, "AT+CFARTHVS=", 12) == 0 && (strUsart.RecLen - 12) > 0 && (strUsart.RecLen - 12) < 3)
		{
			if (AsciiToDec(strUsart.RecBuff + 12, AtoH, strUsart.RecLen - 12) == 0)
			{
				r2_str.cfarThVS = AtoH[0];
				if (r2_str.cfarThVS < ProtocolCFARTHMax)
				{
					Word2Byte(r2_str.cfarThVS, wtbuf);
					if(EFC_ReWrite(FLASH_CFARTHVS_ADDR, 4, wtbuf) == OK)
					{
						r2_str.cfarThVS = Read_M32(FLASH_CFARTHVS_ADDR);
						printf("AT+cfarTHVS=%d\r\n", r2_str.cfarThVS);
					}

				}
			}
		}
		else if (memcmp(strUsart.RecBuff, "AT+DPKTH=", 9) == 0 && (strUsart.RecLen - 9) > 0 && (strUsart.RecLen - 9) < 3)
		{
			if (AsciiToDec(strUsart.RecBuff + 9, AtoH, strUsart.RecLen - 9) == 0)
			{
				r2_str.dpkThres = AtoH[0];
				if (r2_str.dpkThres <= ProtocolDPKTHMax)
				{
					Word2Byte(r2_str.dpkThres, wtbuf);
					if(EFC_ReWrite(FLASH_DPKTH_ADDR, 4, wtbuf) == OK)
					{
						r2_str.dpkThres = Read_M32(FLASH_DPKTH_ADDR);
						printf("AT+dpkTHres=%d\r\n", r2_str.dpkThres);
					}

				}
			}
		}
		else if (memcmp(strUsart.RecBuff, "AT+NUMTLV=", 10) == 0 && (strUsart.RecLen - 10) > 0 && (strUsart.RecLen - 10) < 3)
		{
			if (AsciiToDec(strUsart.RecBuff + 10, AtoH, strUsart.RecLen - 10) == 0)
			{
				r2_str.numTLV = AtoH[0];
				if (r2_str.numTLV >= ProtocolNumTlvsMin && r2_str.numTLV <= ProtocolNumTlvsMax)
				{
					Word2Byte(r2_str.numTLV, wtbuf);
					if(EFC_ReWrite(FLASH_NUMTLV_ADDR, 4, wtbuf) == OK)
					{
						r2_str.numTLV = Read_M32(FLASH_NUMTLV_ADDR);
						printf("AT+numTLV=%d\r\n", r2_str.numTLV);
					}

				}
			}
		}
		else if (memcmp(strUsart.RecBuff, "AT+MEAN=", 8) == 0 && (strUsart.RecLen - 8) > 0 && (strUsart.RecLen - 8) < 3)
		{
			if (AsciiToDec(strUsart.RecBuff + 8, AtoH, strUsart.RecLen - 8) == 0)
			{
				r2_str.meanEn = AtoH[0];
				if (r2_str.meanEn >= 0 && r2_str.meanEn <= 10)
				{
					Word2Byte(r2_str.meanEn, wtbuf);
					if(EFC_ReWrite(FLASH_MEAN_ADDR, 4, wtbuf) == OK)
					{
						r2_str.meanEn = Read_M32(FLASH_MEAN_ADDR);
						printf("AT+MEAN=%02d\r\n", r2_str.meanEn);
					}

				}
			}
		}
		else if(memcmp(strUsart.RecBuff, "AT+GAP=", 7) == 0 && (strUsart.RecLen - 7) > 0 && (strUsart.RecLen - 7) < 6)
		{
			if (AsciiToDec(strUsart.RecBuff + 7, AtoH, strUsart.RecLen - 7) == 0)
			{
				if ((strUsart.RecLen - 7) == 1 || (strUsart.RecLen - 7) == 2)  r2_str.chirpGap = AtoH[0];
				if ((strUsart.RecLen - 7) == 3 || (strUsart.RecLen - 7) == 4)  r2_str.chirpGap = AtoH[0] * 100 + AtoH[1];		//100-9999
				if ((strUsart.RecLen - 7) == 5) 	r2_str.chirpGap = AtoH[0] * 10000 + AtoH[1] * 100 + AtoH[2];	//10000-99999
				if ((r2_str.chirpGap >= ChirpGapMin) && (r2_str.chirpGap <= ChirpGapMax))
				{
					Word2Byte(r2_str.chirpGap, wtbuf);
					if(EFC_ReWrite(FLASH_CHIRPGAP_ADDR, 4, wtbuf) == OK)
					{
						r2_str.chirpGap = Read_M32(FLASH_CHIRPGAP_ADDR);
						printf("AT+GAP=%d\r\n", r2_str.chirpGap);
					}

				}
			}
		}
		else if (memcmp(strUsart.RecBuff, "AT+BW=", 6) == 0 && (strUsart.RecLen - 6) > 0 && (strUsart.RecLen - 6) < 3)
		{
			if (AsciiToDec(strUsart.RecBuff + 6, AtoH, strUsart.RecLen - 6) == 0)
			{
				r2_str.bandWidth = AtoH[0];
				if (r2_str.bandWidth >= 0 && r2_str.bandWidth <= 10)
				{
					Word2Byte(r2_str.bandWidth, wtbuf);
					if(EFC_ReWrite(FLASH_BW_ADDR, 4, wtbuf) == OK)
					{
						r2_str.bandWidth = Read_M32(FLASH_BW_ADDR);
						printf("AT+BW=%02d\r\n", r2_str.bandWidth);
					}

				}
			}
		}		
		else{ //false
			USART_Transmit_Bytes(&USART0,strUsart.RecBuff,strUsart.RecLen);
		}
		strUsart.RecLen = 0;
		strUsart.RecFinish = 0;
		if(systick_flag == 1)
		{
			SYSTICK_ENABLE;
		}
        RTC_Enable(SOURCE_RC32K);
	}
	return 0;
}


void RecProtocol_Loop(void)
{
	if(strUsart.timeoutStart){
		strUsart.timeoutCnt++;
		if(strUsart.timeoutCnt>USART_MAX_TIMEOUT_NUM){
			strUsart.timeoutCnt = 0;
			strUsart.timeoutStart = 0;
			strUsart.RecFinish = 1;
		}
	}
	Protocol_Analysis_Handle();
}


/*UsartD-¨°¨¦:
Head:(2B)
		55AA
Length:(4B)
		²»°üº¬HeadºÍLength
Time:(2B)
    ms
numTLVs:(1B)
		01£ºpoint
		02£ºtrack
		03£ºpoint+track
type:(1B)
		01:point
		02:track
TargetNum:(2B)
TLV:(8B/16B)
	type=01:d1Idx(2B)+d2Idx(1B)+d3Idx(1B)+d4Idx(1B)+powABS(4B)
	type=02:ID(1B)+X(4B)+Y(4B)
*/

uint32_t ProtocolSendPack(STRUCT_FRAMERST *sPtRst,uint8_t *sBuff,uint16_t sTime)
{
	uint32_t sCnt = 0;
	uint32_t sLen = 0;
	sBuff[0]=0x55;
	sBuff[1]=0xAA;
	sCnt=sCnt+2;
	switch (r2_str.numTLV)
	{
		case 1: //µãÔÆ
			sLen = sPtRst->targetNum*9+6;  //6=numTLV(1B)+type(1B)+targetnum(2B)+timeInterval(2B)  9=d1Idx(2B)+d2Idx(1B)+d3Idx(1B)+d4Idx(1B)+powABS(4B)

			#ifdef MPU_OPEN_FLAG
				memcpy(sBuff+sCnt,&sLen,4);
				sCnt=sCnt+4;
			#else
				sBuff[sCnt]=sLen&0xFF;
				sCnt++;
				sBuff[sCnt]=(sLen>>8)&0xFF;
				sCnt++;
				sBuff[sCnt]=(sLen>>16)&0xFF;
				sCnt++;
				sBuff[sCnt]=(sLen>>24)&0xFF;
				sCnt++;
			#endif
			sBuff[sCnt]= sTime&0xFF;
			sBuff[sCnt+1]= (sTime>>8)&0xFF;
			sCnt=sCnt+2; //frame interval time
			sBuff[sCnt] = (uint8_t)r2_str.numTLV;
			sCnt=sCnt+1;
			sBuff[sCnt] = 1; //type=1
			sCnt=sCnt+1;
			#ifdef MPU_OPEN_FLAG
				memcpy(sBuff+sCnt,&sPtRst->targetNum,2);
				sCnt=sCnt+2;
			#else
				sBuff[sCnt]=(sPtRst->targetNum)&0xFF;
				sCnt++;
				sBuff[sCnt]=(sPtRst->targetNum>>8)&0xFF;
				sCnt++;
			#endif
			for(uint32_t i=0;i<sPtRst->targetNum;i++)
			{
				#ifdef MPU_OPEN_FLAG
					memcpy(sBuff+sCnt,&sPtRst->target[i].d1Idx,2);
					sCnt=sCnt+2;
				#else
					sBuff[sCnt]=(sPtRst->target[i].d1Idx)&0xFF;
					sCnt++;
					sBuff[sCnt]=(sPtRst->target[i].d1Idx>>8)&0xFF;
					sCnt++;
				#endif
				sBuff[sCnt]=sPtRst->target[i].d2Idx;
				sCnt++;
				sBuff[sCnt]=sPtRst->target[i].d3Idx;
				sCnt++;
				sBuff[sCnt]=sPtRst->target[i].d4Idx;
				sCnt++;

				#ifdef MPU_OPEN_FLAG
					memcpy(sBuff+sCnt,&sPtRst->target[i].powABS,4);
					sCnt=sCnt+4;
				#else
					sBuff[sCnt]=sPtRst->target[i].powABS&0xFF;
					sCnt++;
					sBuff[sCnt]=(sPtRst->target[i].powABS>>8)&0xFF;
					sCnt++;
					sBuff[sCnt]=(sPtRst->target[i].powABS>>16)&0xFF;
					sCnt++;
					sBuff[sCnt]=(sPtRst->target[i].powABS>>24)&0xFF;
					sCnt++;
				#endif
			}
			sLen = sLen+6; //6=Head(2B)+Len(4B)
	}
	return sLen;
}








