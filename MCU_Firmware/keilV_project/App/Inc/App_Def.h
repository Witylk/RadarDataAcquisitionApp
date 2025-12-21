/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _APP_DEF_H
#define _APP_DEF_H

#include <stdint.h>
/*
	num of BYTE
	NUM_BYTE16: 0xFFFF,                     have  2 bytes
	NUM_BYTE32: 0xFFFFFFFF,                 have  4 bytes
	NUM_BYTE64: 0xFFFFFFFFFFFFFFFF,         have  8 bytes
*/
#define NUM_BYTE8                   (1)
#define NUM_BYTE16                  (2)
#define NUM_BYTE32                  (4)
#define NUM_BYTE64                  (8)
#define NUM_BYTE192                 (24)

//BB_Defnie
#define NUMLOG_2   					(1ULL)
#define NUMLOG_4   					(2ULL)
#define NUMLOG_8   					(3ULL)
#define NUMLOG_16   				(4ULL)
#define NUMLOG_32   				(5ULL)
#define NUMLOG_64   				(6ULL)
#define NUMLOG_128   				(7ULL)
#define NUMLOG_256   				(8ULL)
#define NUMLOG_512   				(9ULL)
#define NUMLOG_1024   			(10ULL)
#define NUMLOG_2048   			(11ULL)
#define NUMLOG_4096   			(12ULL)
#define NUMLOG_8192   			(13ULL)
#define NUMLOG_16384   			(14ULL)
#define NUMLOG_32768   			(15ULL)

#define PREP_CFG2_JUMPINC_1M                     (1048576)

//#define DOWN_FAC_DIV8
#define DOWN_FAC_DIV2
//#define DOWN_FAC_DIV1



/*Function*/
//#define MPU_OPEN_FLAG

//#define COMPARE_DATA





/*RF config*/
#define PA_SET     (0xF0)

#define NUM_CHIRP 								(64)  //实际第一个chirp没用
#define NUM_CHIRP_QUARTER 				(NUM_CHIRP/4)
#define NUM_CHIRP_QUARTERx3       ((NUM_CHIRP+1)*3)
#define NUM_CHIRP_TX              (NUM_CHIRP*NUM_TX)
#define NUM_CHIRP1_TX             ((NUM_CHIRP+1)*NUM_TX)
#define NUM_RX     								(4)
#define NUM_TX     								((PA_SET>>7)+((PA_SET>>6)&0x01)+((PA_SET>>5)&0x01)+((PA_SET>>4)&0x01)+((PA_SET>>3)&0x01)+((PA_SET>>2)&0x01)+((PA_SET>>1)&0x01)+(PA_SET&0x01))
#define NUM_ANT        						(NUM_RX*NUM_TX)
#define NUM_ANT_H             		(8)
#define NUM_ANT_V                 (3)

#ifdef DOWN_FAC_DIV8
#define NUMPT_LOG                 (NUMLOG_128)
#define ADC_OFFSET_LEN 						(8)
#endif
#ifdef DOWN_FAC_DIV2
#define NUMPT_LOG                 (NUMLOG_512)
#define ADC_OFFSET_LEN 						(32)
#endif
#ifdef DOWN_FAC_DIV1
#define NUMPT_LOG                 (NUMLOG_512)
#define ADC_OFFSET_LEN 						(32)
#endif
#define ADC_MAX_LEN    						(1<<NUMPT_LOG)       			//ADC采样点数


#define USE_RANGE                 (256)
#define NUM_VEL                   (NUM_CHIRP)
#define CFAR_TX                   (2-1)
#define CFAR_RX                   (3-1)
#define CFAR_ANT                  (CFAR_TX*NUM_RX+CFAR_RX)
#define FFT1D_USE_A     					(0)
#define FFT1D_USE_B      					(USE_RANGE-1)
#define FFT2D_USE_A     					((NUM_TX-1)*NUM_VEL+NUM_VEL/2)
#define FFT2D_USE_B      					(NUM_VEL/2-1)
#define CFAR_MAX_NUM              (400)
#define DPK_LENLOG                (NUMLOG_128)
#define NUM_ANGLE                 (1<<DPK_LENLOG)
//#define FFT3D_USE_A               (NUM_ANGLE/2)
//#define FFT3D_USE_B               (NUM_ANGLE/2-1)
#define FFT3D_USE_A               (0)
#define FFT3D_USE_B               (NUM_ANGLE-1)
#define DPK_TIMES                 (2)
#define FFT4D_USE_A               (0)
#define FFT4D_USE_B               (NUM_ANGLE-1)

//#define DPK_MODE_NORMAL
//#define ADC_BIT8
#define ADC_BIT16

#ifdef ADC_BIT8
#define ADC_BYTE         (1)
#endif
#ifdef ADC_BIT16
#define ADC_BYTE         (2)
#endif



#define NUM_LITTLE_CHIRP_BYTES   (ADC_MAX_LEN*ADC_BYTE)  //interInc
#define NUM_BIG_CHIRP_BYTES      (NUM_LITTLE_CHIRP_BYTES*NUM_ANT) //jumpInc

//AT
#define ChirpGapMin           (80)      //80us
#define ChirpGapMax           (3200)    //3.2ms
#define ProtocolTimeMax       (60000)   //*8ms
#define ProtocolProgFuncMin   (01)
#define ProtocolProgFuncMax   (03)
#define ProtocolWorkModeMin   (00)
#define ProtocolWorkModeMax   (02)
#define ProtocolRangeMax      (FFT_RANGE_MAX_LEN)
#define ProtocolCFARTHMin     (1)
#define ProtocolCFARTHMax     (99)
#define ProtocolDPKTHMax      (99)
#define ProtocolENMax         (1)
#define ProtocolNumTlvsMin    (1)
#define ProtocolNumTlvsMax    (3)

