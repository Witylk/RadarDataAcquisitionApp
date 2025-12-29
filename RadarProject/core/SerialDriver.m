classdef SerialDriver < handle
    properties
        scom
    end
    
    methods
        function ok = open(obj, port, baud)
            try
                if ~isempty(obj.scom) && isvalid(obj.scom)
                    delete(obj.scom);
                end
                obj.scom = serialport(port, str2double(baud), "Timeout", 1);
                configureTerminator(obj.scom, "LF");
                flush(obj.scom);
                % 发送初始化指令
                writeline(obj.scom, 'AT+STOP'); pause(0.5);
                writeline(obj.scom, 'AT+RESET'); pause(0.5);
                writeline(obj.scom, 'AT+START'); pause(0.5);
                ok = true;
            catch
                ok = false;
            end
        end
        
        function close(obj)
            if ~isempty(obj.scom) && isvalid(obj.scom)
                try writeline(obj.scom, 'AT+STOP'); catch, end
                delete(obj.scom);
            end
            obj.scom = [];
        end
        
        function [byte_FFT, ok] = readFrame(obj)
            byte_FFT = []; ok = false;
            if isempty(obj.scom), return; end
            
            % 原代码的读取逻辑
            if obj.scom.NumBytesAvailable < 2, return; end
            
            header = [hex2dec('66'), hex2dec('BB')];
            while obj.scom.NumBytesAvailable >= 2
                b1 = read(obj.scom, 1, "uint8");
                if b1 == header(1)
                    b2 = read(obj.scom, 1, "uint8");
                    if b2 == header(2)
                        break;
                    end
                end
            end
            
            if obj.scom.NumBytesAvailable < 1208, return; end
            
            byte_FFT = read(obj.scom, 1200, "uint8");
            read(obj.scom, 4, "uint8"); % 读掉头部
            str1 = dec2hex(read(obj.scom, 4, "uint8"), 2);
            hex_num = [str1(4,:), str1(3,:), str1(2,:), str1(1,:)];
            dec_num = hex2dec(hex_num);
            
            if obj.scom.NumBytesAvailable >= dec_num
                read(obj.scom, dec_num, "uint8"); % 读掉剩余数据
                ok = true;
            end
        end
    end
end