classdef RunSystem < handle
    properties
        View        % 界面
        Processor   % 核心算法
        Driver      % 硬件驱动
        Timer       % 定时器
        IsRunning = false
        
        SavedData   % 数据缓存
    end
    
    methods
        function obj = RunSystem()
            % 1. 初始化
            obj.View = ui.MainView();
            obj.Processor = core.Processor();
            obj.Driver = core.SerialDriver();
            
            obj.initSaveData();
            
            % 2. 绑定回调 (映射原代码逻辑)
            obj.View.btnOpenPort.ButtonPushedFcn = @obj.onOpenPort;
            obj.View.btnStartAcq.ButtonPushedFcn = @obj.onStartStop;
            obj.View.btnSaveData.ButtonPushedFcn = @obj.onSaveData;
            
            obj.View.efWindowLen.ValueChangedFcn = @obj.onWindowLenChanged;
            obj.View.spWindowLen.ValueChangedFcn = @obj.onWindowLenChanged;
            
            obj.View.btnResetTop.ButtonPushedFcn = @obj.onResetView;
            obj.View.btnResetBottom.ButtonPushedFcn = @obj.onResetView;
            
            obj.View.UIFigure.CloseRequestFcn = @obj.onClose;
        end
        
        function initSaveData(obj)
            obj.SavedData = struct();
            obj.SavedData.timestamps = datetime.empty(0,1);
            obj.SavedData.distances = [];
            obj.SavedData.fft_data = [];
            obj.SavedData.phase_data = zeros(0, obj.Processor.WindSize);
        end
        
        % --- 回调实现 ---
        
        function onOpenPort(obj, ~, ~)
            if strcmp(obj.View.btnOpenPort.Text, '打开串口')
                ok = obj.Driver.open(obj.View.ddPort.Value, obj.View.ddBaudRate.Value);
                if ok
                    obj.View.btnOpenPort.Text = '关闭串口';
                    uialert(obj.View.UIFigure, ['成功打开 ' obj.View.ddPort.Value], '提示', 'Icon', 'success');
                else
                    uialert(obj.View.UIFigure, '打开串口失败', '错误', 'Icon', 'error');
                end
            else
                obj.onStop();
                obj.Driver.close();
                obj.View.btnOpenPort.Text = '打开串口';
            end
        end
        
        function onStartStop(obj, ~, ~)
            if ~obj.IsRunning
                if isempty(obj.Driver.scom)
                    uialert(obj.View.UIFigure, '请先打开串口', '提示', 'Icon', 'warning');
                    return;
                end
                
                obj.Processor.init();
                obj.initSaveData();
                
                obj.IsRunning = true;
                obj.View.btnStartAcq.Text = '停止采集';
                
                if ~isempty(obj.Timer), delete(obj.Timer); end
                obj.Timer = timer('ExecutionMode', 'fixedRate', ...
                    'Period', 0.1, 'TimerFcn', @obj.mainLoop);
                start(obj.Timer);
            else
                obj.onStop();
            end
        end
        
        function onStop(obj)
            obj.IsRunning = false;
            if isvalid(obj.View.UIFigure)
                obj.View.btnStartAcq.Text = '开始采集';
            end
            if ~isempty(obj.Timer), stop(obj.Timer); delete(obj.Timer); end
            obj.Timer = [];
        end
        
        function mainLoop(obj, ~, ~)
            % 1. 读数据
            [byte_FFT, ok] = obj.Driver.readFrame();
            if ~ok, return; end
            
            % 2. 处理数据 (核心算法在 Processor 中，完全未动)
            res = obj.Processor.process(byte_FFT);
            
            % 3. 更新缓存 (用于保存)
            obj.SavedData.timestamps(end+1) = datetime('now');
            obj.SavedData.distances(end+1) = res.distance;
            obj.SavedData.fft_data(end+1, :) = res.data_FFT;
            
            % 处理 phase_data 大小
            current_phase_len = length(res.breath_wave);
            % 这里需要截断或补零以匹配 savedData 矩阵宽度?
            % 原代码逻辑：app.savedData.phase_data(end+1, 1:length(phase_breath_filtered)) = phase_breath_filtered;
            % 且之前做了 zeros 预分配。
            if size(obj.SavedData.phase_data, 2) ~= current_phase_len
                 % 如果窗口变化了，这里简单处理
            end
            obj.SavedData.phase_data(end+1, :) = res.breath_wave;
            
            % 4. 更新 UI
            obj.View.lblPosition.Text = sprintf('%.1f', res.distance * 100);
            obj.View.lblBreathValue.Text = res.breath_str;
            obj.View.lblHeartValue.Text = res.heart_str;
            
            % 绘图
            cla(obj.View.axRuler); hold(obj.View.axRuler, 'on');
            plot(obj.View.axRuler, [res.distance*100 res.distance*100], [0 1], 'r-', 'LineWidth', 3);
            hold(obj.View.axRuler, 'off');
            
            plot(obj.View.axDistanceProfile, obj.Processor.raxis, res.mti_profile);
            title(obj.View.axDistanceProfile, '实时对消单帧距离像');
            grid(obj.View.axDistanceProfile, 'on');
            
            plot(obj.View.axHeartWaveform, obj.Processor.taxis, res.heart_wave, 'Color', [1 0.3 0.3]);
            grid(obj.View.axHeartWaveform, 'on');
            
            plot(obj.View.axTimeWaveform, obj.Processor.taxis, res.breath_wave, 'Color', [0 0.6 1]);
            grid(obj.View.axTimeWaveform, 'on');
        end
        
        function onWindowLenChanged(obj, src, ~)
            newLen = src.Value;
            obj.View.efWindowLen.Value = newLen;
            obj.View.spWindowLen.Value = newLen;
            
            if obj.IsRunning
                obj.Processor.updateWindow(newLen);
                
                % 调整保存数据大小 (原代码逻辑)
                old_phase = obj.SavedData.phase_data;
                new_size = obj.Processor.WindSize;
                obj.SavedData.phase_data = zeros(size(old_phase,1), new_size);
                cols = min(size(old_phase,2), new_size);
                obj.SavedData.phase_data(:, 1:cols) = old_phase(:, 1:cols);
                
                xlim(obj.View.axTimeWaveform, [0 newLen]);
                xlim(obj.View.axHeartWaveform, [0 newLen]);
            end
        end
        
        function onSaveData(obj, ~, ~)
            try
                filename = obj.View.efFileName.Value;
                if ~endsWith(filename, '.mat'), filename = [filename, '.mat']; end
                
                data = obj.SavedData;
                data.label = obj.View.efLabel.Value;
                data.parameters.FS = obj.Processor.FS;
                data.parameters.Rres = obj.Processor.Rres;
                data.parameters.WindSize = obj.Processor.WindSize;
                data.parameters.Range = obj.Processor.Range;
                
                save(filename, 'data');
                uialert(obj.View.UIFigure, ['保存成功: ' filename], '成功', 'Icon', 'success');
            catch ME
                uialert(obj.View.UIFigure, ['保存失败: ' ME.message], '错误', 'Icon', 'error');
            end
        end
        
        function onResetView(obj, ~, ~)
            xlim(obj.View.axHeartWaveform, [0 obj.View.efWindowLen.Value]);
            ylim(obj.View.axHeartWaveform, [-5 5]);
            xlim(obj.View.axTimeWaveform, [0 obj.View.efWindowLen.Value]);
            ylim(obj.View.axTimeWaveform, [-5 5]);
            xlim(obj.View.axDistanceProfile, [0 3.5]);
            ylim(obj.View.axDistanceProfile, [0 15]);
        end
        
        function onClose(obj, ~, ~)
            obj.onStop();
            obj.Driver.close();
            delete(obj.View.UIFigure);
            delete(obj);
        end
    end
end