function w_final = problem1()
%first, call createTextFile to get gene.txt
M = importdata('gene.txt');

%get 10-fold cross validation errors for 10 lamda values

I = eye(4496);
lamda = 0.000001;
low = [1 21 41 61 81 101 121 141 161 181];
high = [20 40 60 80 100 120 140 160 180 194];
sqError = zeros(1,10);
error = zeros(1,8);
%{
%lamda : 0.00001 = 100
for index = 1:8
    lamda = lamda*10;
    for i = 1:10
        y_growth = M.data(:,1);
        y_test = y_growth(low(1,i):high(1,i),:);
        y_growth(low(1,i):high(1,i),:) = [];
        
        x_expression = ones(194,4496);
        x_expression(:,2:4496) =  M.data(:,2:4496);
        x_test = x_expression(low(1,i):high(1,i),:);
        x_expression(low(1,i):high(1,i),:) = [];

        w = (x_expression'*x_expression + lamda*I)^-1*x_expression'*y_growth;

        for j = 1:4496
            if abs(w(j)) < 0.001
                w(j) = 0;
            end
        end
        sqError(1,i) = mean((y_test - x_test*w).^2);
    end
    error(1,index) = mean(sqError);
end
%error=[0.0338,0.0337,0.0335,0.0327,0.0332,0.0717]
%Choose lamda = 0.01
%}
lamda_final = 0.01;
count = 0;
y = M.data(:,1);
x = ones(194,4496);
x(:,2:4496) =  M.data(:,2:4496);
w_final = (x'*x + lamda_final*I)^-1*x'*y;
for j = 1:4496
    if abs(w_final(j)) < 0.001
        w_final(j) = 0;
    else
        count = count + 1;
    end
end
%count = 2612
end