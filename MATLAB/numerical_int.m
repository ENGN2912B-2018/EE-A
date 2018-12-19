clear 
close all

% Purpose: Script used for testing the numerical integration test and for
% verification against the C++ implementation

%%
% filtered = filter(fcoeff,1, accel);
% csvwrite('C:\Users\Kevin\Documents\Brown\Graduate School\Courses\ENGN 2912B\Team Assignment\EE-A\embedded\trials\stationary_filtered.csv', filtered);
% accel_ = csvread('C:\Users\Kevin\Documents\Brown\Graduate School\Courses\ENGN 2912B\Team Assignment\EE-A\embedded\trials\stationary_filtered.csv');

%%
accel  = csvread('C:\Users\Kevin\Documents\Brown\Graduate School\Courses\ENGN 2912B\Team Assignment\EE-A\embedded\trials\skateboard2.csv', 1, 0);
accel_ = csvread('C:\Users\Kevin\Documents\Brown\Graduate School\Courses\ENGN 2912B\Team Assignment\EE-A\kalman\testing output files\kalman.csv');

fs = 3200; T = 1/fs;
n = (1:size(accel_,1));
t = (n-1)*T;

calibration = [-.1208 0.7132 9.132];

knum = input("Kalman state size: ");
if(size(accel_,2) ~= knum)
    disp("Error. Wrong kalman program run"); 
else

%     df = fs/length(n);
%     f = -fs/2:df:fs/2 - df; 
%     figure, plot(f, abs(fftshift(fft(accel_(n,1)))))
%     figure, plot(f, abs(fftshift(fft(filter(ftaps,1,accel_(n,1))))))
 
    cali = csvread('.\calibration.csv', 1, 0);
    calibration = mean(cali);

%     % Remove bias from angle
%     accel_(:,1) = accel_(:,1) - mean(accel_(:,1));
%     accel_(:,3) = accel_(:,3) - mean(accel_(:,3));
%     accel_(:,1) = accel_(:,1) - calibration(1);
%     accel_(:,2) = accel_(:,2) - calibration(2);
%     accel_(:,3) = accel_(:,3) - calibration(3);
        
    % figure, plot(t, accel_(n,1));  title('accelerationf-x');
    % figure, plot(t, accel_(n,2));  title('accelerationf-y');

    vel = cumtrapz(t, accel_(n,:));
    pos = cumtrapz(t, vel(n,:));

    % figure, plot(t, accel_(:,1));  title('accelerationf-x');
    % figure, plot(t, accel_(:,2));  title('accelerationf-y');
    % figure, plot(t, accel_(:,3));  title('accelerationf-z');

    figure, plot(t, accel(n,1), t, accel_(n,1));  
    title('acceleration-x');
    xlabel('time');
    legend('observation', 'kalman');
    figure, plot(t, accel(n,2), t, accel_(n,2));  
    title('acceleration-y');
    xlabel('time');
    legend('observation', 'kalman');
    figure, plot(t, accel(n,3), t, accel_(n,3));  
    title('acceleration-z');
    xlabel('time');
    legend('observation', 'kalman');

    figure, plot(t, vel(:,1));  title('velocity-x');
    figure, plot(t, vel(:,2));  title('velocity-y');
%     figure, plot(t, vel(:,3));  title('velocity-z');

    figure, plot(t, pos(:,1));  title('position-x');
    figure, plot(t, pos(:,2));  title('position-y');
%     figure, plot(t, pos(:,3));  title('position-z');

end
