%compute top 3 principle components
M = importdata('gene.txt');

%get categories
Y1 = M.textdata(:,2);
Y2 = M.textdata(:,3);
Y3 = M.textdata(:,4);
Y4 = M.textdata(:,5);

%get X 
x = M.data(:,2:4496);
[u,s,v] = svd(x);
pc1 = x*v(:,1);
pc2 = x*v(:,2);
pc3 = x*v(:,3);
X = [pc1 pc2 pc3];

%10-fold cross validate
low = [1 21 41 61 81 101 121 141 161 181];
high = [20 40 60 80 100 120 140 160 180 194];
sum = zeros(4,10);
for i = 1:10
    y1_train = Y1;
    y1_test = y1_train(low(1,i):high(1,i),:);
    y1_train(low(1,i):high(1,i),:) = [];
    
    
    y2_train = Y2;
    y2_test = y2_train(low(1,i):high(1,i),:);
    y2_train(low(1,i):high(1,i),:) = [];
    
    y3_train = Y3;
    y3_test = y3_train(low(1,i):high(1,i),:);
    y3_train(low(1,i):high(1,i),:) = [];
    
    y4_train = Y4;
    y4_test = y4_train(low(1,i):high(1,i),:);
    y4_train(low(1,i):high(1,i),:) = [];
    
    x_train = X;
    x_test = x_train(low(1,i):high(1,i),:);
    x_train(low(1,i):high(1,i),:) = [];
    
    Mdl1 = fitcecoc(x_train,y1_train);
    r1 = predict(Mdl1,x_test);
    Mdl2 = fitcecoc(x_train,y2_train);
    r2 = predict(Mdl2,x_test);
    Mdl3 = fitcecoc(x_train,y3_train);
    r3 = predict(Mdl3,x_test);
    Mdl4 = fitcecoc(x_train,y4_train);
    r4 = predict(Mdl4,x_test);
    
    max = 20;
    if i == 10
        max = 14;
    end
    for j = 1:max
        sum(1,i) = sum(1,i) + strcmp(r1(j),y1_test(j));
        sum(2,i) = sum(2,i) + strcmp(r2(j),y2_test(j));
        sum(3,i) = sum(3,i) + strcmp(r3(j),y3_test(j));
        sum(4,i) = sum(4,i) + strcmp(r4(j),y4_test(j));
    end
end
accuracy1 = mean([sum(1,1:9)./20 sum(1,10)./14]);
accuracy2 = mean([sum(2,1:9)./20 sum(2,10)./14]);
accuracy3 = mean([sum(3,1:9)./20 sum(3,10)./14]);
accuracy4 = mean([sum(4,1:9)./20 sum(4,10)./14]);
%accuracy: 0.7300 0.2700 0.5107 0.7950
%the PCs moderately retain the performance 

%accuracy with only 2 PCs: 0.7350 0.2700 0.5814 0.8100