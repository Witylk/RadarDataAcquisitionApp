#include "RF_SPI_Config.h"
#include "USART_Driver.h"
#include "ParamConfig.h"
//#include "main.h"
STRUCT_PA pa_str;
uint8_t waveStartFlag = 0;
#ifdef RF_RC7701N32
uint16_t RF_ReadRegSPI(uint8_t seqNum, uint8_t u8Addr)
{
    uint16_t u16Data;
    SPI_write_data[0] = (u8Addr & 0x3F) | 0x40;  //sky6605 SPI1 write
    SPI_write_data[1] = 0xFF;                    //send dumy data
    SPI_write_data[2] = 0xFF;                    //send dumy data
    RF_SPI_CS_CTRL(seqNum, 0);                   //拉低
    usci_Spi_TRx(&SPI0, SPI_write_data, 3, SPI_read_data);
    RF_SPI_CS_CTRL(seqNum, 1);                   //拉高
    u16Data = ((uint16_t)SPI_read_data[1] << 8) | SPI_read_data[2];
    printf("addr=%02x,data=%04X\n", u8Addr, u16Data);
    return u16Data;
}
void RF_WriteRegSPI(uint8_t seqNum, uint8_t u8Addr, uint16_t u16Data)
{
    SPI_write_data[0] = (u8Addr & 0x3F);     //sky6605 SPI1 write
    SPI_write_data[1] = u16Data >> 8;
    SPI_write_data[2] = u16Data;
    RF_SPI_CS_CTRL(seqNum, 0);                   //拉低
    usci_Spi_Transmit(&SPI0, SPI_write_data, 3);
    RF_SPI_CS_CTRL(seqNum, 1);                   //拉高
#if 0
    RF_ReadRegSPI(seqNum, u8Addr);
#endif
}
void RF_WriteRegSPI_Fast(uint8_t seqNum, uint8_t u8Addr, uint16_t u16Data)
{
    SPI_write_data[0] = (u8Addr & 0x3F);     //sky6605 SPI1 write
    SPI_write_data[1] = u16Data >> 8;
    SPI_write_data[2] = u16Data;
//    RF_SPI_CS_CTRL(seqNum,0);                    //拉低
    GPIO_PIN_ADDR(0, seqNum) = 0;
    SPI0.instance->TRX = SPI_write_data[0];
    while ((SPI0.instance->ISR & USCI_IS_TXE_Msk) == 0);
    SPI0.instance->TRX = SPI_write_data[1];
    while ((SPI0.instance->ISR & USCI_IS_TXE_Msk) == 0);
    SPI0.instance->TRX = SPI_write_data[2];
    while ((SPI0.instance->ISR & USCI_IS_TXE_Msk) == 0);
    while ((SPI0.instance->ISR & USCI_IS_TE_Msk) == 0);
    GPIO_PIN_ADDR(0, seqNum) = 1;
    SPI0.instance->ISR = USCI_IS_TE_Msk;
//    RF_ReadRegSPI(seqNum,u8Addr);
//    Spi_Transmit_Fast(&SPI0,SPI_write_data,3);

//    RF_SPI_CS_CTRL(seqNum,1);                    //拉高
#if 0
    RF_ReadRegSPI(seqNum, u8Addr);
#endif
}

uint16_t RF_ReadWaveSPI(uint8_t seqNum, uint8_t Row_Addr, uint8_t Col_Addr) {
    uint16_t u16Data;
    SPI_write_data[0] = 0x7E;
    SPI_write_data[1] = (Row_Addr & 0x1F) << 3 | (Col_Addr & 0x07);
    SPI_write_data[2] = 0xFF;
    SPI_write_data[3] = 0xFF;
    RF_SPI_CS_CTRL(seqNum, 0);                   //拉低
    usci_Spi_TRx(&SPI0, SPI_write_data, 4, SPI_read_data);
    RF_SPI_CS_CTRL(seqNum, 1);                   //拉高
    u16Data = ((uint16_t)SPI_read_data[2] << 8) | SPI_read_data[3];
    printf("seq=%d,row=%d,col=%d,data=%04X\n", seqNum, Row_Addr, Col_Addr, u16Data);
    return u16Data;
}
void RF_WriteWaveSPI(uint8_t seqNum, uint8_t Row_Addr, uint8_t Col_Addr, uint16_t u16Data)
{
    SPI_write_data[0] = 0x3E;
    SPI_write_data[1] = (Row_Addr & 0x1F) << 3 | (Col_Addr & 0x07);
    SPI_write_data[2] = u16Data >> 8;
    SPI_write_data[3] = u16Data;
    RF_SPI_CS_CTRL(seqNum, 0);                   //拉低
    usci_Spi_Transmit(&SPI0, SPI_write_data, 4);
    RF_SPI_CS_CTRL(seqNum, 1);                   //拉高
#if 0
    RF_ReadWaveSPI(seqNum, Row_Addr, Col_Addr);
#endif
}
//void RC7701N32_PA_SW(uint8_t paNum)/*{{{*/
//{
//    switch(paNum){
//        case 0:
//            RF_WriteRegSPI(ANA_CFG18_ADDR,0x4000);
//            RF_WriteRegSPI(ANA_CFG19_ADDR,0x4000);
//            break;
//        case 1:
//            RF_WriteRegSPI(ANA_CFG18_ADDR,0x1A15);    //0x5A15 最小，1A15最大
//            RF_WriteRegSPI(ANA_CFG19_ADDR,0x4000);
//            break;
//        case 2:
//                        RF_WriteRegSPI(ANA_CFG19_ADDR,0x1A15);
//            RF_WriteRegSPI(ANA_CFG18_ADDR,0x4000);
//            break;
//    }
//}/*}}}*/

