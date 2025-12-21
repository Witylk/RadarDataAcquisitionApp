Release Data: 2023/10/19
匹配 SKY32B750_EVB3 RC7711C-AIP_4T4R_V1.5 版本硬件
version :SKY32B750_PeopleTrack_V2.2.8.16

Release Data: 2023/9/20
将在单根天线上做CFAR改为在16根天线ABS加和上做CFAR
CFAR配置参数修改为：cfarThVG=08，cfarThVS=08，cfarThV=02
version :SKY32B750_PeopleTrack_V2.2.8.13

Release Data: 2023/7/20
扫频带宽配置成6.5G
增加AT+MEAN指令内容：
·AT+MEAN=0\n，表示不减均值;
·AT+MEAN=1\n，表示减本帧均值;
·AT+MEAN=2\n，表示减上一帧均值;
增加AT+GAP指令，可以控制Chirp间隔，单位是us，最小80us，最大3200us；
·AT+GAP=80\n
增加AT+BW指令，可以控制扫频带宽
·AT+BW=0\n，扫3.22G
·AT+BW=1\n，扫6.5G
version :SKY32B750_PeopleTrack_V2.2.8.11

Release Data: 2023/4/17
将RF的高通配置成400,500，和7711B保持一致
version :SKY32B750_PeopleTrack_U_V2.2.7.10

Release Data: 2023/4/7
去掉RF_SPI_Config.c内CFG1配置，修复dataprep卡死问题
IRQ内增加PEDGE中断，关闭FADC clock，防止数据采集异常
version :SKY32B750_PeopleTrack_U_V2.2.7.9
Release Data: 2023/3/21
增加减均值使能开关
发送命令：
AT+MEAN\n
更新优化低功耗，目前50ms帧间隔对应功耗200mA左右
增加看门狗
version :SKY32B750_PeopleTrack_U_V2.2.5.9


Release Data: 2022/10/28
增加USB3.0对应OSPI配置程序
version :SKY32B750_PeopleTrack_U_V2.2.5.8

Release Data: 2022/9/23
增加GPIO3.0用于切换CHIPEN管脚
version : 2.2.5.7

Release Data: 2022/8/30
更新版本号，包括芯片版本，硬件版本，lib库版本，软件版本
version : 2.2.4.6


Release Data: 2022/8/30
bb dataprep chirp数+1
关掉ramp IMR
数据采集起始地址减掉NUM_ANT*ADC_BYTE*ADC_MAX_LEN

version : 1.0.3.5

Release Data: 2022/8/26
dpkThres改为15，cfar保护带改为2
兼容黑色射频板，修改adc采集通道和中频反向
CFAR GO改SO
version : 1.0.3.4


Release Data: 2022/6/16
Release Description:
更改BB_Config.c
bb_prep_st->cfg0.adc_clk_mode = PREP_CFG0_ADC_CLK_MODE_N;
更改main.c
SYS_CLK_PLL_Init(); //切PLL

保证数据采集相位对齐
version : 1.0.3.3

Release Data: 2022/6/6
Release Description:
update dma and timerA driver
update DMA_Driver.c
version : 1.0.3.2

Release Date: 2022/5/18
Release Description:
change paramConfig.c printf version
version : 1.0.2.1

Release Date: 2022/5/16
Release Description: Initial release
version : 1.0.1.0
