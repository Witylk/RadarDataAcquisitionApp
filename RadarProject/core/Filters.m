classdef Filters
    methods (Static)
        % 创建呼吸带通滤波器 (0.1-0.6 Hz)
        function Hd = createBreathBPF()
            Fs = 10;
            N = 8;
            Fc1 = 0.1;
            Fc2 = 0.6;
            h = fdesign.bandpass('N,F3dB1,F3dB2', N, Fc1, Fc2, Fs);
            Hd = design(h, 'butter');
        end

        % 创建心跳带通滤波器 (0.8-2.5 Hz)
        % 注意：使用了你提供代码中最后定义的参数 Fc2=2.5
        function Hd = createHeartBPF()
            Fs = 10;
            N = 8;
            Fc1 = 0.8;  % 心跳频率下限：48 bpm
            Fc2 = 2.5;  % 心跳频率上限：150 bpm
            h = fdesign.bandpass('N,F3dB1,F3dB2', N, Fc1, Fc2, Fs);
            Hd = design(h, 'butter');
        end
    end
end