static void RF_WaveSet(uint16_t A1, uint16_t A2, uint16_t B1, uint16_t B2, uint16_t C1, uint16_t C2, uint16_t R, uint16_t V)
{
    RF_WriteWaveSPI(1, 0, 0x07, A1); RF_WriteWaveSPI(1, 0, 0x06, A2);
    RF_WriteWaveSPI(1, 0, 0x05, B1); RF_WriteWaveSPI(1, 0, 0x04, B2);
    RF_WriteWaveSPI(1, 0, 0x03, C1); RF_WriteWaveSPI(1, 0, 0x02, C2);
    RF_WriteWaveSPI(1, 0, 0x01, R); RF_WriteWaveSPI(1, 0, 0x00, V);
}
#if 0 //4Chip
void tmp_regconfig()/*{{{*/
{
//        RF_ReadRegSPI(SYS_STA0_ADDR);
    for (uint8_t i = 1; i < 5; i++) {
        RF_WriteRegSPI(i, ANA_CFG0_ADDR, 0x703E);
        RF_WriteRegSPI(i, ANA_CFG1_ADDR, 0xC28F);
        RF_WriteRegSPI(i, ANA_CFG2_ADDR, 0xD124);
        RF_WriteRegSPI(i, ANA_CFG7_ADDR, 0x5000); //LO band
        RF_WriteRegSPI(i, ANA_CFG8_ADDR, 0x6003);
        RF_WriteRegSPI(i, ANA_CFG9_ADDR, 0x0200); //change bias
        RF_WriteRegSPI(i, ANA_CFG10_ADDR, 0x0533); //1.0V LDO
        RF_WriteRegSPI(i, ANA_CFG11_ADDR, 0x0800); //LNA1
        RF_WriteRegSPI(i, ANA_CFG12_ADDR, 0x0800); //LNA2
        RF_WriteRegSPI(i, ANA_CFG13_ADDR, 0x0800); //LNA3
        RF_WriteRegSPI(i, ANA_CFG14_ADDR, 0x7010); //0xF010 C010 只开RX1   7010 开RX123    A010开RX2
        RF_WriteRegSPI(i, ANA_CFG15_ADDR, 0xB277); //baseband channel 1 fc<5:0> and gc B277   f333  0xF277
        RF_WriteRegSPI(i, ANA_CFG16_ADDR, 0xB277); //baseband channel 2 fc and gc
        RF_WriteRegSPI(i, ANA_CFG17_ADDR, 0xB277); //baseband channel 3 fc and gc
        RF_WriteRegSPI(i, ANA_CFG18_ADDR, 0x1A15);  //0x5A15 最小，1A15最大
        RF_WriteRegSPI(i, ANA_CFG19_ADDR, 0x1A15);  //0x5A15 最小，1A15最大
        RF_WriteRegSPI(i, ANA_CFG20_ADDR, 0x001A); //pa peakdet  //VTEMP/ ATST2 / ATST1  40M:001A    20M:011A  10M:021A
        RF_WriteRegSPI(i, RAMP_SYNCW_CFG0_ADDR, 0xFF00); // SYNC_W1 = 0, SYNC_W2 = 400ns
        RF_WriteRegSPI(i, RAMP_SYNCW_CFG1_ADDR, 0x000F); //  SYNC_W3 = 600ns , SYNC_W4 = 1us
        RF_WriteRegSPI(i, RAMP_ACCW_CFG0_ADDR, 0x00FF); //ACC
        RF_WriteRegSPI(i, RAMP_ACCW_CFG1_ADDR, 0x0000);
        RF_WriteRegSPI(i, RAMP_WM_CFG0_ADDR, 0x3000); //2abc*infinite  need cycle+3 //0x3F  0x3700
        RF_WriteRegSPI(i, RAMP_WM_CFG1_ADDR, 0x0000);
        RF_WriteRegSPI(i, RAMP_PLL_CFG0_ADDR, 0x76A6); //75GHz
        //        RC7701N32_WriteRegSPI(RAMP_PLL_CFG0_ADDR,0x76D8); //76GHz
//            RF_WriteRegSPI(i,RAMP_PLL_CFG0_ADDR,0x770A); //77GHz
        //        RC7701N32_WriteRegSPI(RAMP_PLL_CFG0_ADDR,0x776E); //79GHz
        RF_WriteRegSPI(i, RAMP_PLL_CFG1_ADDR, 0x0000);
    }
    RF_WriteRegSPI(1, ANA_CFG6_ADDR, 0x1D1F); //enable LO  1D1F   1909  1912:T2R2  191F T1T2R1R2R3
    RF_WriteRegSPI(1, ANA_CFG21_ADDR, 0x0001); //40M clk output
    RF_WriteRegSPI(2, ANA_CFG6_ADDR, 0x1E1F); //enable LO  1D1F   1909  1912:T2R2  191F T1T2R1R2R3
    RF_WriteRegSPI(2, ANA_CFG21_ADDR, 0x0000); //40M clk output
    RF_WriteRegSPI(3, ANA_CFG6_ADDR, 0x1E1F); //enable LO  1D1F   1909  1912:T2R2  191F T1T2R1R2R3
    RF_WriteRegSPI(3, ANA_CFG21_ADDR, 0x0000); //40M clk output
    RF_WriteRegSPI(4, ANA_CFG6_ADDR, 0x1A1F); //enable LO  1D1F   1909  1912:T2R2  191F T1T2R1R2R3
    RF_WriteRegSPI(4, ANA_CFG21_ADDR, 0x0000); //40M clk output

//        waveGenClear();
    RF_WaveSet(0x0000, 0x00C8 + 1, 0x02AA, 0x0258, 0x8199, 0x03E8, 0x4401, 0x0124); //A10 B30 1G C50
//        RF_WaveSet(0x0000, 0x012D, 0x02AA,0x0258, 0x80CC, 0x07D0, 0x4401, 0x0124);   //A10 B30 1G C50
    RF_PA_FASTSW(1);
}
#endif
#if 1 //1 CHIP
void tmp_regconfig()/*{{{*/
{
//        RF_ReadRegSPI(SYS_STA0_ADDR);
    RF_WriteRegSPI(1, ANA_CFG0_ADDR, 0x703E);
    RF_WriteRegSPI(1, ANA_CFG1_ADDR, 0xC28F);
    RF_WriteRegSPI(1, ANA_CFG2_ADDR, 0xD124);
    RF_WriteRegSPI(1, ANA_CFG7_ADDR, 0x5000); //LO band
    RF_WriteRegSPI(1, ANA_CFG8_ADDR, 0x6003);
    RF_WriteRegSPI(1, ANA_CFG9_ADDR, 0x0200); //change bias
    RF_WriteRegSPI(1, ANA_CFG10_ADDR, 0x0533); //1.0V LDO
    RF_WriteRegSPI(1, ANA_CFG11_ADDR, 0x0800); //LNA1
    RF_WriteRegSPI(1, ANA_CFG12_ADDR, 0x0800); //LNA2
    RF_WriteRegSPI(1, ANA_CFG13_ADDR, 0x0800); //LNA3
    RF_WriteRegSPI(1, ANA_CFG14_ADDR, 0x7010); //0xF010 C010 只开RX1   7010 开RX123    A010开RX2
    RF_WriteRegSPI(1, ANA_CFG15_ADDR, 0x3333); //baseband channel 1 fc<5:0> and gc B277   f333  0xF277
    RF_WriteRegSPI(1, ANA_CFG16_ADDR, 0x3333); //baseband channel 2 fc and gc
    RF_WriteRegSPI(1, ANA_CFG17_ADDR, 0x3333); //baseband channel 3 fc and gc
    RF_WriteRegSPI(1, ANA_CFG18_ADDR, 0x0000);  //0x5A15 最小，1A15最大
    RF_WriteRegSPI(1, ANA_CFG19_ADDR, 0x0000);  //0x5A15 最小，1A15最大
    RF_WriteRegSPI(1, ANA_CFG20_ADDR, 0x001A); //pa peakdet  //VTEMP/ ATST2 / ATST1  40M:001A    20M:011A  10M:021A
    RF_WriteRegSPI(1, ANA_CFG22_ADDR, 0x000C);
    RF_WriteRegSPI(1, RAMP_SYNCW_CFG0_ADDR, 0xFF00); // SYNC_W1 = 0, SYNC_W2 = 400ns
    RF_WriteRegSPI(1, RAMP_SYNCW_CFG1_ADDR, 0x000F); //  SYNC_W3 = 600ns , SYNC_W4 = 1us
    RF_WriteRegSPI(1, RAMP_ACCW_CFG0_ADDR, 0x00FF); //ACC
    RF_WriteRegSPI(1, RAMP_ACCW_CFG1_ADDR, 0x0000);
    RF_WriteRegSPI(1, RAMP_WM_CFG0_ADDR, 0x3000); //2abc*infinite  need cycle+3 //0x3F  0x3700
    RF_WriteRegSPI(1, RAMP_WM_CFG1_ADDR, 0x0000);
//            RF_WriteRegSPI(1,RAMP_PLL_CFG0_ADDR,0x76A6); //75GHz
    //        RC7701N32_WriteRegSPI(RAMP_PLL_CFG0_ADDR,0x76D8); //76GHz
//            RF_WriteRegSPI(1,RAMP_PLL_CFG0_ADDR,0x770A); //77GHz
    RF_WriteRegSPI(1, RAMP_PLL_CFG0_ADDR, 0x773C); //78GHz
    //        RC7701N32_WriteRegSPI(RAMP_PLL_CFG0_ADDR,0x776E); //79GHz
    RF_WriteRegSPI(1, RAMP_PLL_CFG1_ADDR, 0x0000);
//        }
    RF_WriteRegSPI(1, ANA_CFG6_ADDR, 0x1D1F); //enable LO  1D1F   1909  1912:T2R2  191F T1T2R1R2R3
    RF_WriteRegSPI(1, ANA_CFG21_ADDR, 0x0001); //40M clk output
//        waveGenClear();          //wrong cmd
//        RF_WaveSet(0x0000, 0x012D, 0x02AA, 0x0258, 0x9000, 0x0065, 0x4401, 0x0124);   //A15 B30 1G C5
//        RF_WaveSet(0x0000, 0x00C9, 0x04AA, 0x0258, 0xC600, 0x0029, 0x4401, 0x01FF);   //A10 B30 1.75G C2
//        RF_WaveSet(0x0000, 0x00C9, 0x04AA, 0x0258, 0x82CC, 0x03E8+1, 0x4401, 0x01FF);   //A10 B30 1.75G C50
//        RF_WaveSet(0x0000, 0x00C9, 0x04AA, 0x0258, 0x8047, 0x2710, 0x4401, 0x01FF);   //A10 B30 1.75G C500
//        RF_WaveSet(0x0000, 0x00C8+1, 0x04AE, 0x04B0, 0xB828, 0x0064+1, 0x4401, 0x01FF);   //A10 B60 3.51G C5
    RF_WaveSet(0x0000, 0x00C8 + 1, 0x0255, 0x04B0, 0x9C00, 0x0064 + 1, 0x4401, 0x01FF); //A10 B60 1.75G C5
//        RF_WaveSet(0x0000, 0x03E8+1, 0x0255, 0x04B0, 0x9C00, 0x0064+1, 0x4401, 0x01FF);   //A50 B60 1.75G C5
    RF_PA_FASTSW(2);
}
#endif
void RF_PA_FASTSW(uint8_t paNum) {
    switch (paNum) {
    case 0:
        RF_WriteRegSPI_Fast(CS1_PIN, ANA_CFG18_ADDR, 0x0000);
        RF_WriteRegSPI_Fast(CS1_PIN, ANA_CFG19_ADDR, 0x0000);
        break;
    case 1:
        RF_WriteRegSPI_Fast(CS1_PIN, ANA_CFG18_ADDR, 0x1A15);
        RF_WriteRegSPI_Fast(CS1_PIN, ANA_CFG19_ADDR, 0x0000);
        break;
    case 2:
        RF_WriteRegSPI_Fast(CS1_PIN, ANA_CFG18_ADDR, 0x0000);
        RF_WriteRegSPI_Fast(CS1_PIN, ANA_CFG19_ADDR, 0x1A15);
        break;
    }
}
#endif
#if defined (RF_RC7711B) ||defined (RF_RC6011B)

