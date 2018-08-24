A = importdata('problem2.dat');
t = [186,186,186,186,186,186,186];
C = zeros(7,7);
for p = 1:7
    for q = 1:7
        if p == q
            C(p,q) = 2 + t(1,p);
        else
            C(p,q) = -31;
        end
    end
end
w = zeros(1,7);
l = zeros(1,7);
b = zeros(1,7);
for k = 1:31
    for i = 1:6
        for j = i+1:7
            if A(k,i)  > A(k,j)
                w(1,i) = w(1,i)+1;
                l(1,j) = l(1,j)+1;
            elseif A(k,j) > A(k,i)
                w(1,j) = w(1,j)+1;
                l(1,i) = l(1,i)+1;
            else
            end
        end
    end
end

for m = 1:7
    b(1,m) = 1 + 0.5 * (w(1,m) - l(1,m));
end

bb = b';
r = linsolve(C,bb);
[~,~,ranking] = unique(r);
disp('The score vector of problems from 1 to 7 is:');
disp(r');
disp('Because problems that have lower scores are more difficult, the above implies that');
disp('The difficulty ranking of problems from 1 to 7 is: (with 1 being the most difficult)')
disp(ranking');
