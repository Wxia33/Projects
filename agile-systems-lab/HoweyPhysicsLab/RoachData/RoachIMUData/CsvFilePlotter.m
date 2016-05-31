[num, txt, raw] = xlsread('controldata.csv');

accelx = num(1:end,3);
accely = num(1:end,4);
accelz = num(1:end,5);
gyrox = num(1:end,6);
gyroy = num(1:end,7);
gyroz = num(1:end,8);

angleX = rad2deg(atan(y/sqrt(x^2+z^2)));
angleY = rad2deg(atan(x/sqrt(y^2+z^2)));

compFangX = 0.98 * (angleX + gyrox * dt) + 0.02 * accelx;
compFangY = 0.98 * (angleY + gyroY * dt) + 0.02 * accely;

subplot(1,2,1)
plot(1:length(AngleX(1:end)),AngleX(1:end))
title('AngleX')
subplot(1,2,2)
plot(1:length(AngleY(1:end)),AngleY(1:end))
title('AngleY')


