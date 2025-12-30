classdef RunSystem < handle
    properties
        View        % 界面
        Processor   % 核心算法
        Driver      % 硬件驱动
        Timer       % 定时器
        IsRunning = false
        
        % 录制相关
        IsRecording = false % 是否正在录制
        RecordingData       % 专门用于存储录制片段的数据
    end
    
    methods
        function obj = RunSystem()
            % 1. 初始化
            obj.View = ui.MainView();
            obj.Processor = core.Processor();
            obj.Driver = core.SerialDriver();
            
            % 初始化录制缓存
            obj.initRecordingData();
            
            % 2. 绑定回调
            obj.View.btnOpenPort.ButtonPushedFcn = @obj.onOpenPort;
            obj.View.btnStartAcq.ButtonPushedFcn = @obj.onStartStop;
            
            % 【改动】绑定保存按钮到新的 Toggle 逻辑
            obj.View.btnSaveData.ButtonPushedFcn = @obj.onToggleSave;
            
            obj.View.efWindowLen.ValueChangedFcn = @obj.onWindowLenChanged;
            obj.View.spWindowLen.ValueChangedFcn = @obj.onWindowLenChanged;
            
            obj.View.btnResetTop.ButtonPushedFcn = @obj.onResetView;
            obj.View.btnResetBottom.ButtonPushedFcn = @obj.onResetView;
            
            obj.View.UIFigure.CloseRequestFcn = @obj.onClose;
        end
        
        function initRecordingData(obj)
            obj.RecordingData = struct();
            obj.RecordingData.timestamps = datetime.empty(0,1);
            obj.RecordingData.distances = [];
            obj.RecordingData.fft_data = [];
            obj.RecordingData.phase_data = zeros(0, obj.Processor.WindSize);
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
                
                % 采集开始不代表录制开始，所以这里不重置 RecordingData
                % 也不设置 IsRecording = true
                
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
            
            % 如果正在录制，强制停止并保存
            if obj.IsRecording
                obj.onToggleSave(); 
            end
            
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
            
            % 2. 处理数据
            res = obj.Processor.process(byte_FFT);
            
            % 3. 【核心改动】仅在录制状态下保存数据
            if obj.IsRecording
                obj.RecordingData.timestamps(end+1) = datetime('now');
                obj.RecordingData.distances(end+1) = res.distance;
                obj.RecordingData.fft_data(end+1, :) = res.data_FFT;
                
                % 简单处理相位数据尺寸匹配问题
                if size(obj.RecordingData.phase_data, 2) ~= length(res.breath_wave)
                     % 如果窗口变了，不做复杂处理，直接忽略或截断，防止报错
                else
                    obj.RecordingData.phase_data(end+1, :) = res.breath_wave;
                end
            end
            
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
                xlim(obj.View.axTimeWaveform, [0 newLen]);
                xlim(obj.View.axHeartWaveform, [0 newLen]);
                
                % 如果正在录制，改变窗长可能会导致 phase_data 维度不一致
                % 建议停止录制或重置 buffer，这里简单处理：重置录制buffer以防报错
                if obj.IsRecording
                    uialert(obj.View.UIFigure, '录制中改变窗长，录制已重置！', '警告');
                    obj.initRecordingData();
                end
            end
        end
        
        % --- 【新功能】开始/停止保存 ---
        function onToggleSave(obj, ~, ~)
            if ~obj.IsRecording
                % === 开始录制 ===
                obj.IsRecording = true;
                obj.View.btnSaveData.Text = '结束保存';
                obj.View.btnSaveData.BackgroundColor = [1 0.6 0.6]; % 变红提示
                
                % 清空缓存，准备开始记录新的一段
                obj.initRecordingData();
                % 确保 phase_data 宽度匹配当前 Processor 设置
                obj.RecordingData.phase_data = zeros(0, obj.Processor.WindSize);
                
                % 提示
                disp('开始录制数据...');
            else
                % === 停止录制并保存 ===
                obj.IsRecording = false;
                obj.View.btnSaveData.Text = '开始保存';
                obj.View.btnSaveData.BackgroundColor = [0.96 0.96 0.96]; % 恢复颜色
                
                % 执行保存
                obj.saveToFile();
                disp('录制结束并保存。');
            end
        end
        
        function saveToFile(obj)
            try
                fname = obj.View.efFileName.Value;
                if ~endsWith(fname, '.mat'), fname = [fname, '.mat']; end
                
                data = obj.RecordingData;
                
                % 如果没录到数据
                if isempty(data.timestamps)
                    uialert(obj.View.UIFigure, '没有录制到任何数据', '提示');
                    return; 
                end
                
                data.label = obj.View.efLabel.Value;
                data.parameters.FS = obj.Processor.FS;
                data.parameters.Rres = obj.Processor.Rres;
                data.parameters.WindSize = obj.Processor.WindSize;
                data.parameters.Range = obj.Processor.Range;
                
                save(fname, 'data');
                uialert(obj.View.UIFigure, ['成功保存 ' num2str(length(data.timestamps)) ' 帧数据到: ' fname], '成功', 'Icon', 'success');
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