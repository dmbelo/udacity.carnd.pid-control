clear

m = 1000; % Mass
vx = 27; % Longitudinal velocity
k = 5000; % Linear tyre coefficent

a = 0.5;
L = 1;
I = 1;

% PID
kp = 0.05;
ki = 0.002;
kd = 0.0;

sys1 = tf(k, [m, k/vx, 0]);
sys2 = tf([kd, kp, ki],[1 0]); % PID
sys3 = feedback(sys1*sys2,1);% Close loop control

s = tf('s');

% pzmap(sys3)
step(sys3)
hold on