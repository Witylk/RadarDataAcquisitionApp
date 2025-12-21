classdef RadarDataAcquisitionApp < matlab.apps.AppBase
    % Properties that correspond to app components
    properties (Access = public)
        UIFigure matlab.ui.Figure
        TabGroup matlab.ui.container.TabGroup
        Tab1 matlab.ui.container.Tab
        % 左侧控制面板组件
        LeftPanel matlab.ui.container.Panel
        cbDataCollection matlab.ui.control.CheckBox
        lblPort matlab.ui.control.Label
        ddPort matlab.ui.control.DropDown
        btnOpenPort matlab.ui.control.Button
        lblBaudRate matlab.ui.control.Label
        ddBaudRate matlab.ui.control.DropDown
        btnStartAcq matlab.ui.control.Button
        lblFileName matlab.ui.control.Label
        efFileName matlab.ui.control.EditField
        btnSaveData matlab.ui.control.Button
        lblLabel matlab.ui.control.Label
        efLabel matlab.ui.control.EditField
        lblWindowLen matlab.ui.control.Label
        efWindowLen matlab.ui.control.NumericEditField
        spWindowLen matlab.ui.control.Spinner
        lblTargetPos matlab.ui.control.Label
        lblPosition matlab.ui.control.Label
        lblUnit matlab.ui.control.Label
        lblBreathRate matlab.ui.control.Label   % 呼吸率标题
        lblBreathValue matlab.ui.control.Label  % 呼吸次数大数字
        lblBreathUnit matlab.ui.control.Label   % 单位"次/分钟"
        lblHeartRate matlab.ui.control.Label    % 新增：心率标题
        lblHeartValue matlab.ui.control.Label   % 新增：心跳次数大数字
        lblHeartUnit matlab.ui.control.Label    % 新增：单位"次/分钟"
        % 右上方图表面板
        RightTopPanel matlab.ui.container.Panel
        axHeartWaveform matlab.ui.control.UIAxes  % 改名：心跳时域波形
        axTimeWaveform matlab.ui.control.UIAxes   % 呼吸时域波形
        btnResetTop matlab.ui.control.Button
        % 右下方结果统计面板
        RightBottomPanel matlab.ui.container.Panel
        lblProgress matlab.ui.control.Label
        axRuler matlab.ui.control.UIAxes
        axDistanceProfile matlab.ui.control.UIAxes
        btnResetBottom matlab.ui.control.Button
    end
   
    properties (Access = private)
        % 串口和控制标志
        scom % 串口对象
        isAcquiring = false % 采集标志
        isPortOpen = false % 串口打开标志
        timerObj % 定时器对象
        % 雷达参数
        FS = 10 % 采样频率
        FFTNum = 512 % FFT点数
        frametime % 帧时间
        Rres = 0.05 % 距离分辨率
        WindSize = 300 % 窗口大小
        Range = 150 % 距离范围
        raxis % 距离轴
        taxis % 时间轴
        % 数据缓存
        data_FFT_old % 上一帧FFT数据
        fifo_ori % 原始数据FIFO
        fifo_mti % MTI数据FIFO
        savedData % 用于保存的数据结构
        % 滤波器
        Hd_breath % 呼吸带通滤波器 (0.1-0.6 Hz)
        Hd_heart  % 心跳带通滤波器 (0.8-2.0 Hz)
    end
   
    methods (Access = private)
        % 初始化参数
        function initializeParameters(app)
            app.frametime = 1 / app.FS;
            app.raxis = 0:app.Rres:app.Range*app.Rres-app.Rres;
            app.taxis = 0:app.frametime:app.WindSize*app.frametime-app.frametime;
            app.data_FFT_old = zeros(1, app.Range);
            app.fifo_ori = zeros(app.WindSize, app.Range);
            app.fifo_mti = zeros(app.WindSize, app.Range);
            % 初始化保存数据结构
            app.savedData = struct();
            app.savedData.timestamps = datetime.empty(0,1);
            app.savedData.distances = [];
            app.savedData.fft_data = [];
            app.savedData.phase_data = zeros(0, app.WindSize);
            % 创建滤波器
            app.Hd_breath = createBreathBPF();  % 呼吸滤波器
            app.Hd_heart = createHeartBPF();    % 心跳滤波器
        end
        
        % 更新窗长回调
        function updateWindowLen(app, ~)
            newWindowLen = app.spWindowLen.Value;
            app.efWindowLen.Value = newWindowLen;
            if app.isAcquiring
                old_WindSize = app.WindSize;
                app.WindSize = newWindowLen * app.FS;
                app.taxis = 0:app.frametime:app.WindSize*app.frametime-app.frametime;
                app.fifo_ori = zeros(app.WindSize, app.Range);
                app.fifo_mti = zeros(app.WindSize, app.Range);
                if ~isempty(app.savedData.phase_data)
                    old_phase = app.savedData.phase_data;
                    app.savedData.phase_data = zeros(size(old_phase,1), app.WindSize);
                    app.savedData.phase_data(:,1:min(old_WindSize, app.WindSize)) = old_phase(:,1:min(old_WindSize, app.WindSize));
                end
                xlim(app.axTimeWaveform, [0 newWindowLen]);
                xlim(app.axHeartWaveform, [0 newWindowLen]);
            end
        end
        
        % 编辑框窗长改变回调
        function efWindowLenChanged(app, ~)
            newWindowLen = app.efWindowLen.Value;
            app.spWindowLen.Value = newWindowLen;
            if app.isAcquiring
                old_WindSize = app.WindSize;
                app.WindSize = newWindowLen * app.FS;
                app.taxis = 0:app.frametime:app.WindSize*app.frametime-app.frametime;
                app.fifo_ori = zeros(app.WindSize, app.Range);
                app.fifo_mti = zeros(app.WindSize, app.Range);
                if ~isempty(app.savedData.phase_data)
                    old_phase = app.savedData.phase_data;
                    app.savedData.phase_data = zeros(size(old_phase,1), app.WindSize);
                    app.savedData.phase_data(:,1:min(old_WindSize, app.WindSize)) = old_phase(:,1:min(old_WindSize, app.WindSize));
                end
                xlim(app.axTimeWaveform, [0 newWindowLen]);
                xlim(app.axHeartWaveform, [0 newWindowLen]);
            end
        end
        
        % 打开/关闭串口
        function openPortCallback(app, ~)
            if ~app.isPortOpen
                try
                    port = app.ddPort.Value;
                    baudrate = str2double(app.ddBaudRate.Value);
                    if ~isempty(app.scom) && isvalid(app.scom)
                        delete(app.scom);
                    end
                    app.scom = serialport(port, baudrate, "Timeout", 1);
                    configureTerminator(app.scom, "LF");
                    flush(app.scom);
                    sendCommand(app, 'AT+STOP');
                    pause(0.5);
                    sendCommand(app, 'AT+RESET');
                    pause(0.5);
                    sendCommand(app, 'AT+START');
                    pause(0.5);
                    app.isPortOpen = true;
                    app.btnOpenPort.Text = '关闭串口';
                    uialert(app.UIFigure, ['成功打开 ' port], '提示', 'Icon', 'success');
                catch ME
                    uialert(app.UIFigure, ['打开串口失败: ' ME.message], '错误', 'Icon', 'error');
                end
            else
                if app.isAcquiring
                    stopAcquisition(app);
                end
                if ~isempty(app.scom) && isvalid(app.scom)
                    sendCommand(app, 'AT+STOP');
                    delete(app.scom);
                end
                app.scom = [];
                app.isPortOpen = false;
                app.btnOpenPort.Text = '打开串口';
            end
        end
        
        % 开始/停止采集
        function startAcquisitionCallback(app, ~)
            if ~app.isAcquiring
                if ~app.isPortOpen || isempty(app.scom) || ~isvalid(app.scom)
                    uialert(app.UIFigure, '请先打开串口', '提示', 'Icon', 'warning');
                    return;
                end
                initializeParameters(app);
                app.isAcquiring = true;
                app.btnStartAcq.Text = '停止采集';
                app.timerObj = timer('ExecutionMode', 'fixedRate', ...
                    'Period', 0.1, ...
                    'TimerFcn', @(~,~) acquireData(app));
                start(app.timerObj);
            else
                stopAcquisition(app);
            end
        end
        
        % 停止采集
        function stopAcquisition(app)
            app.isAcquiring = false;
            app.btnStartAcq.Text = '开始采集';
            if ~isempty(app.timerObj) && isvalid(app.timerObj)
                stop(app.timerObj);
                delete(app.timerObj);
                app.timerObj = [];
            end
        end
        
        % 数据采集函数
        function acquireData(app)
            try
                if app.scom.NumBytesAvailable < 2
                    return;
                end
                header = [hex2dec('66'), hex2dec('BB')];
                while app.scom.NumBytesAvailable >= 2
                    byte_head1 = read(app.scom, 1, "uint8");
                    if byte_head1 == header(1)
                        byte_head2 = read(app.scom, 1, "uint8");
                        if byte_head2 == header(2)
                            break;
                        end
                    end
                end
                if app.scom.NumBytesAvailable < 1208
                    return;
                end
                byte_FFT = read(app.scom, 1200, "uint8");
                byte_head3 = read(app.scom, 4, "uint8");
                str1 = dec2hex(read(app.scom, 4, "uint8"), 2);
                hex_num = [str1(4,:), str1(3,:), str1(2,:), str1(1,:)];
                dec_num = hex2dec(hex_num);
                if app.scom.NumBytesAvailable >= dec_num
                    data = read(app.scom, dec_num, "uint8");
                else
                    return;
                end
                
                % 数据处理
                data_FFT_ori = typecast(uint8(byte_FFT), 'single');
                data_FFT = data_FFT_ori(1:150) + 1i * data_FFT_ori(151:end);
                app.fifo_ori(1:end-1, :) = app.fifo_ori(2:end, :);
                app.fifo_ori(end, :) = data_FFT';
                app.fifo_mti = abs(app.fifo_ori - mean(app.fifo_ori, 1));
                app.fifo_mti = sum(app.fifo_mti, 1);
                [m, p] = max(app.fifo_mti(1:75));
                
                % 更新目标位置
                distance = p * app.Rres;
                distance_cm = distance * 100;
                app.lblPosition.Text = sprintf('%.1f', distance_cm);
                
                % 保存数据
                app.savedData.timestamps(end+1) = datetime('now');
                app.savedData.distances(end+1) = distance;
                app.savedData.fft_data(end+1, :) = data_FFT;
                
                % 更新距离刻度尺
                cla(app.axRuler);
                hold(app.axRuler, 'on');
                plot(app.axRuler, [distance_cm distance_cm], [0 1], 'r-', 'LineWidth', 3);
                hold(app.axRuler, 'off');
                
                % 呼吸和心跳相位处理
                phase_breath = angle(app.fifo_ori(:, p))';
                phase_temp = [diff(unwrap(phase_breath)), 0];
                
                % 呼吸滤波 (0.1-0.6 Hz)
                phase_breath_filtered = filter(app.Hd_breath, phase_temp);
                
                % 心跳滤波 (0.8-2.0 Hz)
                phase_heart_filtered = filter(app.Hd_heart, phase_temp);
                
                % 保存相位数据
                app.savedData.phase_data(end+1, 1:length(phase_breath_filtered)) = phase_breath_filtered;
                
                % MTI处理
                data_FFT_mti = abs(data_FFT - app.data_FFT_old);
                app.data_FFT_old = data_FFT;
                
                % 更新距离像图
                plot(app.axDistanceProfile, app.raxis, data_FFT_mti);
                title(app.axDistanceProfile, '实时对消单帧距离像');
                xlabel(app.axDistanceProfile, 'range/m');
                ylabel(app.axDistanceProfile, 'amplitude');
                grid(app.axDistanceProfile, 'on');
                
                % 更新心跳时域波形图
                plot(app.axHeartWaveform, app.taxis, phase_heart_filtered, 'Color', [1 0.3 0.3]);
                title(app.axHeartWaveform, '心跳时域波形');
                xlabel(app.axHeartWaveform, '时间/s');
                ylabel(app.axHeartWaveform, '幅度');
                grid(app.axHeartWaveform, 'on');
                
                % 更新呼吸时域波形图
                plot(app.axTimeWaveform, app.taxis, phase_breath_filtered, 'Color', [0 0.6 1]);
                title(app.axTimeWaveform, '呼吸时域波形');
                xlabel(app.axTimeWaveform, '时间/s');
                ylabel(app.axTimeWaveform, '幅度');
                grid(app.axTimeWaveform, 'on');
                
                % ==================== 实时呼吸率计算 ====================
                breath_window_sec = app.efWindowLen.Value;
                idx = seconds(app.savedData.timestamps(end) - app.savedData.timestamps) <= breath_window_sec;
                
                if sum(idx) > breath_window_sec * app.FS / 3
                    recent_phase = app.savedData.phase_data(idx,:);
                    recent_phase = recent_phase(:);
                    
                    [pks, locs] = findpeaks(recent_phase, 'MinPeakDistance', app.FS * 2);
                    valid_idx = pks > 0.10;
                    valid_locs = locs(valid_idx);
                    
                    if length(valid_locs) >= 2
                        breath_count = length(valid_locs);
                        actual_duration = length(recent_phase) / app.FS;
                        breath_rate = (breath_count / actual_duration) * 60;
                        app.lblBreathValue.Text = sprintf('%.1f', breath_rate);
                    else
                        app.lblBreathValue.Text = '--';
                    end
                else
                    app.lblBreathValue.Text = '--';
                end
                
                % ==================== 实时心率计算 (修正部分) ====================
                % 直接使用当前窗口已经滤波好的波形: phase_heart_filtered
                
                if ~isempty(phase_heart_filtered)
                    % 1. 去除滤波器前端的瞬态响应（忽略前2秒数据）
                    skip_sec = 2;
                    start_idx = round(app.FS * skip_sec);
                    
                    if length(phase_heart_filtered) > start_idx
                        calc_data = phase_heart_filtered(start_idx:end);
                    else
                        calc_data = phase_heart_filtered;
                    end
                    
                    % 2. 寻找峰值
                    % MinPeakDistance: 0.33s (对应最高心率约180bpm)
                    % MinPeakHeight: 0.15 
                    [pks_heart, locs_heart] = findpeaks(calc_data, ...
                        'MinPeakDistance', floor(app.FS * 0.33), ...
                        'MinPeakHeight', 0.15);
                    
                    if length(locs_heart) >= 2
                        % 3. 计算心率 (基于峰值平均间隔)
                        mean_interval = mean(diff(locs_heart)) / app.FS; % 秒
                        heart_rate = 60 / mean_interval;
                        
                        % 4. 合理性范围检查 (40-160 bpm)
                        if heart_rate >= 40 && heart_rate <= 160
                            app.lblHeartValue.Text = sprintf('%.0f', heart_rate);
                        else
                            app.lblHeartValue.Text = '--';
                        end
                    else
                        app.lblHeartValue.Text = '--';
                    end
                else
                    app.lblHeartValue.Text = '--';
                end
                
            catch ME
                disp(['采集错误: ' ME.message]);
            end
        end
       
        % 复原视图回调函数
        function resetTopView(app, ~)
            axis(app.axHeartWaveform, 'auto');
            xlim(app.axHeartWaveform, [0 app.efWindowLen.Value]);
            ylim(app.axHeartWaveform, [-5 5]);
            axis(app.axTimeWaveform, 'auto');
            xlim(app.axTimeWaveform, [0 app.efWindowLen.Value]);
            ylim(app.axTimeWaveform, [-5 5]);
        end
       
        function resetBottomView(app, ~)
            axis(app.axDistanceProfile, 'auto');
            xlim(app.axDistanceProfile, [0 3.5]);
            ylim(app.axDistanceProfile, [0 15]);
        end
       
        % 保存数据回调
        function saveDataCallback(app, ~)
            try
                filename = app.efFileName.Value;
                if ~endsWith(filename, '.mat')
                    filename = [filename, '.mat'];
                end
                data.timestamps = app.savedData.timestamps;
                data.distances = app.savedData.distances;
                data.fft_data = app.savedData.fft_data;
                data.phase_data = app.savedData.phase_data;
                data.label = app.efLabel.Value;
                data.parameters.FS = app.FS;
                data.parameters.Rres = app.Rres;
                data.parameters.WindSize = app.WindSize;
                data.parameters.Range = app.Range;
                save(filename, 'data');
                uialert(app.UIFigure, ...
                    ['数据已保存到: ' filename newline '位置: ' pwd], ...
                    '保存成功', 'Icon', 'success');
            catch ME
                uialert(app.UIFigure, ...
                    ['保存失败: ' ME.message], ...
                    '错误', 'Icon', 'error');
            end
        end
       
        % 发送命令函数
        function sendCommand(app, cmd)
            writeline(app.scom, cmd);
        end
    end
   
    % 组件初始化和布局
    methods (Access = private)
        % 创建UI组件
        function createComponents(app)
            % 创建主窗口
            app.UIFigure = uifigure('Visible', 'off');
            app.UIFigure.Position = [100 50 1200 750];
            app.UIFigure.Name = 'MATLAB App';
            app.UIFigure.CloseRequestFcn = @(~,~) closeApp(app);
            
            % 创建标签页组
            app.TabGroup = uitabgroup(app.UIFigure);
            app.TabGroup.Position = [10 10 1180 730];
            
            % 创建"程序模式"标签页
            app.Tab1 = uitab(app.TabGroup);
            app.Tab1.Title = '程序模式';
            
            % ==================== 左侧控制面板 ====================
            app.LeftPanel = uipanel(app.Tab1);
            app.LeftPanel.Position = [10 10 400 680];
            app.LeftPanel.Title = '';
            
            % 数据采集复选框
            app.cbDataCollection = uicheckbox(app.LeftPanel);
            app.cbDataCollection.Position = [20 640 150 20];
            app.cbDataCollection.Text = '数据采集';
            app.cbDataCollection.Value = true;
            
            % 串口号
            app.lblPort = uilabel(app.LeftPanel);
            app.lblPort.Position = [20 600 80 20];
            app.lblPort.Text = '串口号';
            app.ddPort = uidropdown(app.LeftPanel);
            app.ddPort.Position = [110 600 100 22];
            app.ddPort.Items = {'COM1', 'COM2', 'COM3', 'COM4', 'COM5', 'COM6', 'COM7', 'COM8', 'COM9'};
            app.ddPort.Value = 'COM7';
            app.btnOpenPort = uibutton(app.LeftPanel, 'push');
            app.btnOpenPort.Position = [220 600 100 22];
            app.btnOpenPort.Text = '打开串口';
            app.btnOpenPort.ButtonPushedFcn = @(~,event) openPortCallback(app, event);
            
            % 波特率
            app.lblBaudRate = uilabel(app.LeftPanel);
            app.lblBaudRate.Position = [20 560 80 20];
            app.lblBaudRate.Text = '波特率';
            app.ddBaudRate = uidropdown(app.LeftPanel);
            app.ddBaudRate.Position = [110 560 100 22];
            app.ddBaudRate.Items = {'9600', '19200', '38400', '57600', '115200', '921600'};
            app.ddBaudRate.Value = '921600';
            app.btnStartAcq = uibutton(app.LeftPanel, 'push');
            app.btnStartAcq.Position = [220 560 100 22];
            app.btnStartAcq.Text = '开始采集';
            app.btnStartAcq.ButtonPushedFcn = @(~,event) startAcquisitionCallback(app, event);
            
            % 保存文件名
            app.lblFileName = uilabel(app.LeftPanel);
            app.lblFileName.Position = [20 520 80 20];
            app.lblFileName.Text = '保存文件名';
            app.efFileName = uieditfield(app.LeftPanel, 'text');
            app.efFileName.Position = [110 520 150 22];
            app.efFileName.Value = '1.mat';
            app.btnSaveData = uibutton(app.LeftPanel, 'push');
            app.btnSaveData.Position = [270 520 50 22];
            app.btnSaveData.Text = '保存';
            app.btnSaveData.ButtonPushedFcn = @(~,event) saveDataCallback(app, event);
            
            % 标签实时输入
            app.lblLabel = uilabel(app.LeftPanel);
            app.lblLabel.Position = [20 480 80 20];
            app.lblLabel.Text = '标签实时输入';
            app.efLabel = uieditfield(app.LeftPanel, 'text');
            app.efLabel.Position = [110 480 210 22];
            app.efLabel.Value = '0';
            
            % 观测窗长
            app.lblWindowLen = uilabel(app.LeftPanel);
            app.lblWindowLen.Position = [20 440 80 20];
            app.lblWindowLen.Text = '观测窗长(s)';
            app.efWindowLen = uieditfield(app.LeftPanel, 'numeric');
            app.efWindowLen.Position = [110 440 170 22];
            app.efWindowLen.Value = 30;
            app.efWindowLen.ValueChangedFcn = @(~,event) efWindowLenChanged(app, event);
            app.spWindowLen = uispinner(app.LeftPanel);
            app.spWindowLen.Position = [290 440 30 22];
            app.spWindowLen.Value = 30;
            app.spWindowLen.Limits = [1 100];
            app.spWindowLen.ValueChangedFcn = @(~,event) updateWindowLen(app, event);
            
            % 目标位置显示
            app.lblTargetPos = uilabel(app.LeftPanel);
            app.lblTargetPos.Position = [20 340 100 30];
            app.lblTargetPos.Text = '目标位置:';
            app.lblTargetPos.FontSize = 16;
            app.lblPosition = uilabel(app.LeftPanel);
            app.lblPosition.Position = [130 320 180 80];
            app.lblPosition.Text = '0';
            app.lblPosition.FontSize = 56;
            app.lblPosition.FontColor = 'red';
            app.lblPosition.FontWeight = 'bold';
            app.lblPosition.HorizontalAlignment = 'center';
            app.lblUnit = uilabel(app.LeftPanel);
            app.lblUnit.Position = [310 340 50 40];
            app.lblUnit.Text = 'cm';
            app.lblUnit.FontSize = 32;
            
            % ============= 呼吸率显示 =============
            app.lblBreathRate = uilabel(app.LeftPanel);
            app.lblBreathRate.Position = [20 240 100 30];
            app.lblBreathRate.Text = '呼吸率:';
            app.lblBreathRate.FontSize = 16;
            app.lblBreathValue = uilabel(app.LeftPanel);
            app.lblBreathValue.Position = [130 220 180 80];
            app.lblBreathValue.Text = '--';
            app.lblBreathValue.FontSize = 56;
            app.lblBreathValue.FontColor = [0 0.6 1];
            app.lblBreathValue.FontWeight = 'bold';
            app.lblBreathValue.HorizontalAlignment = 'center';
            app.lblBreathUnit = uilabel(app.LeftPanel);
            app.lblBreathUnit.Position = [310 240 100 40];
            app.lblBreathUnit.Text = '次/分钟';
            app.lblBreathUnit.FontSize = 20;
            
            % ============= 新增：心率显示 =============
            app.lblHeartRate = uilabel(app.LeftPanel);
            app.lblHeartRate.Position = [20 140 100 30];
            app.lblHeartRate.Text = '心率:';
            app.lblHeartRate.FontSize = 16;
            app.lblHeartValue = uilabel(app.LeftPanel);
            app.lblHeartValue.Position = [130 120 180 80];
            app.lblHeartValue.Text = '--';
            app.lblHeartValue.FontSize = 56;
            app.lblHeartValue.FontColor = [1 0.3 0.3];  % 红色
            app.lblHeartValue.FontWeight = 'bold';
            app.lblHeartValue.HorizontalAlignment = 'center';
            app.lblHeartUnit = uilabel(app.LeftPanel);
            app.lblHeartUnit.Position = [310 140 100 40];
            app.lblHeartUnit.Text = '次/分钟';
            app.lblHeartUnit.FontSize = 20;
            
            % ==================== 右上方图表面板 ====================
            app.RightTopPanel = uipanel(app.Tab1);
            app.RightTopPanel.Position = [420 390 750 300];
            app.RightTopPanel.Title = '心跳与呼吸';
            
            % 创建心跳时域波形图（左侧，原来的1DFFT位置）
            app.axHeartWaveform = uiaxes(app.RightTopPanel);
            app.axHeartWaveform.Position = [20 20 340 250];
            title(app.axHeartWaveform, '心跳时域波形');
            xlabel(app.axHeartWaveform, '时间/s');
            ylabel(app.axHeartWaveform, '幅度');
            grid(app.axHeartWaveform, 'on');
            xlim(app.axHeartWaveform, [0 30]);
            ylim(app.axHeartWaveform, [-5 5]);
            
            % 创建呼吸时域波形图（右侧）
            app.axTimeWaveform = uiaxes(app.RightTopPanel);
            app.axTimeWaveform.Position = [390 20 340 250];
            title(app.axTimeWaveform, '呼吸时域波形');
            xlabel(app.axTimeWaveform, '时间/s');
            ylabel(app.axTimeWaveform, '幅度');
            grid(app.axTimeWaveform, 'on');
            xlim(app.axTimeWaveform, [0 30]);
            ylim(app.axTimeWaveform, [-5 5]);
            
            % 添加复原视图按钮（上方）
            app.btnResetTop = uibutton(app.RightTopPanel, 'push');
            app.btnResetTop.Position = [680 5 50 25];
            app.btnResetTop.Text = '调整';
            app.btnResetTop.ButtonPushedFcn = @(~,event) resetTopView(app, event);
            
            % ==================== 右下方结果统计面板 ====================
            app.RightBottomPanel = uipanel(app.Tab1);
            app.RightBottomPanel.Position = [420 10 750 370];
            app.RightBottomPanel.Title = '结果统计';
            
            % 创建距离刻度尺
            app.lblProgress = uilabel(app.RightBottomPanel);
            app.lblProgress.Position = [20 320 60 20];
            app.lblProgress.Text = '距离(cm)';
            app.axRuler = uiaxes(app.RightBottomPanel);
            app.axRuler.Position = [80 300 650 50];
            xlim(app.axRuler, [0 100]);
            ylim(app.axRuler, [0 1]);
            app.axRuler.YTick = [];
            app.axRuler.XTick = 0:10:100;
            app.axRuler.XGrid = 'on';
            app.axRuler.YColor = 'none';
            xlabel(app.axRuler, '距离/cm');
            hold(app.axRuler, 'on');
            
            % 创建实时对消单帧距离像图
            app.axDistanceProfile = uiaxes(app.RightBottomPanel);
            app.axDistanceProfile.Position = [50 20 650 250];
            title(app.axDistanceProfile, '实时对消单帧距离像');
            xlabel(app.axDistanceProfile, 'range/m');
            ylabel(app.axDistanceProfile, 'amplitude');
            grid(app.axDistanceProfile, 'on');
            xlim(app.axDistanceProfile, [0 3.5]);
            ylim(app.axDistanceProfile, [0 15]);
            
            % 添加复原视图按钮（下方）
            app.btnResetBottom = uibutton(app.RightBottomPanel, 'push');
            app.btnResetBottom.Position = [650 5 50 25];
            app.btnResetBottom.Text = '调整';
            app.btnResetBottom.ButtonPushedFcn = @(~,event) resetBottomView(app, event);
            
            % 显示窗口
            app.UIFigure.Visible = 'on';
        end
    end
   
    % App创建和删除
    methods (Access = public)
        % 构造函数
        function app = RadarDataAcquisitionApp
            createComponents(app);
            registerApp(app, app.UIFigure);
            if nargout == 0
                clear app
            end
        end
       
        % 析构函数
        function delete(app)
            if app.isAcquiring
                stopAcquisition(app);
            end
            if ~isempty(app.scom) && isvalid(app.scom)
                try
                    sendCommand(app, 'AT+STOP');
                    delete(app.scom);
                catch
                end
            end
            delete(app.UIFigure);
        end
       
        % 关闭应用回调
        function closeApp(app)
            delete(app);
        end
    end
end
% ==================== 辅助函数 ====================
% 创建呼吸带通滤波器 (0.1-0.6 Hz)
function Hd = createBreathBPF()
    Fs = 10;
    N = 8;
    Fc1 = 0.1;
    Fc2 = 0.6;
    h = fdesign.bandpass('N,F3dB1,F3dB2', N, Fc1, Fc2, Fs);
    Hd = design(h, 'butter');
end
% 创建心跳带通滤波器 (0.8-2.0 Hz)
function Hd = createHeartBPF()
    Fs = 10;
    N = 8;
    Fc1 = 0.8;  % 心跳频率下限：48 bpm
    Fc2 = 2.5;  % 心跳频率上限：150 bpm
    h = fdesign.bandpass('N,F3dB1,F3dB2', N, Fc1, Fc2, Fs);
    Hd = design(h, 'butter');
end
