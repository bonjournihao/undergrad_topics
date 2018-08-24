A = importdata('problem2.dat');
X = importdata('matrixX.dat');
B = zeros(7, 7);
y = zeros(21,1);
for k = 1:31
    for i = 1:6
        for j = i+1:7
            B(i,j) = B(i,j) + A(k,i)-A(k,j);
        end
    end
end

for i = 1:6
     for j = i+1:7
         y((i-1)*7+j-(i*(i+1))/2, 1) = B(i,j);
     end
end

P = X'*y;