[num, txt, raw] = xlsread('RoachIMU_Data3.csv');
AngleX = num(1:end,1);
AngleY = num(1:end,2);
accelx = num(1:end,3);
accely = num(1:end,4);
accelz = num(1:end,5);
gyrox = num(1:end,6);
gyroy = num(1:end,7);
gyroz = num(1:end,8);
totAcc = num(1:end,9);
totGyr = num(1:end,10);
subplot(1,2,1)
plot(1:length(AngleX(1:end)),AngleX(1:end))
title('AngleX')
subplot(1,2,2)
plot(1:length(AngleY(1:end)),AngleY(1:end))
title('AngleY')


