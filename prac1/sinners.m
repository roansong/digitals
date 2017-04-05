a = linspace(-pi,pi,100);
b = linspace(-pi,pi,1000);
c = linspace(-pi,pi,10000);



x1  = sin(a*1);
x2  = sin(a*2);
x10 = sin(a*10);

y1  = sin(b*1);
y2  = sin(b*2);
y10 = sin(b*10);

z1  = sin(c*1);
z2  = sin(c*2);
z10 = sin(c*10);

figure(1);
title('Overview of waveforms');
subplot(3,1,1);
plot(a,x1);
subplot(3,1,2);
plot(a,x2);
subplot(3,1,3);
plot(a,x10);



w1 = 1:50;
w2 = 2:51;
w3 = 11:60;

figure(2);
title({'Comparison of different windows at low frequency',' @ 100 samples'});
subplot(3,1,1);
plot(w1,x1(w1));
subplot(3,1,2);
plot(w1,x1(w2));
subplot(3,1,3);
plot(w1,x1(w3));

figure(3);
title('Comparison of different windows at high frequency @ 100 samples');
subplot(3,1,1);
plot(w1,x10(w1));
subplot(3,1,2);
plot(w1,x10(w2));
subplot(3,1,3);
plot(w1,x10(w3));

figure(4);
title('Comparison of different windows at high frequency @ 10000 samples');
subplot(3,1,1);
plot(w1,z10(w1));
subplot(3,1,2);
plot(w1,z10(w2));
subplot(3,1,3);
plot(w1,z10(w3));

figure(5);
title('Absolute difference between original and shifted signal at different sample rates');
subplot(3,1,1);
plot(w1,abs(x1(w1)-x1(w2)));
subplot(3,1,2);
plot(w1,abs(y1(w1)-y1(w2)));
subplot(3,1,3);
plot(w1,abs(z1(w1)-z1(w2)));


disp('100 samples')
disp('shift by 1')
corr(x1(w1),x1(w2))
corr(x2(w1),x2(w2))
corr(x10(w1),x10(w2))
disp('shift by 10')
corr(x1(w1),x1(w3))
corr(x2(w1),x2(w3))
corr(x10(w1),x10(w3))

disp('1000 samples')
disp('shift by 1')
corr(y1(w1),y1(w2))
corr(y2(w1),y2(w2))
corr(y10(w1),y10(w2))
disp('shift by 10')
corr(y1(w1),y1(w3))
corr(y2(w1),y2(w3))
corr(y10(w1),y10(w3))

disp('10000 samples')
disp('shift by 1')
corr(z1(w1),z1(w2))
corr(z2(w1),z2(w2))
corr(z10(w1),z10(w2))
disp('shift by 10')
corr(z1(w1),z1(w3))
corr(z2(w1),z2(w3))
corr(z10(w1),z10(w3))
