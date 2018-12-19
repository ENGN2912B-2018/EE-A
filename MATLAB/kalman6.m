% Author: Kevin Anderson
% Script for displaying results of Kalamn Filter for 6 variable state

obv = csvread('C:\Users\Kevin\Documents\Brown\Graduate School\Courses\ENGN 2912B\Team Assignment\EE-A\embedded\trials\stationary.csv', 1, 0);
kfilt = csvread('C:\Users\Kevin\Documents\Brown\Graduate School\Courses\ENGN 2912B\Team Assignment\EE-A\kalman\testing output files\kalman.csv');


% Parameters for individual plots
fs = 3200;
T = 1/fs;
t = (1:size(kfilt, 1))*T;

% %% Plot indiviudal positions separately
% figure, plot(t, kfilt(:,1));
% title('position-x')
% xlabel('time');
% figure, plot(t, kfilt(:,2));
% title('position-y')
% xlabel('time');
% figure, plot(t, kfilt(:,3));
% title('position-z')
% xlabel('time');

% %% Plot velocity separately
% figure, plot(t, kfilt(:,1));
% title('velocity-x')
% xlabel('time');
% figure, plot(t, kfilt(:,2));
% title('velocity-y')
% xlabel('time');
% figure, plot(t, kfilt(:,3));
% title('velocity-z')
% xlabel('time');

% %% Plot acceleration separately
% figure, plot(t, kfilt(:,7));
% title('acceleration-x')
% xlabel('time');
% figure, plot(t, kfilt(:,8));
% title('acceleration-y')
% xlabel('time');
% figure, plot(t, kfilt(:,9));
% title('acceleration-z')
% xlabel('time');


%% Plot filter vs measurements
figure, plot(t, obv(1:size(kfilt, 1),1), t, kfilt(:,4));
title('acceleration-x');
xlabel('time');
legend('kalman', 'observation');
figure, plot(t, obv(1:size(kfilt, 1),2), t, kfilt(:,5));
title('acceleration-y');
xlabel('time');
legend('kalman', 'observation');
figure, plot(t, obv(1:size(kfilt, 1),3), t, kfilt(:,6));
title('acceleration-z');
xlabel('time');
legend('kalman', 'observation');

% % Filter data and write to file in kalman directory
% fdata = filtfilt(filter_coefficients, 1, obv);
% fdata_x = filtfilt(filter_coefficients, 1,obv(1:size(data, 1),1));
% d = fdata(1:size(data, 1),1) - fdata_x;
% [x,y,v] = find(d);
% csvwrite('C:\Users\Kevin\Documents\Brown\Graduate School\Courses\ENGN 2912B\Team Assignment\EE-A\kalman\stationary_filtered.csv', fdata);

% %% Plot observations
% figure, plot(t, obv(1:size(kfilt, 1),1));
% title('acceleration-x')
% xlabel('time');
% figure, plot(t, obv(1:size(kfilt, 1),2));
% title('acceleration-y')
% xlabel('time');
% figure, plot(t, obv(1:size(kfilt, 1),3));
% title('acceleration-z')
% xlabel('time');

% %% 3D plots
% % Plot position (x,y,z)
% figure, plot3(kfilt(:, 1),  kfilt(:,2), kfilt(:,3));
% title('position')
% xlabel('x');
% ylabel('y');
% zlabel('z');
% % Plot velocity (x,y,z)
% figure, plot3(kfilt(:, 4),  kfilt(:,5), kfilt(:,6));
% title('velocity')
% xlabel('x');
% ylabel('y');
% zlabel('z');
% % Plot acceleration (x,y,z)
% figure, plot3(kfilt(:, 7),  kfilt(:,8), kfilt(:,9));
% title('acceleration')
% xlabel('x');
% ylabel('y');
% zlabel('z');