void RF_PA_FASTSW(uint8_t paNum) {
    switch (paNum) {
    case 0:   //不开PA
        RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG22_ADDR, 0xFE00,1); //channel en
        break;
    case 1:        //PA1
        RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG22_ADDR, 0xFE88,1); //channel en
        break;
    case 2:        //PA2
        RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG22_ADDR, 0xFE44,1); //channel en
        break;
    case 3:   //PA3
        RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG22_ADDR, 0xFE22,1); //channel en
        break;
    case 4:   //PA4
        RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG22_ADDR, 0xFE11,1); //channel en
        break;
    default:
        break;
    }
}
static void RF_WaveSet(uint8_t Row_Addr, uint16_t A1, uint16_t A2, uint16_t B1, uint16_t B2, uint16_t C1, uint16_t C2, uint16_t R, uint16_t V) {
    RF_WriteWaveSPI_Ext(CS1_PIN, Row_Addr, 0x07, A1,1);
    RF_WriteWaveSPI_Ext(CS1_PIN, Row_Addr, 0x06, A2,1);
    RF_WriteWaveSPI_Ext(CS1_PIN, Row_Addr, 0x05, B1,1);
    RF_WriteWaveSPI_Ext(CS1_PIN, Row_Addr, 0x04, B2,1);
    RF_WriteWaveSPI_Ext(CS1_PIN, Row_Addr, 0x03, C1,1);
    RF_WriteWaveSPI_Ext(CS1_PIN, Row_Addr, 0x02, C2,1);
    RF_WriteWaveSPI_Ext(CS1_PIN, Row_Addr, 0x01, R,1);
    RF_WriteWaveSPI_Ext(CS1_PIN, Row_Addr, 0x00, V,1);
}
#endif

