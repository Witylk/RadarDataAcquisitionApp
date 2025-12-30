classdef Processor < handle
    properties
        % 参数
        FS, Rres, Range, WindSize
        frametime, raxis, taxis
        
        % 状态缓存
        data_FFT_old
        fifo_ori
        fifo_mti
        
        % 滤波器
        Hd_breath
        Hd_heart
        
        % 平滑缓存
        hr_buffer
        br_buffer
    end
    
    methods
        function obj = Processor()
            % 从配置加载参数
            cfg = config.Settings;
            obj.FS = cfg.FS;
            obj.Rres = cfg.Rres;
            obj.Range = cfg.Range;
            obj.WindSize = cfg.WindSize;
            
            obj.init();
        end
        
        function init(obj)
            % 初始化逻辑
            obj.frametime = 1 / obj.FS;
            obj.raxis = 0 : obj.Rres : obj.Range*obj.Rres - obj.Rres;
            obj.taxis = 0 : obj.frametime : obj.WindSize*obj.frametime - obj.frametime;
            
            obj.data_FFT_old = zeros(1, obj.Range);
            obj.fifo_ori = zeros(obj.WindSize, obj.Range);
            obj.fifo_mti = zeros(obj.WindSize, obj.Range);
            
            obj.hr_buffer = zeros(1, 10);
            obj.br_buffer = zeros(1, 10);
            
            obj.Hd_breath = core.Filters.createBreathBPF();
            obj.Hd_heart = core.Filters.createHeartBPF();
        end
        
        function updateWindow(obj, newLen)
            obj.WindSize = newLen * obj.FS;
            obj.taxis = 0 : obj.frametime : obj.WindSize*obj.frametime - obj.frametime;
            obj.fifo_ori = zeros(obj.WindSize, obj.Range);
            obj.fifo_mti = zeros(obj.WindSize, obj.Range);
        end
        
        function res = process(obj, input_data)
            % === 1. 数据源智能判断 (修复核心：用长度判断，更稳健) ===
            % 如果输入是 1200 字节（不管是什么类型），说明是串口原始数据
            if numel(input_data) == 1200
                % 强制转为 uint8 再 typecast，确保万无一失
                data_FFT_ori = typecast(uint8(input_data), 'single');
                data_FFT = data_FFT_ori(1:150) + 1i * data_FFT_ori(151:end);
            else
                % 否则认为是回放的已处理数据
                data_FFT = input_data; 
                if size(data_FFT, 1) > size(data_FFT, 2)
                    data_FFT = data_FFT.'; 
                end
            end
            
            % === 2. FIFO 更新 & MTI ===
            % 这里就是报错的地方，上面 data_FFT 修正后，这里就变成 1x150 对 1x150 了
            obj.fifo_ori(1:end-1, :) = obj.fifo_ori(2:end, :);
            obj.fifo_ori(end, :) = data_FFT; 
            
            obj.fifo_mti = abs(obj.fifo_ori - mean(obj.fifo_ori, 1));
            fifo_mti_sum = sum(obj.fifo_mti, 1);
            
            % 限制搜索范围 1:75
            [~, p] = max(fifo_mti_sum(1:75));
            distance = p * obj.Rres;
            
            % === 3. 相位提取 & 滤波 ===
            phase_breath = angle(obj.fifo_ori(:, p))';
            phase_temp = [diff(unwrap(phase_breath)), 0];
            
            phase_breath_filtered = filter(obj.Hd_breath, phase_temp);
            phase_heart_filtered = filter(obj.Hd_heart, phase_temp);
            
            % === 4. 绘图用的距离像 ===
            data_FFT_mti = abs(data_FFT - obj.data_FFT_old);
            obj.data_FFT_old = data_FFT;
            
            % === 5. 全量计算呼吸心率 (保证回放一致性) ===
            full_phase_breath = angle(obj.fifo_ori(:, p))';
            full_phase_temp = [diff(unwrap(full_phase_breath)), 0];
            
            full_breath_filtered = filter(obj.Hd_breath, full_phase_temp);
            full_heart_filtered = filter(obj.Hd_heart, full_phase_temp);
            
            % 呼吸率 (波峰间隔法)
            breath_str = '--';
            [pks, locs] = findpeaks(full_breath_filtered, 'MinPeakDistance', obj.FS * 2);
            if length(locs(pks > 0.10)) >= 2
                valid_locs = locs(pks > 0.10);
                mean_interval = mean(diff(valid_locs)) / obj.FS;
                if mean_interval > 0
                    b_rate = 60 / mean_interval;
                    breath_str = sprintf('%.1f', b_rate);
                else
                    b_rate = 0;
                end
            else
                b_rate = 0;
            end
            
            % 心率 (波峰间隔法)
            heart_str = '--';
            skip = round(2 * obj.FS);
            if length(full_heart_filtered) > skip
                calc_data = full_heart_filtered(skip:end);
                [~, locs_h] = findpeaks(calc_data, 'MinPeakDistance', floor(obj.FS*0.33), 'MinPeakHeight', 0.15);
                if length(locs_h) >= 2
                    hr = 60 / (mean(diff(locs_h)) / obj.FS);
                    if hr >= 40 && hr <= 160
                        heart_str = sprintf('%.0f', hr);
                    else
                        hr = 0;
                    end
                else
                    hr = 0;
                end
            else
                hr = 0;
            end
            
            % 打包结果
            res.distance = distance;
            res.data_FFT = data_FFT;
            res.mti_profile = data_FFT_mti;
            res.breath_wave = full_breath_filtered; 
            res.heart_wave = full_heart_filtered;
            res.breath_str = breath_str;
            res.heart_str = heart_str;
            res.b_rate_val = b_rate; % 方便回放使用
            res.h_rate_val = hr;
        end
    end
end