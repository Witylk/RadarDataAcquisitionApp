# 基于 FMCW 雷达的非接触式生命体征监测系统 

本项目实现了一套**非接触式（Non-contact）**的生理信号监测系统。通过毫米波雷达采集人体胸腔微动信号，利用 MATLAB 上位机进行实时信号处理，成功分离出**呼吸（Respiration）**和**心跳（Heartbeat）**波形，并实时计算呼吸率（RPM）和心率（BPM）。

## 1. 核心功能与算法 (Core Features)

本项目不仅仅是数据采集，核心在于对雷达回波的**相位（Phase）**进行深度处理：

* **❤️ 心跳监测 (Heart Rate Monitoring)**
    * **原理**：提取雷达中频信号的相位变化，检测由心脏跳动引起的微米级胸壁位移。
    * **处理**：应用带通滤波器（0.8Hz - 2.0Hz）分离心跳分量。
    * **输出**：实时心跳波形图 + 心率数值 (**BPM** - Beats Per Minute)。

* **🫁 呼吸监测 (Respiration Monitoring)**
    * **原理**：检测由呼吸引起的胸腔起伏（毫米级）。
    * **处理**：应用带通滤波器（0.1Hz - 0.5Hz）分离呼吸分量。
    * **输出**：实时呼吸波形图 + 呼吸率数值 (**RPM** - Respirations Per Minute)。

* **⚡ 实时信号处理流程**
    1. **DC 去除**：消除静态杂波干扰。
    2. **相位提取 (Phase Extraction)**：从 FFT 峰值点提取相位信息。
    3. **相位解缠 (Phase Unwrapping)**：解决相位突变问题，还原真实位移曲线。
    4. **数字滤波 (Digital Filtering)**：设计 IIR/FIR 滤波器分离呼吸和心跳频段。

## 2. 系统架构 (System Architecture)

### 下位机 (KeilV)
* **工程路径**：`MCU_Firmware/`
* **功能**：控制雷达前端，通过定时器触发 ADC 高速采样，利用 DMA 将原始 I/Q 信号或中频信号通过串口透传至上位机。
* **关键技术**：ADC + DMA + High Speed UART。

### 上位机 (MATLAB App Designer)
* **源码路径**：`PC_Software/RadarDataAcquisitionApp.m`
* **功能**：串口接收 -> 缓冲队列 -> 复杂算法解算 -> 界面波形绘制。

## 3. 快速开始 (Quick Start)

### 硬件准备与烧录
1.  将 `Tools/` 目录下的 **`KY32B750_Flash.flm`** 复制到 Keil 安装目录的 Flash 文件夹中（必做！）。
2.  使用 J-Link/ST-Link 连接开发板，打开 `MCU_Firmware/Jupiter.uvprojx` 编译并下载。

### 软件运行
1.  连接开发板 USB 到电脑。
2.  运行 MATLAB，打开 `PC_Software/RadarDataAcquisitionApp.m`。
3.  设置串口（与连接设备串口保持一致），点击 **打开串口** 后点击 **开始采集** 。
4.  **重要操作**：
    * 请将雷达正对胸口（距离 0.5m 左右效果最佳）。
    * 保持静止（避免身体大幅晃动干扰微弱的心跳信号）。
    * 等待几秒钟，波形稳定后即可看到心跳和呼吸曲线。

## 4. 运行效果展示
### 1. MATLAB代码功能实现（当把开发板放在胸腔0.5m左右时且测量稳定时）

![生命体征监测界面 - 稳定测量时](Assets/demo_picture1.png)

### 2. MATLAB代码功能实现（当把雷达放进盒子且测量稳定时)

![生命体征监测界面 - 稳定测量时](Assets/demo_picture2.png)

代码更新后（将原代码分割成几个子文件，便于调节参数），视频展示如下:
<video width="100%" controls>
  <source src="https://github.com/Witylk/RadarDataAcquisitionApp/blob/main/Assets/demo_video.mp4">
</video>

