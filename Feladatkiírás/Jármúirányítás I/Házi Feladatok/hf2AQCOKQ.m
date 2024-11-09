mc = 500;
mw = 25;
b = 5500;
k = 15000;
kw1 = 2 * 10^4;
kw3 = -2 * 10^9;


A = [0 1 0 0;
    -k/mw -b/mw k/mw b/mw;
    0 0 0 1;
    k/mc b/mc -k/mc -b/mc];

B = [0;
    1/mw;
    0;
    0];

C = [1 0 0 0;
    0 0 1 0];

D = zeros(2, 1);

%A nemlineáris rugókaraterisztika az erő fv-ében
deltaL = [-0.01:0.00001:0.01];
force = kw1*deltaL+kw3*deltaL.^3;
plot(deltaL,force)