//void waveGenClear(void)/*{{{*/
//{
//    RF_WriteCmdSPI(1,0x03);  //waveform generator clear all
//
//}/*}}}*/
void waveGenStop(void)/*{{{*/
{
    RF_WriteCmdSPI_Ext(1, 0x02); //waveform generator start
}/*}}}*/
void waveGenStart(void)/*{{{*/
{
    RF_WriteCmdSPI_Ext(1, 0x00); //waveform generator start
}/*}}}*/


#ifdef RF_RC7711B
void tmp_regconfig(void) {
    RF_WriteRegSPI_Fast(CS1_PIN, ANA_CFG0_ADDR, 0x33C0);
    RF_WriteRegSPI_Fast(CS1_PIN, ANA_CFG1_ADDR, 0xC005);
    RF_WriteRegSPI_Fast(CS1_PIN, ANA_CFG2_ADDR, 0x0001);
    RF_WriteRegSPI_Fast(CS1_PIN, ANA_CFG8_ADDR, 0x091F); //RX baseband channel
    RF_WriteRegSPI_Fast(CS1_PIN, ANA_CFG9_ADDR, 0x007F); //047F:1.5 50 200
    RF_WriteRegSPI_Fast(CS1_PIN, ANA_CFG10_ADDR, 0x1110); //0x1330
    RF_WriteRegSPI_Fast(CS1_PIN, ANA_CFG18_ADDR, 0x4007);
    RF_WriteRegSPI_Fast(CS1_PIN, ANA_CFG19_ADDR, 0x014C);
    RF_WriteRegSPI_Fast(CS1_PIN, ANA_CFG20_ADDR, 0x3200);
    RF_WriteRegSPI_Fast(CS1_PIN, ANA_CFG21_ADDR, 0x93F4); //00c4
    RF_WriteRegSPI_Fast(CS1_PIN, ANA_CFG22_ADDR, 0xFEF0);
    RF_WriteRegSPI_Fast(CS1_PIN, RAMP_SYNCW_CFG0_ADDR, 0xFF00);
    RF_WriteRegSPI_Fast(CS1_PIN, RAMP_SYNCW_CFG1_ADDR, 0x0032);
    RF_WriteRegSPI_Fast(CS1_PIN, RAMP_ACCW_CFG0_ADDR, 0x00FF);
    RF_WriteRegSPI_Fast(CS1_PIN, RAMP_ACCW_CFG1_ADDR, 0x0000);
    RF_WriteRegSPI_Fast(CS1_PIN, RAMP_WM_CFG0_ADDR, 0x3000);
    RF_WriteRegSPI_Fast(CS1_PIN, RAMP_WM_CFG1_ADDR, 0x0401); //0:1个  0100:2个 0200:3个 波形
//    RF_WriteRegSPI_Fast(CS1_PIN,RAMP_PLL_CFG0_ADDR,0x7674);  //初始频率设置   74g
    RF_WriteRegSPI_Fast(CS1_PIN, RAMP_PLL_CFG0_ADDR, 0x76A6); //初始频率设置   75g
//    RC7711B_WriteRegSPI(RAMP_PLL_CFG0_ADDR,0x76D8);  //初始频率设置  76G
//    RF_WriteRegSPI_Fast(CS1_PIN,RAMP_PLL_CFG0_ADDR,0x770A);  //初始频率设置  77G
//    RC7711B_WriteRegSPI(RAMP_PLL_CFG0_ADDR,0x773C);  //初始频率设置  78G
//    RC7711B_WriteRegSPI(RAMP_PLL_CFG0_ADDR,0x776E);  //初始频率设置  79G
    RF_WriteRegSPI_Fast(CS1_PIN, RAMP_PLL_CFG1_ADDR, 0x0000);
//    RF_PA_FASTSW(2);

//RF_WaveSet(0,0x0000,0x0064+1,0x0133,0x0FA0,0xB000,0x0064+1,0x47FF,0x0124);  //no PA


//A12us_B85us_3.62G_C3us
//    RC7711B_WaveSet(0,0x0000,0x00F0+1,0x0368,0x06A4+2,0xE088,0x003C+1,0x4401,0x0124);  //no PA
//    RC7711B_WaveSet(1,0x0000,0x00F0+1,0x0368,0x06A4+2,0xE088,0x003C+1,0x4401,0x1124);  //PA1
//    RC7711B_WaveSet(2,0x0000,0x00F0+1,0x0368,0x06A4+2,0xE088,0x003C+1,0x4401,0x4124);  //PA3
//A15us_B110us_4G_C5us
//    RC7711B_WaveSet(0,0x0000,0x012C+1,0x02E8,0x0898,0xC000,0x0064+1,0x4401,0x21FF);  //no PA
//    RC7711B_WaveSet(1,0x0000,0x012C+1,0x02E8,0x0898,0xC000,0x0064+1,0x4401,0x11FF);  //PA1
//    RC7711B_WaveSet(2,0x0000,0x012C+1,0x02E8,0x0898,0xC000,0x0064+1,0x4401,0x21FF);  //PA2
//    RC7711B_WaveSet(3,0x0000,0x012C+1,0x02E8,0x0898,0xC000,0x0064+1,0x4401,0x41FF);  //PA3
//    RC7711B_WaveSet(4,0x0000,0x012C+1,0x02E8,0x0898,0xC000,0x0064+1,0x4401,0x81FF);  //PA4
//A10us_B60us_1.75G_C5us
    RF_WaveSet(0, 0x0000, 0x00C8 + 1, 0x0255, 0x04B0, 0x9C00, 0x0064 + 1, 0x4401, 0x0124); //no PA
    RF_WaveSet(1, 0x0000, 0x00C8 + 1, 0x0255, 0x04B0, 0x9C00, 0x0064 + 1, 0x4401, 0x1124); //PA1
    RF_WaveSet(2, 0x0000, 0x00C8 + 1, 0x0255, 0x04B0, 0x9C00, 0x0064 + 1, 0x4401, 0x2124); //PA2
    RF_WaveSet(3, 0x0000, 0x00C8 + 1, 0x0255, 0x04B0, 0x9C00, 0x0064 + 1, 0x4401, 0x4124); //PA3
    RF_WaveSet(4, 0x0000, 0x00C8 + 1, 0x0255, 0x04B0, 0x9C00, 0x0064 + 1, 0x4401, 0x8124); //PA4
//A10us_B60us_3.51G_C5us
//    RF_WaveSet(0,0x0000,0x00C8+1,0x04AE,0x04B0,0xB828,0x0064+1,0x4401,0x0124);  //no PA
//    RF_WaveSet(1,0x0000,0x00C8+1,0x04AE,0x04B0,0xB828,0x0064+1,0x4401,0x0124);  //PA1
//    RF_WaveSet(2,0x0000,0x00C8+1,0x04AE,0x04B0,0xB828,0x0064+1,0x4401,0x0124);  //PA2
//    RF_WaveSet(3,0x0000,0x00C8+1,0x04AE,0x04B0,0xB828,0x0064+1,0x4401,0x0124);  //PA3
//    RF_WaveSet(4,0x0000,0x00C8+1,0x04AE,0x04B0,0xB828,0x0064+1,0x4401,0x0124);  //PA4
//A15us_B3276us_4G_C5us
//    RC7711B_WaveSet(0,0x0000,0x012C+1,0x0019,0xFFF0,0xC000,0x0064,0x4401,0x21FF);  //no PA
//    RC7711B_WaveSet(1,0x0000,0x012C+1,0x0019,0xFFF0,0xC000,0x0064+1,0x4401,0x11FF);  //PA1
//    RC7711B_WaveSet(2,0x0000,0x012C+1,0x0019,0xFFF0,0xC000,0x0064+1,0x4401,0x21FF);  //PA2
//    RC7711B_WaveSet(3,0x0000,0x012C+1,0x0019,0xFFF0,0xC000,0x0064+1,0x4401,0x41FF);  //PA3
//    RC7711B_WaveSet(4,0x0000,0x012C+1,0x0019,0xFFF0,0xC000,0x0064+1,0x4401,0x81FF);  //PA4
}
#endif
#ifdef RF_RC6011B
void tmp_regconfig(void) {
#if 0
    while (1) {
        RF_ReadRegSPI(CS1_PIN, SYS_STA0_ADDR);
        Cnt_Delay_ms(500);
    }
#endif

		//    RF_ReadRegSPI(CS1_PIN,0x00);
		RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG0_ADDR, 0x0D40, 1);
		//RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG0_ADDR, (uint16_t)r2_str.tx_power_l+(uint16_t)r2_str.tx_power_h*256, 1);
	RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG1_ADDR, 0xC0C0, 1); //0xC005
    //RF_WriteRegSPI_Fast(CS1_PIN,ANA_CFG2_ADDR,0x0001);
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG8_ADDR, 0x081F, 1); //RX baseband channel
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG9_ADDR, 0x0010, 1); //047F:1.5 50 200  0x087A
		
    if (r2_str.bandWidth == 1)
    {
			RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG10_ADDR, 0x1110, 1); //0x1330
			//RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG10_ADDR,  (uint16_t)r2_str.rx_power_l+(uint16_t)r2_str.rx_power_h*256, 1);
    }
    else
    {
       RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG10_ADDR, 0x1330, 1); //
    }
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG11_ADDR, 0x0000,1);
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG18_ADDR, 0x4007,1); //11:9 vco freq div
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG19_ADDR, 0x5138,1); //0x014C
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG20_ADDR, 0x3703,1);
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG21_ADDR, 0x0000,1); //77G:93F4 60G:0070
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG22_ADDR, 0x9000,1);
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, RAMP_SYNCW_CFG0_ADDR, 0xFF00,1);
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, RAMP_SYNCW_CFG1_ADDR, 0x0032,1);
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, RAMP_ACCW_CFG0_ADDR, 0x00FF,1);
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, RAMP_ACCW_CFG1_ADDR, 0x0000,1);
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, RAMP_WM_CFG0_ADDR, 0x3000,1);
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, RAMP_WM_CFG1_ADDR, 0x0401,1); //0:1个  0100: 2个 0200:3个 波形
//      RF_WriteRegSPI_Fast(CS1_PIN,RAMP_PLL_CFG0_ADDR,0x7322);  //初始频率设置   57g
//      RF_WriteRegSPI_Fast(CS1_PIN,RAMP_PLL_CFG0_ADDR,0x7354);  //初始频率设置   58g
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, RAMP_PLL_CFG0_ADDR, 0x736D,1); //初始频率设置   58.5g
//      RF_WriteRegSPI_Fast(CS1_PIN,RAMP_PLL_CFG0_ADDR,0x7386);  //初始频率设置   59g
//      RF_WriteRegSPI_Fast(CS1_PIN,RAMP_PLL_CFG0_ADDR,0x739F);  //初始频率设置   59.5g
//    RF_WriteRegSPI_Fast(CS1_PIN,RAMP_PLL_CFG0_ADDR,0x73B8);  //初始频率设置   60g
//    RF_WriteRegSPI_Fast(CS1_PIN,RAMP_PLL_CFG0_ADDR,0x73EA);  //初始频率设置   61g
//    RF_WriteRegSPI_Fast(CS1_PIN,RAMP_PLL_CFG0_ADDR,0x741C);  //初始频率设置   62g
//    RF_WriteRegSPI_Fast(CS1_PIN,RAMP_PLL_CFG0_ADDR,0x7480);  //初始频率设置   64g
//    RF_WriteRegSPI_Fast(CS1_PIN,RAMP_PLL_CFG0_ADDR,0x74E4);  //初始频率设置   66g
//    RF_WriteRegSPI_Fast(CS1_PIN,RAMP_PLL_CFG0_ADDR,0x76A6);    //初始频率设置   75g
//    RF_WriteRegSPI_Fast(CS1_PIN,RAMP_PLL_CFG0_ADDR,0x76D8);  //初始频率设置   76g
//    RF_WriteRegSPI_Fast(CS1_PIN,RAMP_PLL_CFG0_ADDR,0x770A);  //初始频率设置   77g
//      RF_WriteRegSPI_Fast(CS1_PIN,RAMP_PLL_CFG0_ADDR,0x773C);  //初始频率设置   78g
//          RF_WriteRegSPI_Fast(CS1_PIN,RAMP_PLL_CFG0_ADDR,0x7755);  //初始频率设置   78.5g
//      RF_WriteRegSPI_Fast(CS1_PIN,RAMP_PLL_CFG0_ADDR,0x776E);  //初始频率设置   79g
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, RAMP_PLL_CFG1_ADDR, 0x0000,1);

