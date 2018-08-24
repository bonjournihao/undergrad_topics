%create multiclass SVM classifier and report 10-fold crossvalidation error
M = importdata('gene.txt');
%get 2612 selected features
w_ = problem1;

%get categories
Y2 = M.textdata(:,3);
Y3 = M.textdata(:,4);
composite_y = class_label(Y2,Y3);

%select 2612 columns of x
x= M.data(:,2:4496);
X = [];
columns = [];
for i = 1:4495
    if w_(i)~=0
        X = horzcat(X,x(:,i));
        columns = horzcat(columns, [i]);
    end
end

%10-fold cross validate
low = [1 21 41 61 81 101 121 141 161 181];
high = [20 40 60 80 100 120 140 160 180 194];
sum = zeros(1,10);
for i = 1:10
    y_train = composite_y;
    y_test = y_train(low(1,i):high(1,i),:);
    y_train(low(1,i):high(1,i),:) = [];
   
    x_train = X;
    x_test = x_train(low(1,i):high(1,i),:);
    x_train(low(1,i):high(1,i),:) = [];

    Mdl = fitcecoc(x_train,y_train);
    r = predict(Mdl,x_test);   
    
    max = 20;
    if i == 10
        max = 14;
    end
    for j = 1:max
        sum(1,i) = sum(1,i) + strcmp(r(j),y_test(j));
    end
end
accuracy_composite = mean([sum(1,1:9)./20 sum(1,10)./14]);
%accuracy_composite = 0.2300
%accuracy_medium_environ = 0.2213