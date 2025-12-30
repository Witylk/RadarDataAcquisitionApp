classdef PlaybackSystem < handle
    properties
        View        % 复用界面
        Processor   % 复用算法
        Dataset     % 加载的数据
        Timer       % 定时器
        CurrentIdx  % 当前播放到第几帧
        IsPlaying = false
    end
    
    methods
        function obj = PlaybackSystem()
            % 1. 选择文件
            [fname, fpath] = uigetfile('*.mat', '选择要回放的数据文件');
            if isequal(fname, 0), delete(obj); return; end
            
            % 加载数据
            loaded = load(fullfile(fpath, fname));
            if isfield(loaded, 'data')
                obj.Dataset = loaded.data;
            else
                uialert(uifigure, '数据格式不对', '错误');
                return;
            end
            
            % 2. 初始化界面和算法 (复用现有模块！)
            obj.View = ui.MainView();
            obj.Processor = core.Processor();
            
            % 修改界面标题，让人知道是在回放
            obj.View.UIFigure.Name = ['回放模式 - ' fname];
            
            % 3. 调整界面按钮功能 (覆盖原有的串口功能)
            obj.View.btnOpenPort.Text = '加载新文件';
            obj.View.btnOpenPort.ButtonPushedFcn = @obj.onLoadNew;
            
            obj.View.btnStartAcq.Text = '开始回放';
            obj.View.btnStartAcq.ButtonPushedFcn = @obj.onStartPause;
            
            % 禁用不需要的控件
            obj.View.cbDataCollection.Enable = 'off';
            obj.View.btnSaveData.Enable = 'off';
            
            % 4. 设置关闭回调
            obj.View.UIFigure.CloseRequestFcn = @obj.onClose;
            
            obj.CurrentIdx = 1;
        end
        
        function onStartPause(obj, ~, ~)
            if ~obj.IsPlaying
                % 开始播放
                obj.IsPlaying = true;
                obj.View.btnStartAcq.Text = '暂停回放';
                
                if ~isempty(obj.Timer), delete(obj.Timer); end
                obj.Timer = timer('ExecutionMode', 'fixedRate', ...
                    'Period', 0.1, 'TimerFcn', @obj.playLoop); % 0.1s 对应 10Hz
                start(obj.Timer);
            else
                % 暂停
                obj.IsPlaying = false;
                obj.View.btnStartAcq.Text = '继续回放';
                stop(obj.Timer);
            end
        end
        
        function onLoadNew(obj, ~, ~)
            % 简单的逻辑：关闭当前，重新运行一个新的
            delete(obj.View.UIFigure);
            delete(obj);
            PlaybackSystem(); 
        end
        
        function playLoop(obj, ~, ~)
            try
                % 检查是否播完
                if obj.CurrentIdx > size(obj.Dataset.fft_data, 1)
                    obj.onStartPause(); % 自动暂停
                    uialert(obj.View.UIFigure, '回放结束', '提示');
                    obj.CurrentIdx = 1; % 重置
                    return;
                end
                
                % 1. 取出一帧历史原始数据 (Raw FFT)
                % 注意：从 Dataset 中取出的通常已经是 double 或 single 类型
                raw_fft = obj.Dataset.fft_data(obj.CurrentIdx, :);
                
                % 2. 扔给处理器 (核心！！)
                res = obj.Processor.process(raw_fft);
                
                % 3. 更新界面 (完全复用)
                % 【关键修改】这里必须使用 res.distance，而不是 res.dist
                obj.View.lblPosition.Text = sprintf('%.1f', res.distance * 100);
                obj.View.lblBreathValue.Text = res.breath_str;
                obj.View.lblHeartValue.Text = res.heart_str;
                
                % 绘图
                plot(obj.View.axHeartWaveform, obj.Processor.taxis, res.heart_wave, 'Color', [1 0.3 0.3]);
                plot(obj.View.axTimeWaveform, obj.Processor.taxis, res.breath_wave, 'Color', [0 0.6 1]);
                plot(obj.View.axDistanceProfile, obj.Processor.raxis, res.mti_profile);
                
                % 标尺
                cla(obj.View.axRuler); hold(obj.View.axRuler, 'on');
                plot(obj.View.axRuler, [res.distance*100 res.distance*100], [0 1], 'r-', 'LineWidth', 3);
                hold(obj.View.axRuler, 'off');
                
                % 4. 进度条或时间显示 (可选)
                progressStr = sprintf('帧: %d / %d', obj.CurrentIdx, size(obj.Dataset.fft_data, 1));
                obj.View.efFileName.Value = progressStr;
                
                % 下一帧
                obj.CurrentIdx = obj.CurrentIdx + 1;
                
            catch ME
                stop(obj.Timer);
                uialert(obj.View.UIFigure, ['回放出错: ' ME.message], '错误');
            end
        end
        
        function onClose(obj, ~, ~)
            if ~isempty(obj.Timer), stop(obj.Timer); delete(obj.Timer); end
            delete(obj.View.UIFigure);
            delete(obj);
        end
    end
end