//A10us_B60us_3.51G_C5us
//    RF_WaveSet(0,0x0000,0x00C8+1,0x04AE,0x04B0,0xB828,0x0064+1,0x4401,0x2124);  //PA1
////A10us_B55us_6.5G_C5us
//    RF_WaveSet(0,0x0000,0x0190+1,0x0974,0x044C,0xB385,0x0064+1,0x4401,0x0124);  //PA1
//    RF_WaveSet(1,0x0000,0x0190+1,0x0974,0x044C,0xB385,0x0064+1,0x4401,0x2124);  //PA1
//    RF_WaveSet(2,0x0000,0x0190+1,0x0974,0x044C,0xB385,0x0064+1,0x4401,0x1124);  //PA2
//    RF_WaveSet(3,0x0000,0x0190+1,0x0974,0x044C,0xB385,0x0064+1,0x4401,0x4124);  //PA3
//    RF_WaveSet(4,0x0000,0x0190+1,0x0974,0x044C,0xB385,0x0064+1,0x4401,0x8124);  //PA4

    if (r2_str.bandWidth == 1)
    {
        uint16_t time_c = (r2_str.chirpGap - 75) * 20;
////A20us_B55us_6.5G_C time_c us
        RF_WaveSet(0, 0x0000, 0x0190 + 1, 0x0974, 0x044C + 0, 0xB385, time_c + 0, 0x4401, 0x0124); //PA1
        RF_WaveSet(1, 0x0000, 0x0190 + 1, 0x0974, 0x044C + 0, 0xB385, time_c + 0, 0x4401, 0x2124); //PA1
        RF_WaveSet(2, 0x0000, 0x0190 + 1, 0x0974, 0x044C + 0, 0xB385, time_c + 0, 0x4401, 0x1124); //PA2
        RF_WaveSet(3, 0x0000, 0x0190 + 1, 0x0974, 0x044C + 0, 0xB385, time_c + 0, 0x4401, 0x4124); //PA3
        RF_WaveSet(4, 0x0000, 0x0190 + 1, 0x0974, 0x044C + 0, 0xB385, time_c + 0, 0x4401, 0x8124); //PA4
//			        uint16_t time_c = (r2_str.chirpGap - 65) * 20;
//////A10us_B55us_3.22G_C time_c us
//        RF_WaveSet(0, 0x0000, 0x00C8 + 1, 0x04AF, 0x044C + 0, 0xB385, time_c + 0, 0x4401, 0x0124); //PA1
//        RF_WaveSet(1, 0x0000, 0x00C8 + 1, 0x04AF, 0x044C + 0, 0xB385, time_c + 0, 0x4401, 0x2124); //PA1
//        RF_WaveSet(2, 0x0000, 0x00C8 + 1, 0x04AF, 0x044C + 0, 0xB385, time_c + 0, 0x4401, 0x1124); //PA2
//        RF_WaveSet(3, 0x0000, 0x00C8 + 1, 0x04AF, 0x044C + 0, 0xB385, time_c + 0, 0x4401, 0x4124); //PA3
//        RF_WaveSet(4, 0x0000, 0x00C8 + 1, 0x04AF, 0x044C + 0, 0xB385, time_c + 0, 0x4401, 0x8124); //PA4
    }
    else
    {
        uint16_t time_c = (r2_str.chirpGap - 65) * 20;
////A10us_B55us_3.22G_C time_c us
        RF_WaveSet(0, 0x0000, 0x00C8 + 1, 0x04AF, 0x044C + 0, 0xB385, time_c + 0, 0x4401, 0x0124); //PA1
        RF_WaveSet(1, 0x0000, 0x00C8 + 1, 0x04AF, 0x044C + 0, 0xB385, time_c + 0, 0x4401, 0x2124); //PA1
        RF_WaveSet(2, 0x0000, 0x00C8 + 1, 0x04AF, 0x044C + 0, 0xB385, time_c + 0, 0x4401, 0x1124); //PA2
        RF_WaveSet(3, 0x0000, 0x00C8 + 1, 0x04AF, 0x044C + 0, 0xB385, time_c + 0, 0x4401, 0x4124); //PA3
        RF_WaveSet(4, 0x0000, 0x00C8 + 1, 0x04AF, 0x044C + 0, 0xB385, time_c + 0, 0x4401, 0x8124); //PA4
    }

