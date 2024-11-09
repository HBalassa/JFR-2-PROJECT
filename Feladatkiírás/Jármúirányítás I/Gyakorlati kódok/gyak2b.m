m = 1200;
r = 0.25;
theta = 1;

b=10;
b2 = 2;

%A = zeros(2); %zeros fv null mátrixot tud létrehozni, egy bemenet esetén négyzetes mátrix, két változóval téglalap is lehet
A = [-b 0; 0 -b2];

B = [0 1/m; 1/theta -r/theta];

C = eye(2); 

D = zeros(2),

