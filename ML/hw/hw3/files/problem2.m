%10 resampling with 10 from 194 each time -> bootstrap distibutino sample mean and sd dev
M = importdata('gene.txt');

w = problem1;

y = zeros(194,1);
ys = zeros(10000,1);

for j = 1:10000
    for i = 1:194
        samples = floor(rand*200);
        while(samples < 1 || samples > 194)
            samples = floor(rand*200);
        end       
        y(i) = M.data(samples,1);
    end
    ys(j,1) = mean(y);
end

abc = sort(ys);
low = abc(250);
high = abc(9750);
% 95-confidence interval [0.3623 0.4269]
