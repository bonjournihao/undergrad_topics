%create multiclass SVM classifier and report 10-fold crossvalidation error
M = importdata('gene.txt');
%get 2612 selected feateres
w_ = problem1;

strain = {'BW25113', 'CG2', 'DH5alpha', 'MG1655', 'P2', 'P4X', 'W3110', 'rpoA14', 'rpoA27', 'rpoD3'};
medium = {'MD001', 'MD002', 'MD003', 'MD004', 'MD005', 'MD006', 'MD007', 'MD008', 'MD009', 'MD0010', 'MD0011', 'MD0012', 'MD0013', 'MD0014', 'MD0015', 'MD0016', 'MD0017', 'MD0018'};
environ = {'Indole', 'O2-starvation', 'RP-overexpress', 'antibacterial', 'carbon-limitation', 'Dna-damage', 'zinc-limitation', 'none'};
gene = {'appY_KO', 'arcA_KO', 'argR_KO', 'cya_KO', 'fis_OE', 'fnr_KO', 'frdC_KO', 'na_WT', 'oxyR_KO', 'rpoS_KO', 'soxS_KO', 'tnaA_KO'};

%get categories
Y1 = M.textdata(:,2);
Y2 = M.textdata(:,3);
Y3 = M.textdata(:,4);
Y4 = M.textdata(:,5);

%selected 2612 columns of x
x = M.data(:,2:4496);
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
sum = zeros(4,10);
sum_second = zeros(4,10);
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
    [r1,score1,~] = predict(Mdl1,x_test);
    %pick second ranked feature
    r1_second_rank = cell(size(x_test,1),1);
    for h = 1:size(x_test,1)
        temp1 = sort(score1(h,:));
        second_large1 = temp1(1,end-1);
        for k = 1:size(score1,2)
            if second_large1 == score1(h,k)
                r1_second_rank(h) = strain(1,k);
            end
        end
    end
            
    Mdl2 = fitcecoc(x_train,y2_train);
    [r2,score2,~] = predict(Mdl2,x_test);
    %pick second ranked feature
    r2_second_rank = cell(size(x_test,1),1);
    for h = 1:size(x_test,1)
        temp2 = sort(score2(h,:));
        second_large2 = temp2(1,end-1);
        for k = 1:size(score2,2)
            if second_large2 == score2(h,k)
                r2_second_rank(h) = medium(1,k);
            end
        end
    end
    
    Mdl3 = fitcecoc(x_train,y3_train);
    [r3,score3,~] = predict(Mdl3,x_test);
    %pick second ranked feature
    r3_second_rank = cell(size(x_test,1),1);
    for h = 1:size(x_test,1)
        temp3 = sort(score3(h,:));
        second_large3 = temp3(1,end-1);
        for k = 1:size(score3,2)
            if second_large3 == score3(h,k)
                r3_second_rank(h) = environ(1,k);
            end
        end
    end
    
    Mdl4 = fitcecoc(x_train,y4_train);
    [r4, score4,~] = predict(Mdl4,x_test);
    %pick second ranked feature
    r4_second_rank = cell(size(x_test,1),1);
    for h = 1:size(x_test,1)
        temp4 = sort(score4(h,:));
        second_large4 = temp4(1,end-1);
        for k = 1:size(score4,2)
            if second_large4 == score4(h,k)
                r4_second_rank(h) = gene(1,k);
            end
        end
    end
    
    max = 20;
    if i == 10
        max = 14;
    end
    for j = 1:max
        sum(1,i) = sum(1,i) + strcmp(r1(j),y1_test(j));
        sum(2,i) = sum(2,i) + strcmp(r2(j),y2_test(j));
        sum(3,i) = sum(3,i) + strcmp(r3(j),y3_test(j));
        sum(4,i) = sum(4,i) + strcmp(r4(j),y4_test(j));
        
        sum_second(1,i) = sum_second(1,i) + strcmp(r1_second_rank(j),y1_test(j));
        sum_second(2,i) = sum_second(2,i) + strcmp(r2_second_rank(j),y2_test(j));
        sum_second(3,i) = sum_second(3,i) + strcmp(r3_second_rank(j),y3_test(j));
        sum_second(4,i) = sum_second(4,i) + strcmp(r4_second_rank(j),y4_test(j));
    end
end
accuracy1 = mean([sum(1,1:9)./20 sum(1,10)./14]);
accuracy2 = mean([sum(2,1:9)./20 sum(2,10)./14]);
accuracy3 = mean([sum(3,1:9)./20 sum(3,10)./14]);
accuracy4 = mean([sum(4,1:9)./20 sum(4,10)./14]);
accuracy_second1 = mean([sum_second(1,1:9)./20 sum_second(1,10)./14]);
accuracy_second2 = mean([sum_second(2,1:9)./20 sum_second(2,10)./14]);
accuracy_second3 = mean([sum_second(3,1:9)./20 sum_second(3,10)./14]);
accuracy_second4 = mean([sum_second(4,1:9)./20 sum_second(4,10)./14]);
accuracy_medium_environ = accuracy2*accuracy3;
%accuracy : 0.6650 0.3750 0.5900 0.7100
%accuracy second : 0.0600 0.0750 0.0900 0.2193
%accuracy_medium_environ = 0.2213