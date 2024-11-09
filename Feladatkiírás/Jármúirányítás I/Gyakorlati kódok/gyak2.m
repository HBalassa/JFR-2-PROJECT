m = 1;
b = 5;
k = 1;

num = 1;
denom = [m b k];
sys = tf (num, denom)

figure
bode(sys)

