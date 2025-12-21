/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _GPIO_DRIVER_H
#define _GPIO_DRIVER_H
#include "usci.h"
#include <stdio.h>
/*
EXTGPIO[0]AD2      GPIO0.0                                                          QSPI0_SS         OSPI0_SS
EXTGPIO[1]AF7      GPIO0.1              USCI0_IO0                     SPI_CLK       QSPI0_CLK        OSPI0_CLK
EXTGPIO[2]AE7      GPIO0.2                    USCI0_IO1        UART_TX      SPI_MOSI      QSPI0_DIO0       OSPI0_DIO0
EXTGPIO[3]P9       GPIO0.3                    USCI0_IO2              UART_RX      SPI_MISO      QSPI0_DIO1       OSPI0_DIO1
EXTGPIO[4]AD1      GPIO0.4                                                          QSPI0_DIO2       OSPI0_DIO2
EXTGPIO[5]AH1      GPIO0.5                    USCI1_IO0                     SPI_CLK       QSPI0_DIO3       OSPI0_DIO3
EXTGPIO[6]AF2      GPIO0.6              USCI1_IO1                 UART_TX        SPI_MOSI                                         OSPI0_DIO4
EXTGPIO[7]AF5      GPIO0.7              USCI1_IO2        UART_RX      SPI_MISO                       OSPI0_DIO5
EXTGPIO[8]AJ1         GPIO0.8                                                          QSPI1_SS         OSPI0_DIO6
EXTGPIO[9]AJ6         GPIO0.9                    USCI2_IO0                     SPI_CLK       QSPI1_CLK        OSPI0_DIO7
EXTGPIO[10]AJ5       GPIO0.10             USCI2_IO1                 UART_TX      SPI_MOSI      QSPI1_DIO0       OSPI1_SS
EXTGPIO[11]AH2       GPIO0.11             USCI2_IO2        UART_RX      SPI_MISO      QSPI1_DIO1       OSPI1_CLK
EXTGPIO[12]AC7     GPIO0.12                                                         QSPI1_DIO2       OSPI1_DIO0
EXTGPIO[13]AC6     GPIO0.13                    USCI3_IO0                     SPI_CLK       QSPI1_DIO3       OSPI1_DIO1
EXTGPIO[14]N9           GPIO0.14(EINT0)    USCI3_IO1        UART_TX      SPI_MOSI                       OSPI1_DIO2
EXTGPIO[15]N8           GPIO0.15(EINT1)    USCI3_IO2        UART_RX      SPI_MISO                       OSPI1_DIO3                     ANA_CFG1[22] 0:0.15  1:1.0
EXTGPIO[16]M10     GPIO1.0                                                          QSPI0_SS         OSPI1_DIO4      RAMSYNC_PAD1
EXTGPIO[17]L10         GPIO1.1                    USCI4_IO0                     SPI_CLK       QSPI0_CLK        OSPI1_DIO5
EXTGPIO[18]AC8     GPIO1.2              USCI4_IO1                 UART_TX      SPI_MOSI      QSPI0_DIO0       OSPI1_DIO6
EXTGPIO[19]P11     GPIO1.3              USCI4_IO2        UART_RX      SPI_MISO      QSPI0_DIO1       OSPI1_DIO7
EXTGPIO[20]N11     GPIO1.4                                                          QSPI0_DIO2
EXTGPIO[21]L16     GPIO1.5                    USCI5_IO0                     SPI_CLK       QSPI0_DIO3
EXTGPIO[22]K16     GPIO1.6              USCI5_IO1        UART_TX      SPI_MOSI
EXTGPIO[23]L15     GPIO1.7              USCI5_IO2        UART_RX      SPI_MISO
EXTGPIO[24]K15     GPIO1.8                                                          QSPI1_SS
EXTGPIO[25]Y12     GPIO1.9                    USCI6_IO0                     SPI_CLK       QSPI1_CLK
EXTGPIO[26]AA12    GPIO1.10             USCI6_IO1        UART_TX      SPI_MOSI      QSPI1_DIO0
EXTGPIO[27]N13     GPIO1.11             USCI6_IO2        UART_RX      SPI_MISO      QSPI1_DIO1
EXTGPIO[28]M13     GPIO1.12                                                         QSPI1_DIO2
EXTGPIO[29]M15     GPIO1.13                    USCI7_IO0                     SPI_CLK       QSPI1_DIO3
EXTGPIO[30]M14     GPIO1.14(EINT2)    USCI7_IO1        UART_TX      SPI_MOSI
EXTGPIO[31]M11     GPIO1.15(EINT3)    USCI7_IO2        UART_RX      SPI_MISO
EXTGPIO[32]L11     GPIO2.0                                                          QSPI0_SS           ADC0_INP
EXTGPIO[33]U9      GPIO2.1                    USCI0_IO0                     SPI_CLK       QSPI0_CLK                 ADC0_INN
EXTGPIO[34]U8      GPIO2.2              USCI0_IO1        UART_TX      SPI_MOSI      QSPI0_DIO0             ADC1_INP
EXTGPIO[35]V8      GPIO2.3              USCI0_IO2        UART_RX      SPI_MISO      QSPI0_DIO1         ADC1_INN
EXTGPIO[36]V7      GPIO2.4                                                          QSPI0_DIO2             ADC2_INP
EXTGPIO[37]V12     GPIO2.5                    USCI1_IO0                     SPI_CLK       QSPI0_DIO3         ADC2_INN
EXTGPIO[38]V11     GPIO2.6              USCI1_IO1        UART_TX      SPI_MOSI                         ADC3_INP
EXTGPIO[39]Y10     GPIO2.7              USCI1_IO2        UART_RX      SPI_MISO                         ADC3_INN
EXTGPIO[40]Y9      GPIO2.8                                                          QSPI1_SS                 ADC4_INP
EXTGPIO[41]L13     GPIO2.9                    USCI2_IO0                     SPI_CLK       QSPI1_CLK          ADC4_INN
EXTGPIO[42]K13     GPIO2.10             USCI2_IO1        UART_TX      SPI_MOSI      QSPI1_DIO0         ADC5_INP
EXTGPIO[43]P12     GPIO2.11             USCI2_IO2        UART_RX      SPI_MISO      QSPI1_DIO1         ADC5_INN
EXTGPIO[44]N12     GPIO2.12                                                         QSPI1_DIO2         ADC6_INP
EXTGPIO[45]L12     GPIO2.13                    USCI3_IO0                     SPI_CLK       QSPI1_DIO3         ADC6_INN
EXTGPIO[46]K12     GPIO2.14(EINT4)    USCI3_IO1        UART_TX      SPI_MOSI                         ADC7_INP
EXTGPIO[47]T7      GPIO2.15(EINT5)    USCI3_IO2        UART_RX      SPI_MISO                         ADC7_INN
EXTGPIO[48]T6      GPIO3.0                                                          QSPI0_SS                       RAMSYNC_PAD0
EXTGPIO[49]V6      GPIO3.1                    USCI4_IO0                     SPI_CLK       QSPI0_CLK
EXTGPIO[50]U6      GPIO3.2              USCI4_IO1        UART_TX      SPI_MOSI      QSPI0_DIO0
EXTGPIO[51]U11     GPIO3.3              USCI4_IO2        UART_RX      SPI_MISO      QSPI0_DIO1
EXTGPIO[52]T11     GPIO3.4                                                          QSPI0_DIO2
EXTGPIO[53]AL12    GPIO3.5                    USCI5_IO0                     SPI_CLK       QSPI0_DIO3
EXTGPIO[54]AH14    GPIO3.6              USCI5_IO1        UART_TX      SPI_MOSI
EXTGPIO[55]AH13    GPIO3.7              USCI5_IO2        UART_RX      SPI_MISO
EXTGPIO[56]AJ15    GPIO3.8                                                          QSPI1_SS
EXTGPIO[57]AJ14    GPIO3.9                    USCI6_IO0                     SPI_CLK       QSPI1_CLK
EXTGPIO[58]AE13    GPIO3.10             USCI6_IO1        UART_TX      SPI_MOSI      QSPI1_DIO0
EXTGPIO[59]AF13    GPIO3.11             USCI6_IO2        UART_RX      SPI_MISO      QSPI1_DIO1
EXTGPIO[60]AG14    GPIO3.12                                                         QSPI1_DIO2
EXTGPIO[61]AG15    GPIO3.13                    USCI7_IO0                     SPI_CLK       QSPI1_DIO3
EXTGPIO[62]AE14    GPIO3.14             USCI7_IO1        UART_TX      SPI_MOSI                      SWD_DIO
EXTGPIO[63]AE15    GPIO3.15             USCI7_IO2        UART_RX      SPI_MISO                      SWD_CLK

*/
//GPIO
#define CHIPEN_Pin GPIO_PIN_0
#define CHIPEN_GPIO_Port GPIO3
#define CHPIEN_WritePin GPIO3_0

