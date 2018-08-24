M = importdata('gene.txt');
x_mean = ones(1,4496);
x_mean(1,2:4496) = mean(M.data(:,2:4496));
w = problem1;
result = x_mean*w;
%result = 0.3592