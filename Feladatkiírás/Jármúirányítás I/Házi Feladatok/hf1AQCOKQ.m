%Adatok
m1 = 1;
m2 = 0.5;
b = 0.5;
k = 5;
U = 6;
w = 1.2;
sigma = 0.01;

%Átviteli fv:
num = m1; %szamlalo
den = [m1*m2 (m1+m2)*b (m1+m2)*k]; % nevezo
G = tf (num, den)

%Polusok zerusok
polus = pole(G)
zerus = zero(G)
figure
pzmap(G)

%A rendszer tranziensét leíró fv:
%Mivel a pólusaim a+b*i alakúak:
%A*cos(3.7997*t)*e^(-0.75*t) + B*sin(3.7997*t)*e^(-0.75*t)


%A rendszer ugrásválasza:
figure
step(G)

[stepOut tOut] = step(G)