#define SPI0_CS1_Pin GPIO_PIN_10
#define SPI0_CS1_GPIO_Port GPIO0
#define SPI0_CS1_WritePin GPIO0_10

#define SPI0_CS2_Pin GPIO_PIN_11
#define SPI0_CS2_GPIO_Port GPIO0
#define SPI0_CS2_WritePin GPIO0_11

#define SPI0_CS3_Pin GPIO_PIN_12
#define SPI0_CS3_GPIO_Port GPIO0
#define SPI0_CS3_WritePin GPIO0_12

#define SPI0_CS4_Pin GPIO_PIN_1
#define SPI0_CS4_GPIO_Port GPIO1
#define SPI0_CS4_WritePin GPIO1_1

#define CS1_PIN       0
#define CS2_PIN       1
#define CS3_PIN       2
#define CS4_PIN       3

//test
//#define TEST_Pin GPIO_PIN_0
//#define TEST_GPIO_Port GPIO0
//#define TEST_WritePin GPIO0_0 //



#define GPIO_BSRR_BS0_Pos              (0U)
#define GPIO_BSRR_BS0_Msk              (0x1UL << GPIO_BSRR_BS0_Pos)            /*!< 0x00000001 */
#define GPIO_BSRR_BS0                  GPIO_BSRR_BS0_Msk
#define GPIO_BSRR_BS1_Pos              (1U)
#define GPIO_BSRR_BS1_Msk              (0x1UL << GPIO_BSRR_BS1_Pos)            /*!< 0x00000002 */
#define GPIO_BSRR_BS1                  GPIO_BSRR_BS1_Msk
#define GPIO_BSRR_BS2_Pos              (2U)
#define GPIO_BSRR_BS2_Msk              (0x1UL << GPIO_BSRR_BS2_Pos)            /*!< 0x00000004 */
#define GPIO_BSRR_BS2                  GPIO_BSRR_BS2_Msk
#define GPIO_BSRR_BS3_Pos              (3U)
#define GPIO_BSRR_BS3_Msk              (0x1UL << GPIO_BSRR_BS3_Pos)            /*!< 0x00000008 */
#define GPIO_BSRR_BS3                  GPIO_BSRR_BS3_Msk
#define GPIO_BSRR_BS4_Pos              (4U)
#define GPIO_BSRR_BS4_Msk              (0x1UL << GPIO_BSRR_BS4_Pos)            /*!< 0x00000010 */
#define GPIO_BSRR_BS4                  GPIO_BSRR_BS4_Msk
#define GPIO_BSRR_BS5_Pos              (5U)
#define GPIO_BSRR_BS5_Msk              (0x1UL << GPIO_BSRR_BS5_Pos)            /*!< 0x00000020 */
#define GPIO_BSRR_BS5                  GPIO_BSRR_BS5_Msk
#define GPIO_BSRR_BS6_Pos              (6U)
#define GPIO_BSRR_BS6_Msk              (0x1UL << GPIO_BSRR_BS6_Pos)            /*!< 0x00000040 */
#define GPIO_BSRR_BS6                  GPIO_BSRR_BS6_Msk
#define GPIO_BSRR_BS7_Pos              (7U)
#define GPIO_BSRR_BS7_Msk              (0x1UL << GPIO_BSRR_BS7_Pos)            /*!< 0x00000080 */
#define GPIO_BSRR_BS7                  GPIO_BSRR_BS7_Msk
#define GPIO_BSRR_BS8_Pos              (8U)
#define GPIO_BSRR_BS8_Msk              (0x1UL << GPIO_BSRR_BS8_Pos)            /*!< 0x00000100 */
#define GPIO_BSRR_BS8                  GPIO_BSRR_BS8_Msk
#define GPIO_BSRR_BS9_Pos              (9U)
#define GPIO_BSRR_BS9_Msk              (0x1UL << GPIO_BSRR_BS9_Pos)            /*!< 0x00000200 */
#define GPIO_BSRR_BS9                  GPIO_BSRR_BS9_Msk
#define GPIO_BSRR_BS10_Pos             (10U)
#define GPIO_BSRR_BS10_Msk             (0x1UL << GPIO_BSRR_BS10_Pos)           /*!< 0x00000400 */
#define GPIO_BSRR_BS10                 GPIO_BSRR_BS10_Msk
#define GPIO_BSRR_BS11_Pos             (11U)
#define GPIO_BSRR_BS11_Msk             (0x1UL << GPIO_BSRR_BS11_Pos)           /*!< 0x00000800 */
#define GPIO_BSRR_BS11                 GPIO_BSRR_BS11_Msk
#define GPIO_BSRR_BS12_Pos             (12U)
#define GPIO_BSRR_BS12_Msk             (0x1UL << GPIO_BSRR_BS12_Pos)           /*!< 0x00001000 */
#define GPIO_BSRR_BS12                 GPIO_BSRR_BS12_Msk
#define GPIO_BSRR_BS13_Pos             (13U)
#define GPIO_BSRR_BS13_Msk             (0x1UL << GPIO_BSRR_BS13_Pos)           /*!< 0x00002000 */
#define GPIO_BSRR_BS13                 GPIO_BSRR_BS13_Msk
#define GPIO_BSRR_BS14_Pos             (14U)
#define GPIO_BSRR_BS14_Msk             (0x1UL << GPIO_BSRR_BS14_Pos)           /*!< 0x00004000 */
#define GPIO_BSRR_BS14                 GPIO_BSRR_BS14_Msk
#define GPIO_BSRR_BS15_Pos             (15U)
#define GPIO_BSRR_BS15_Msk             (0x1UL << GPIO_BSRR_BS15_Pos)           /*!< 0x00008000 */
#define GPIO_BSRR_BS15                 GPIO_BSRR_BS15_Msk

void GPIO_Init(void);


#endif /*_GPIO_DRIVER_H */

/************************ (C) COPYRIGHT  *****END OF FILE****/