////A10us_B60us_1.75G_C5us
//    RF_WaveSet(0,0x0000,0x00C8+1,0x0255,0x04B0,0x9C00,0x0064+1,0x4401,0x0124);  //PA1
//    RF_WaveSet(1,0x0000,0x00C8+1,0x0255,0x04B0,0x9C00,0x0064+1,0x4401,0x2124);  //PA1
//    RF_WaveSet(2,0x0000,0x00C8+1,0x0255,0x04B0,0x9C00,0x0064+1,0x4401,0x1124);  //PA2
//    RF_WaveSet(3,0x0000,0x00C8+1,0x0255,0x04B0,0x9C00,0x0064+1,0x4401,0x4124);  //PA3
//    RF_WaveSet(4,0x0000,0x00C8+1,0x0255,0x04B0,0x9C00,0x0064+1,0x4401,0x8124);  //PA4
////A10us_B60us_3.51G_C5us
//    RF_WaveSet(0,0x0000,0x00C8+1,0x04AE,0x04B0,0xB828,0x0064+1,0x4401,0x0124);  //PA1
//    RF_WaveSet(1,0x0000,0x00C8+1,0x04AE,0x04B0,0xB828,0x0064+1,0x4401,0x2124);  //PA1
//    RF_WaveSet(2,0x0000,0x00C8+1,0x04AE,0x04B0,0xB828,0x0064+1,0x4401,0x1124);  //PA2
//    RF_WaveSet(3,0x0000,0x00C8+1,0x04AE,0x04B0,0xB828,0x0064+1,0x4401,0x4124);  //PA3
//    RF_WaveSet(4,0x0000,0x00C8+1,0x04AE,0x04B0,0xB828,0x0064+1,0x4401,0x8124);  //PA4
//A10us_B60us_3.51G_C50us
//    RF_WaveSet(0,0x0000,0x00C8+1,0x04AE,0x04B0,0x859d,0x03e8,0x4401,0x0124);  //PA1
//    RF_WaveSet(1,0x0000,0x00C8+1,0x04AE,0x04B0,0x859d,0x03e8,0x4401,0x2124);  //PA1
//    RF_WaveSet(2,0x0000,0x00C8+1,0x04AE,0x04B0,0x859d,0x03e8,0x4401,0x1124);  //PA2
//    RF_WaveSet(3,0x0000,0x00C8+1,0x04AE,0x04B0,0x859d,0x03e8,0x4401,0x4124);  //PA3
//    RF_WaveSet(4,0x0000,0x00C8+1,0x04AE,0x04B0,0x859d,0x03e8,0x4401,0x8124);  //PA4
//A10us_B60us_1.76G_C5us
//    RF_WaveSet(0,0x0000,0x00C8+1,0x0258,0x04B0,0x9c28,0x0064+1,0x4401,0x1124);  //no PA
//    RF_WaveSet(1,0x0000,0x00C8+1,0x0258,0x04B0,0x9c28,0x0064+1,0x4401,0x2124);  //PA1
//    RF_WaveSet(2,0x0000,0x00C8+1,0x0258,0x04B0,0x9c28,0x0064+1,0x4401,0x1124);  //PA2
//    RF_WaveSet(3,0x0000,0x00C8+1,0x0258,0x04B0,0x9c28,0x0064+1,0x4401,0x4124);  //PA3
//    RF_WaveSet(4,0x0000,0x00C8+1,0x0258,0x04B0,0x9c28,0x0064+1,0x4401,0x8124);  //PA4
//    RF_WaveSet(0,0x0000,0x00C8+1,0x04AE,0x04B0,0xB828,0x0064+1,0x4401,0x0124);  //no PA
//    RF_WaveSet(1,0x0000,0x00C8+1,0x04AE,0x04B0,0xB828,0x0064+1,0x4401,0x1124);  //PA1
//    RF_WaveSet(2,0x0000,0x00C8+1,0x04AE,0x04B0,0xB828,0x0064+1,0x4401,0x1124);  //PA2
//    RF_WaveSet(3,0x0000,0x00C8+1,0x04AE,0x04B0,0xB828,0x0064+1,0x4401,0x1124);  //PA3
//    RF_WaveSet(4,0x0000,0x00C8+1,0x04AE,0x04B0,0xB828,0x0064+1,0x4401,0x1124);  //PA4
//    RF_WaveSet(0,0x0000,0x00C8+1,0x04AE,0x04B0,0xB828,0x0064+1,0x4401,0x0124);  //no PA
//    RF_WaveSet(1,0x0000,0x00C8+1,0x04AE,0x04B0,0xB828,0x0064+1,0x4401,0x1124);  //PA1
//    RF_WaveSet(2,0x0000,0x00C8+1,0x04AE,0x04B0,0xB828,0x0064+1,0x4401,0x1124);  //PA2
//    RF_WaveSet(3,0x0000,0x00C8+1,0x04AE,0x04B0,0xB828,0x0064+1,0x4401,0x1124);  //PA3
//    RF_WaveSet(4,0x0000,0x00C8+1,0x04AE,0x04B0,0xB828,0x0064+1,0x4401,0x1124);  //PA4
}
#endif
void RF_CHIPEN(void)
{
    CHPIEN_WritePin = 1;
    Cnt_Delay_ms(1);
    SPI0_CS1_WritePin = 1;
//    SPI0_CS2_WritePin = 1;
//    SPI0_CS3_WritePin = 1;
//    SPI0_CS4_WritePin = 1;
}