#define IMPORT_MAX_LEN     (1024*1024)
#define IMPORT_ADDR        (0x30000000)

//#define PROTOCOL_CFAR
#define PROTOCOL_DPK

#define FRAME_MAX_TIME   (00)  //ms



typedef struct{
	uint16_t d1Idx;
	uint8_t d2Idx;
	uint8_t d3Idx;
	uint8_t d4Idx;
	uint32_t powABS;
}STRUCT_FRAMEPOINT;

typedef struct{
	uint16_t targetNum;
	STRUCT_FRAMEPOINT target[CFAR_MAX_NUM*DPK_TIMES];
}STRUCT_FRAMERST;

/********************************Addr*********************************/
//WBUF
#define WIN_RANGE_ADDR                         (BB_WBUF_BASE) //512*2=1024Byte
#define WIN_VEL1_ADDR 	                       (WIN_RANGE_ADDR+ADC_MAX_LEN*NUM_BYTE16) //64*2=128Byte
#define WIN_VEL2_ADDR 	                       (WIN_VEL1_ADDR+NUM_CHIRP*NUM_BYTE16)    //64*2=128Byte
#define WIN_VEL3_ADDR 	                       (WIN_VEL2_ADDR+NUM_CHIRP*NUM_BYTE16)    //64*2=128Byte
#define WIN_VEL4_ADDR 	                       (WIN_VEL3_ADDR+NUM_CHIRP*NUM_BYTE16)    //64*2=128Byte
#define SINCBUF_PF_ADDR                        (WIN_VEL4_ADDR+NUM_CHIRP*NUM_BYTE16)    //128*4=512Byte
#define STEERING_VEC_PF_ADDR                   (SINCBUF_PF_ADDR+NUM_ANGLE*NUM_BYTE32)  //128*4*4=2048Byte
#define ZEROS_ADDR                             (STEERING_VEC_PF_ADDR + 128*4*4)

//ABUF0
#define CALIB_AP_PF_ADDR                       (BB_ABUF0_BASE)
#define ADC_SAMPLE_ADDR                        (BB_ABUF0_BASE) //512*6*2 = 6KB@1chirp
#define FFT1D_CACHE_ADDR                       (ADC_SAMPLE_ADDR)  //ADC_SAMPLE_ADDR       256*16*64*4=1MByte
#define FFT2D_CACHE_ADDR                       (FFT1D_CACHE_ADDR)  //256*16*64*4=1MByte
//ABUF1

//ABUF2

//ABUF3

//ABUF4
#define FFT1D_MEAN_ADDR                        (BB_ABUF4_BASE)  //256*16*4=16K
#define CFAR_CACHE_ADDR                        (FFT1D_MEAN_ADDR+USE_RANGE*NUM_ANT*NUM_BYTE32)  //400*8=3.2K
#define DPK_DST_FIRST_ADDR                     (CFAR_CACHE_ADDR+sizeof(BB_CFARRST_T)*CFAR_MAX_NUM) //24Byte
#define DPK_DST_ADDR_MINUS                     (DPK_DST_FIRST_ADDR+NUM_BYTE192) //24*(1+1)
#define FRAME_RST_ADDR                         (DPK_DST_ADDR_MINUS+NUM_BYTE192*(DPK_TIMES+1)) //14K
#define PROTOCOL_PACK_ADDR                     (FRAME_RST_ADDR+sizeof(STRUCT_FRAMERST))
	
#define FFT2D_PF_SUM_ADDR                      (BB_ABUF4_BASE + 64 * 1024)

//DBUF0
#define FFT2D_ABS_SUM_ADDR                     (BB_DBUF0_BASE)
#define FFT3D_CACHE_ADDR                       (BB_DBUF0_BASE)  //128*4=512Byte
#define DPK_SRCA_ADDR                          (FFT3D_CACHE_ADDR) //256Byte
#define FFT4D_CACHE_ADDR                       (BB_DBUF0_BASE)  //128*4=512Byte
//DBUF1
#define P1MUL_CACHE_ADDR                       (BB_DBUF1_BASE)
#define DPK_SRCB_ADDR                          (BB_DBUF1_BASE) //256Byte
#define FFT4D_PRE_ADDR                         (BB_DBUF1_BASE)//3*4=12Byte

//CPX_ADDR
#define FFT2D_PF_SUM_CPX_ADDR                  (BB_ABUF4_CPX_BASE+(FFT2D_PF_SUM_ADDR-BB_ABUF4_BASE)*2)
#define CALIB_AP_CPX_ADDR                      (BB_ABUF0_CPX_BASE)
#define SINCBUF_CPX_ADDR                       (BB_WBUF_CPX_BASE+(SINCBUF_PF_ADDR-BB_WBUF_BASE)*2)
#define STEERING_VEC_CPX_ADDR                  (BB_WBUF_CPX_BASE+(STEERING_VEC_PF_ADDR-BB_WBUF_BASE)*2)
//#define STEERING_VEC_CPX_ADDR                  (BB_WBUF_CPX_BASE)

//ABS_ADDR
#define FFT2D_SUM_ABS_ADDR                     (FFT2D_PF_SUM_ADDR+0x400000)
#endif

