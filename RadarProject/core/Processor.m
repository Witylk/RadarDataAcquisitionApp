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
            % 初始化逻辑 (原 initializeParameters)
            obj.frametime = 1 / obj.FS;
            obj.raxis = 0 : obj.Rres : obj.Range*obj.Rres - obj.Rres;
            obj.taxis = 0 : obj.frametime : obj.WindSize*obj.frametime - obj.frametime;
            
            obj.data_FFT_old = zeros(1, obj.Range);
            obj.fifo_ori = zeros(obj.WindSize, obj.Range);
            obj.fifo_mti = zeros(obj.WindSize, obj.Range);
            
            obj.Hd_breath = core.Filters.createBreathBPF();
            obj.Hd_heart = core.Filters.createHeartBPF();
        end
        
        function updateWindow(obj, newLen)
            obj.WindSize = newLen * obj.FS;
            obj.taxis = 0 : obj.frametime : obj.WindSize*obj.frametime - obj.frametime;
            obj.fifo_ori = zeros(obj.WindSize, obj.Range);
            obj.fifo_mti = zeros(obj.WindSize, obj.Range);
        end
        
        function res = process(obj, byte_FFT)
            % === 以下逻辑完全复制自原 acquireData ===
            
            % 1. 数据转换
            data_FFT_ori = typecast(uint8(byte_FFT), 'single');
            data_FFT = data_FFT_ori(1:150) + 1i * data_FFT_ori(151:end);
            
            % 2. FIFO 更新 & MTI
            obj.fifo_ori(1:end-1, :) = obj.fifo_ori(2:end, :);
            obj.fifo_ori(end, :) = data_FFT';
            
            obj.fifo_mti = abs(obj.fifo_ori - mean(obj.fifo_ori, 1));
            fifo_mti_sum = sum(obj.fifo_mti, 1);
            
            % 【关键】保留原代码的 1:75 限制
            [~, p] = max(fifo_mti_sum(1:75));
            
            distance = p * obj.Rres;
            
            % 3. 相位提取
            phase_breath = angle(obj.fifo_ori(:, p))';
            phase_temp = [diff(unwrap(phase_breath)), 0];
            
            % 4. 滤波
            phase_breath_filtered = filter(obj.Hd_breath, phase_temp);
            phase_heart_filtered = filter(obj.Hd_heart, phase_temp);
            
            % 5. MTI 距离像 (用于绘图)
            data_FFT_mti = abs(data_FFT - obj.data_FFT_old);
            obj.data_FFT_old = data_FFT;
            
            % 6. 计算呼吸率 (逻辑完全保留)
            breath_rate = 0; 
            breath_str = '--';
            if length(phase_breath_filtered) > obj.FS * 2 % 简单防错
                 % 注意：这里实际上需要传入完整历史数据进行 findpeaks，
                 % 为了保持无状态，我们假设 phase_breath_filtered 已经是当前窗口数据
                 % 在 RunSystem 里我们会把整个 fifo 的滤波结果传进来吗？
                 % 原代码 logic 是：process 每一帧，但 calculate 用的是 recent_phase
                 % 这里为了MVC，我们只返回波形，计算在外部或传入完整波形。
                 % 为了不改逻辑，我们把计算放回 process，但 process 需要知道它是在处理整个 buffer
            end
            
            % 为了严格保证原代码逻辑，计算部分由 Controller 传入 accumulation 后的数据进行计算
            % 或者，Processor 内部维护的 fifo_ori 已经足够提取整段相位了。
            
            % 重复提取整段相位用于计算 (和原代码一致)
            % 原代码: recent_phase = app.savedData.phase_data(idx,:);
            % 这里直接用 fifo 里的数据重算一遍全量滤波，保证一致性
            full_phase_breath = angle(obj.fifo_ori(:, p))';
            full_phase_temp = [diff(unwrap(full_phase_breath)), 0];
            
            % 对整个窗口进行滤波，用于计算频率
            full_breath_filtered = filter(obj.Hd_breath, full_phase_temp);
            full_heart_filtered = filter(obj.Hd_heart, full_phase_temp);
            
            % --- 呼吸率计算 ---
            [pks, locs] = findpeaks(full_breath_filtered, 'MinPeakDistance', obj.FS * 2);
            valid_idx = pks > 0.10;
            valid_locs = locs(valid_idx);

            % 这可以实现 0.1 的精度，因为它计算的是平均周期
            if length(valid_locs) >= 2
                % 计算相邻波峰之间的距离（点数）
                intervals_points = diff(valid_locs);
                
                % 转换为时间间隔（秒）
                intervals_sec = intervals_points / obj.FS;
                
                % 求平均间隔
                mean_interval = mean(intervals_sec);
                
                % 算出频率 (60 / 周期)
                if mean_interval > 0
                    breath_rate = 60 / mean_interval;
                    breath_str = sprintf('%.1f', breath_rate);
                end
            end
            
            % --- 心率计算 ---
            heart_str = '--';
            skip_sec = 2;
            start_idx = round(obj.FS * skip_sec);
            if length(full_heart_filtered) > start_idx
                calc_data = full_heart_filtered(start_idx:end);
            else
                calc_data = full_heart_filtered;
            end
            
            [pks_heart, locs_heart] = findpeaks(calc_data, ...
                'MinPeakDistance', floor(obj.FS * 0.33), ...
                'MinPeakHeight', 0.18);
            
            if length(locs_heart) >= 2
                mean_interval = mean(diff(locs_heart)) / obj.FS;
                hr = 60 / mean_interval;
                if hr >= 40 && hr <= 160
                    heart_str = sprintf('%.0f', hr);
                end
            end

            % 打包结果
            res.distance = distance;
            res.data_FFT = data_FFT;
            res.mti_profile = data_FFT_mti;
            % 返回单帧滤波结果用于绘图累加，或者返回整段？
            % 原代码绘图 plot(..., phase_breath_filtered) 是画整个窗口
            res.breath_wave = full_breath_filtered; 
            res.heart_wave = full_heart_filtered;
            res.breath_str = breath_str;
            res.heart_str = heart_str;
        end
    end
end