void RF_CHIPEN_Disable(void)
{
#if 0
    CHPIEN_WritePin = 0;

    SPI0_CS1_WritePin = 0;
//    SPI0_CS2_WritePin = 0;
//    SPI0_CS3_WritePin = 0;
//    SPI0_CS4_WritePin = 0;
#endif
#if 1
    waveGenStop();
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG0_ADDR, 0,1);
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG1_ADDR, 0,1);
//        RF_WriteRegSPI_Fast(CS1_PIN,ANA_CFG2_ADDR,0x0001);
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG8_ADDR, 0,1); //RX baseband channel
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG9_ADDR, 0,1); //047F:1.5 50 200
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG10_ADDR, 0,1); //0x1330
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG18_ADDR, 0,1);
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG19_ADDR, 0,1);
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG20_ADDR, 0,1);
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG21_ADDR, 0,1); //93F4
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG22_ADDR, 0,1);
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, RAMP_SYNCW_CFG0_ADDR, 0,1);
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, RAMP_SYNCW_CFG1_ADDR, 0,1);
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, RAMP_ACCW_CFG0_ADDR, 0,1);
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, RAMP_ACCW_CFG1_ADDR, 0,1);
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, RAMP_WM_CFG0_ADDR, 0,1);
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, RAMP_WM_CFG1_ADDR, 0,1); //0:1个  0100:2个 0200:3个 波形
#endif
}
void RF_PA_SET(uint8_t sPA_In, STRUCT_PA *sPA_ST)
{
    memset(sPA_ST, 0, sizeof(STRUCT_PA));
    for (uint32_t i = 0; i < 8; i++) {
        if (sPA_In & (1 << (7 - i)))
        {
            sPA_ST->PA_Buf[sPA_ST->num] = i + 1;
            sPA_ST->num++;
        }
    }
//    memset(&sPA_ST->PA_Buf[0],0,8);
//    sPA_ST->PA_Buf[2] = 1;
}
void RF_SPI_Init(void)
{
#if 1
//        RF_PA_SET(PA_SET,&pa_str);
//        RF_CHIPEN();
    tmp_regconfig();
#endif
#if 0
    RF_CHIPEN();
    RF_WriteRegSPI_Fast(CS1_PIN, ANA_CFG2_ADDR, 0x0001);
#endif

//        waveGenStart();
}
void RF_Clock40M_En(void) {
    RF_CHIPEN();
    RF_WriteRegSPI_Fast_Ext(CS1_PIN, ANA_CFG2_ADDR, 0x0001,1);
}
