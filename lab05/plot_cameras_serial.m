% Real time data collection example
%
% This script is implemented as a function so that it can
%   include sub-functions
%
% This script can be modified to be used on any platform by changing the
% serialPort variable.
% Example:-
% On Linux:     serialPort = '/dev/ttyS0';
% On MacOS:     serialPort = '/dev/tty.KeySerial1';
% On Windows:   serialPort = 'COM1';
%
% To run: 
% plot_cameras_serial()
%
% TODO: Complete these sections
%

function plot_cameras_serial

trace = zeros(1, 128);  % Stored Values for Raw Input
plt = tiledlayout(3,1); % Plot Layout 

ax1 = nexttile;
ax2 = nexttile;
ax3 = nexttile;

%try
    while (true)
        trace = readData(trace);
        smoothtrace = smoothData(trace);  % Smoothed data
        bintrace = edgeData(smoothtrace); % Edge detection
        if ~isvalid(plt), break;   end
        if isvalid(ax1), cla(ax1); end
        if isvalid(ax2), cla(ax2); end
        if isvalid(ax3), cla(ax3); end
        
        plotdata(trace, smoothtrace, bintrace, plt, ax1, ax2, ax3);
    end
%catch
    %close(plt.Parent);
%end

disp('Exiting...');

end % plot_cameras_serial

%*****************************************************************************************************************
%*****************************************************************************************************************
function trace = readData(trace)
    % Initialize Serial Object
    serialPort = "COM4";
    serialBaudrate = 9600;
    camera = serialport(serialPort, serialBaudrate);
    camera.FlowControl = "software";
    camera.configureTerminator("CR/LF");
    count = 1;

    % Read data from serial object for trace
    while(true)
        % disp("Searching for start..");
        val = readline(camera);
        if (strcmp(val, "-1") == 0) % if not the start
            % disp(val); % words, not numbers
            continue;
        end
        % disp("FOUND START!");
        while (true)
            val = str2double(readline(camera));
            % disp(val);
            if (val == -2)
                break;
            else
                trace(count) = val;
                count = count + 1;
            end
        end
        break; % hit from val=-2
    end

    % Clean up the serial object
    clear camera;
end

% TODO: Complete the functions below

function data = smoothData(data)
    % TODO: 5-point Averager
    %   For loop or movmean()
    for i = 1:128
        data = movmean(data, 5); % Apply a 5-point moving average

    end;

end

function data = edgeData(data)
    for i = 1:128
        % TODO: Edge detection (binary 0 or 1)
        threshold = mean(data); % Define a threshold for edge detection
        if (data > threshold)
            data(i) = 1;
        else 
            data(i) = 0; % Convert to binary based on threshold
        end

    end
end

function plotdata(trace, smoothtrace, bintrace, plt, ax1, ax2, ax3)
    % TODO: Plot data
    %   plot(ax, trace)
    %subplot(3, 1, 1);
    plot(ax1, trace);
    xlim(ax1,[0 128]);

    %subplot(3, 1, 2);
    plot(ax2, smoothtrace);
    xlim(ax2,[0 128]);
    %subplot(3, 1, 3);
    plot(ax3, bintrace);
    xlim(ax3,[0 128]);
    xlabel(ax3, 'Sample Number');
    ylabel(ax3, 'Edge Detection');
    title(ax3, 'Binary Edge Data');

    
    refreshdata;
    drawnow
end
