classdef MainView < handle
    properties
        UIFigure
        TabGroup
        Tab1
        LeftPanel
        % 组件
        cbDataCollection, lblPort, ddPort, btnOpenPort, lblBaudRate, ddBaudRate
        btnStartAcq, lblFileName, efFileName, btnSaveData, lblLabel, efLabel
        lblWindowLen, efWindowLen, spWindowLen
        lblTargetPos, lblPosition, lblUnit
        lblBreathRate, lblBreathValue, lblBreathUnit
        lblHeartRate, lblHeartValue, lblHeartUnit
        
        RightTopPanel, axHeartWaveform, axTimeWaveform, btnResetTop
        RightBottomPanel, lblProgress, axRuler, axDistanceProfile, btnResetBottom
    end
    
    methods
        function obj = MainView()
            obj.createComponents();
        end
        
        function createComponents(obj)
            obj.UIFigure = uifigure('Visible', 'off');
            obj.UIFigure.Position = [100 50 1200 750];
            obj.UIFigure.Name = 'Radar Data Acquisition';
            
            obj.TabGroup = uitabgroup(obj.UIFigure);
            obj.TabGroup.Position = [10 10 1180 730];
            
            obj.Tab1 = uitab(obj.TabGroup);
            obj.Tab1.Title = '程序模式';
            
            obj.LeftPanel = uipanel(obj.Tab1);
            obj.LeftPanel.Position = [10 10 400 680];
            obj.LeftPanel.Title = '';
            
            obj.cbDataCollection = uicheckbox(obj.LeftPanel);
            obj.cbDataCollection.Position = [20 640 150 20];
            obj.cbDataCollection.Text = '数据采集';
            obj.cbDataCollection.Value = true;
            
            obj.lblPort = uilabel(obj.LeftPanel);
            obj.lblPort.Position = [20 600 80 20];
            obj.lblPort.Text = '串口号';
            obj.ddPort = uidropdown(obj.LeftPanel);
            obj.ddPort.Position = [110 600 100 22];
            obj.ddPort.Items = {'COM1', 'COM2', 'COM3', 'COM4', 'COM5', 'COM6', 'COM7', 'COM8', 'COM9'};
            obj.ddPort.Value = 'COM7';
            obj.btnOpenPort = uibutton(obj.LeftPanel, 'push');
            obj.btnOpenPort.Position = [220 600 100 22];
            obj.btnOpenPort.Text = '打开串口';
            
            obj.lblBaudRate = uilabel(obj.LeftPanel);
            obj.lblBaudRate.Position = [20 560 80 20];
            obj.lblBaudRate.Text = '波特率';
            obj.ddBaudRate = uidropdown(obj.LeftPanel);
            obj.ddBaudRate.Position = [110 560 100 22];
            obj.ddBaudRate.Items = {'9600', '19200', '38400', '57600', '115200', '921600'};
            obj.ddBaudRate.Value = '921600';
            obj.btnStartAcq = uibutton(obj.LeftPanel, 'push');
            obj.btnStartAcq.Position = [220 560 100 22];
            obj.btnStartAcq.Text = '开始采集';
            
            obj.lblFileName = uilabel(obj.LeftPanel);
            obj.lblFileName.Position = [20 520 80 20];
            obj.lblFileName.Text = '保存文件名';
            obj.efFileName = uieditfield(obj.LeftPanel, 'text');
            obj.efFileName.Position = [110 520 150 22];
            obj.efFileName.Value = '1.mat';
            
            % === 修改点：按钮变宽，文字改为"开始保存" ===
            obj.btnSaveData = uibutton(obj.LeftPanel, 'push');
            obj.btnSaveData.Position = [270 520 80 22]; % 宽度从50改为80
            obj.btnSaveData.Text = '开始保存';
            % ===========================================
            
            obj.lblLabel = uilabel(obj.LeftPanel);
            obj.lblLabel.Position = [20 480 80 20];
            obj.lblLabel.Text = '标签实时输入';
            obj.efLabel = uieditfield(obj.LeftPanel, 'text');
            obj.efLabel.Position = [110 480 210 22];
            obj.efLabel.Value = '0';
            
            obj.lblWindowLen = uilabel(obj.LeftPanel);
            obj.lblWindowLen.Position = [20 440 80 20];
            obj.lblWindowLen.Text = '观测窗长(s)';
            obj.efWindowLen = uieditfield(obj.LeftPanel, 'numeric');
            obj.efWindowLen.Position = [110 440 170 22];
            obj.efWindowLen.Value = 30;
            
            obj.spWindowLen = uispinner(obj.LeftPanel);
            obj.spWindowLen.Position = [290 440 30 22];
            obj.spWindowLen.Value = 30;
            obj.spWindowLen.Limits = [1 100];
            
            obj.lblTargetPos = uilabel(obj.LeftPanel);
            obj.lblTargetPos.Position = [20 340 100 30];
            obj.lblTargetPos.Text = '目标位置:';
            obj.lblTargetPos.FontSize = 16;
            obj.lblPosition = uilabel(obj.LeftPanel);
            obj.lblPosition.Position = [130 320 180 80];
            obj.lblPosition.Text = '0';
            obj.lblPosition.FontSize = 56;
            obj.lblPosition.FontColor = 'red';
            obj.lblPosition.FontWeight = 'bold';
            obj.lblPosition.HorizontalAlignment = 'center';
            obj.lblUnit = uilabel(obj.LeftPanel);
            obj.lblUnit.Position = [310 340 50 40];
            obj.lblUnit.Text = 'cm';
            obj.lblUnit.FontSize = 32;
            
            obj.lblBreathRate = uilabel(obj.LeftPanel);
            obj.lblBreathRate.Position = [20 240 100 30];
            obj.lblBreathRate.Text = '呼吸率:';
            obj.lblBreathRate.FontSize = 16;
            obj.lblBreathValue = uilabel(obj.LeftPanel);
            obj.lblBreathValue.Position = [130 220 180 80];
            obj.lblBreathValue.Text = '--';
            obj.lblBreathValue.FontSize = 56;
            obj.lblBreathValue.FontColor = [0 0.6 1];
            obj.lblBreathValue.FontWeight = 'bold';
            obj.lblBreathValue.HorizontalAlignment = 'center';
            obj.lblBreathUnit = uilabel(obj.LeftPanel);
            obj.lblBreathUnit.Position = [310 240 100 40];
            obj.lblBreathUnit.Text = '次/分钟';
            obj.lblBreathUnit.FontSize = 20;
            
            obj.lblHeartRate = uilabel(obj.LeftPanel);
            obj.lblHeartRate.Position = [20 140 100 30];
            obj.lblHeartRate.Text = '心率:';
            obj.lblHeartRate.FontSize = 16;
            obj.lblHeartValue = uilabel(obj.LeftPanel);
            obj.lblHeartValue.Position = [130 120 180 80];
            obj.lblHeartValue.Text = '--';
            obj.lblHeartValue.FontSize = 56;
            obj.lblHeartValue.FontColor = [1 0.3 0.3];
            obj.lblHeartValue.FontWeight = 'bold';
            obj.lblHeartValue.HorizontalAlignment = 'center';
            obj.lblHeartUnit = uilabel(obj.LeftPanel);
            obj.lblHeartUnit.Position = [310 140 100 40];
            obj.lblHeartUnit.Text = '次/分钟';
            obj.lblHeartUnit.FontSize = 20;
            
            obj.RightTopPanel = uipanel(obj.Tab1);
            obj.RightTopPanel.Position = [420 390 750 300];
            obj.RightTopPanel.Title = '心跳与呼吸';
            
            obj.axHeartWaveform = uiaxes(obj.RightTopPanel);
            obj.axHeartWaveform.Position = [20 20 340 250];
            title(obj.axHeartWaveform, '心跳时域波形');
            xlabel(obj.axHeartWaveform, '时间/s');
            ylabel(obj.axHeartWaveform, '幅度');
            grid(obj.axHeartWaveform, 'on');
            xlim(obj.axHeartWaveform, [0 30]);
            ylim(obj.axHeartWaveform, [-5 5]);
            
            obj.axTimeWaveform = uiaxes(obj.RightTopPanel);
            obj.axTimeWaveform.Position = [390 20 340 250];
            title(obj.axTimeWaveform, '呼吸时域波形');
            xlabel(obj.axTimeWaveform, '时间/s');
            ylabel(obj.axTimeWaveform, '幅度');
            grid(obj.axTimeWaveform, 'on');
            xlim(obj.axTimeWaveform, [0 30]);
            ylim(obj.axTimeWaveform, [-5 5]);
            
            obj.btnResetTop = uibutton(obj.RightTopPanel, 'push');
            obj.btnResetTop.Position = [680 5 50 25];
            obj.btnResetTop.Text = '调整';
            
            obj.RightBottomPanel = uipanel(obj.Tab1);
            obj.RightBottomPanel.Position = [420 10 750 370];
            obj.RightBottomPanel.Title = '结果统计';
            
            obj.lblProgress = uilabel(obj.RightBottomPanel);
            obj.lblProgress.Position = [20 320 60 20];
            obj.lblProgress.Text = '距离(cm)';
            
            obj.axRuler = uiaxes(obj.RightBottomPanel);
            obj.axRuler.Position = [80 300 650 50];
            xlim(obj.axRuler, [0 100]);
            ylim(obj.axRuler, [0 1]);
            obj.axRuler.YTick = [];
            obj.axRuler.XTick = 0:10:100;
            obj.axRuler.XGrid = 'on';
            obj.axRuler.YColor = 'none';
            xlabel(obj.axRuler, '距离/cm');
            hold(obj.axRuler, 'on');
            
            obj.axDistanceProfile = uiaxes(obj.RightBottomPanel);
            obj.axDistanceProfile.Position = [50 20 650 250];
            title(obj.axDistanceProfile, '实时对消单帧距离像');
            xlabel(obj.axDistanceProfile, 'range/m');
            ylabel(obj.axDistanceProfile, 'amplitude');
            grid(obj.axDistanceProfile, 'on');
            xlim(obj.axDistanceProfile, [0 3.5]);
            ylim(obj.axDistanceProfile, [0 15]);
            
            obj.btnResetBottom = uibutton(obj.RightBottomPanel, 'push');
            obj.btnResetBottom.Position = [650 5 50 25];
            obj.btnResetBottom.Text = '调整';
            
            obj.UIFigure.Visible = 'on';
        end
    end
end