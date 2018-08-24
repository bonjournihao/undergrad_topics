azip = importdata('azip.mat');
dzip = importdata('dzip.mat');
dtest = importdata('dtest.mat');
testzip = importdata('testzip.mat');
classNum = 0;
class = zeros(256,1,10);
class0 = zeros(256,1);
class1 = zeros(256,1);
class2 = zeros(256,1);
class3 = zeros(256,1);
class4 = zeros(256,1);
class5 = zeros(256,1);
class6 = zeros(256,1);
class7 = zeros(256,1);
class8 = zeros(256,1);
class9 = zeros(256,1);

temp = zeros(256,1);
for i=1:1707
    temp = azip(:,i);
    classNum = dzip(i);
    if classNum == 0
        class0 = horzcat(class0,temp);
    elseif classNum == 1
        class1 = horzcat(class1, temp);
    elseif classNum == 2
        class2 = horzcat(class2, temp);
    elseif classNum == 3
        class3 = horzcat(class3, temp);
    elseif classNum == 4
        class4 = horzcat(class4, temp);
    elseif classNum == 5
        class5 = horzcat(class5, temp);
    elseif classNum == 6
        class6 = horzcat(class6, temp);
    elseif classNum == 7
        class7 = horzcat(class7, temp);
    elseif classNum == 8
        class8 = horzcat(class8, temp);
    else classNum == 9
        class9 = horzcat(class9, temp);
    end
end

class0=class0(:,2:end);
class1=class1(:,2:end);
class2=class2(:,2:end);
class3=class3(:,2:end);
class4=class4(:,2:end);
class5=class5(:,2:end);
class6=class6(:,2:end);
class7=class7(:,2:end);
class8=class8(:,2:end);
class9=class9(:,2:end);

U = ones(256,256,10);

[U(:,:,1), S, V] = svd(class0);
[U(:,:,2), S, V] = svd(class1);
[U(:,:,3), S, V] = svd(class2);
[U(:,:,4), S, V] = svd(class3);
[U(:,:,5), S, V] = svd(class4);
[U(:,:,6), S, V] = svd(class5);
[U(:,:,7), S, V] = svd(class6);
[U(:,:,8), S, V] = svd(class7);
[U(:,:,9), S, V] = svd(class8);
[U(:,:,10), S, V] = svd(class9);

U5 = zeros(256,256,10);
U10 = zeros(256,256,10);
U20 = zeros(256,256,10);

for i=1:10
    U5(:,1:5,i) = U(:,1:5,i);
    U5(:,6:256,i) = zeros(256,251);
end

for i=1:10
    U10(:,1:10,i) = U(:,1:10,i);
    U10(:,11:256,i) = zeros(256,246);
end

for i=1:10
    U20(:,1:20,i) = U(:,1:20,i);
    U20(:,21:256,i) = zeros(256,236);
end


a5 = zeros(1,256);
a10 = zeros(1,256);
a20 = zeros(1,256);
right_class5 = zeros(1, 2007);
right_class10 = zeros(1, 2007);
right_class20 = zeros(1, 2007);
all_writings_check = zeros(1,10);

c1 = 2007;
for j = 1:2007
    rightIndex = 1;
    a = lsqr(U5(:,:,1),testzip(:,j)); %testing the first number using U5
    min_res = (norm(testzip(:,j)-U5(:,:,1)*a))/norm(testzip(:,j));
    for i = 2:10 % check residual for each class from 0 t0 9
        a = lsqr(U5(:,:,i),testzip(:,j)); 
        res = (norm(testzip(:,j)-U5(:,:,i)*a))/norm(testzip(:,j));
        if min_res > res
            min_res = res;
            rightIndex = i;
        end
    end
    right_class_5(j) = rightIndex - 1;
    if (rightIndex - 1) ~= dtest(j)
        c1 = c1 - 1;
        all_writings_check(dtest(j)+1) = all_writings_check(dtest(j)+1) + 1;
    end
end


c2 = 2007;
for j = 1:2007
    rightIndex = 1;
    a = lsqr(U10(:,:,1),testzip(:,j)); %testing the first number using U10
    min_res = (norm(testzip(:,j)-U10(:,:,1)*a))/norm(testzip(:,j));
    for i = 2:10 % check residual for each class from 0 t0 9
        a = lsqr(U10(:,:,i),testzip(:,j)); 
        res = (norm(testzip(:,j)-U10(:,:,i)*a))/norm(testzip(:,j));
        if min_res > res
            min_res = res;
            rightIndex = i;
        end
    end
    right_class_10(j) = rightIndex - 1;
    if (rightIndex - 1) ~= dtest(j)
        c2 = c2 - 1;
        all_writings_check(dtest(j)+1) = all_writings_check(dtest(j)+1) + 1;
    end
end


c3 = 2007;
for j = 1:2007
    rightIndex = 1;
    a = lsqr(U20(:,:,1),testzip(:,j)); %testing the first number using U20
    min_res = (norm(testzip(:,j)-U20(:,:,1)*a))/norm(testzip(:,j));
    for i = 2:10 % check residual for each class from 0 t0 9
        a = lsqr(U20(:,:,i),testzip(:,j)); 
        res = (norm(testzip(:,j)-U20(:,:,i)*a))/norm(testzip(:,j));
        if min_res > res
            min_res = res;
            rightIndex = i;
        end
    end
    right_class_20(j) = rightIndex - 1;
    if (rightIndex - 1) ~= dtest(j)
        c3 = c3 - 1;
        all_writings_check(dtest(j)+1) = all_writings_check(dtest(j)+1) + 1;
    end
end


worst_digit = 0;
max = all_writings_check(1);


for k=2:10
    if all_writings_check(k) > max
        max = all_writings_check(k);
        worst_digit = k - 1;
    end
end 

percent5 = 100 * c1 / 2007;
percent10 = 100 * c2 / 2007;
percent20 = 100 * c3 / 2007;

f = figure('Position',[200 500 700 100])
d = [percent5 percent10 percent20];
cnames = {'5 vectors','10 vectors','20 vectors'};
rnames = {'Percentage(correctness)'};
t = uitable(f,'Data',d,'ColumnName',cnames,'RowName',rnames,'ColumnWidth',{100});
t.Position(3) = t.Extent(3);
t.Position(4) = t.Extent(4);

%%for class 1, we can use fewer singular vectors
%%the most difficult digit to read for the computer is 8.
%%No it doesn't