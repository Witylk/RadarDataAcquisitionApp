classdef Settings
    properties (Constant)
        % === 雷达参数 (完全保留原代码数值) ===
        FS = 10             % 采样频率
        FFTNum = 512        % FFT点数
        Rres = 0.05         % 距离分辨率
        WindSize = 300      % 窗口大小
        Range = 150         % 距离范围
        
        % === 串口默认参数 ===
        DefaultPort = 'COM7'
        DefaultBaud = '921600'
    end
end