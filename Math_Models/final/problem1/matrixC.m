A = importdata('problem1_revisedData.dat');
C1 = zeros(5,5);
C2 = zeros(5,5);
C = zeros(5,5);
C_abs = zeros(5,5);
for i = 1:240
    for j = 1:4
        for k = j+1:5
            C1(A(i,j),A(i,k)) = C1(A(i,j),A(i,k)) + 1;
        end
    end
    
    for m = 5 :-1: 2
        for n = m-1:-1 : 1
            C2(A(i,m),A(i,n)) = C2(A(i,m),A(i,n)) + 1;
        end
    end
end

C = C1 - C2;
C_abs = abs(C);