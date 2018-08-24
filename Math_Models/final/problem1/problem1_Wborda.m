ranking = zeros(1,5);
fd = fopen('problem1.dat');
A = textscan(fd, '%s %s %s %s %s', 'Delimiter', ',');
vector1 = [100, 90, 40, 30, 10];
valueList = zeros(1,5);
for i = 1 : 240
    for j = 1:5
        if(strcmp(A{j}{i}, 'HC') == 1)
            valueList(1,1) = valueList(1,1)+ vector1(1, j);
        elseif(strcmp(A{j}{i}, 'BS') == 1)
            valueList(1,2) = valueList(1,2)+ vector1(1, j);
        elseif(strcmp(A{j}{i}, 'JK') == 1)
            valueList(1,3) = valueList(1,3)+ vector1(1, j);
        elseif(strcmp(A{j}{i}, 'TC') == 1)
            valueList(1,4) = valueList(1,4)+ vector1(1, j);
        else
            valueList(1,5) = valueList(1,5)+ vector1(1, j);
        end
    end
end

[~,~,sort]= unique(valueList);
for j = 1:5
    if sort(j,1) == 1
        ranking(1,j) = 5;
    elseif sort(j,1) == 2
        ranking(1,j) = 4;
    elseif sort(j,1) == 3
        ranking(1,j) = 3;
    elseif sort(j,1) == 4
        ranking(1,j) = 2;
    else
        ranking(1,j) = 1;
    end
end
disp('vector1 is: ')
disp(vector1);
disp('ranking of:');
disp('     HC    BS    JK    TC    DT');
disp(ranking);


vector2 = [100, 9, 4, 3, 1];
valueList = zeros(1,5);
for i = 1 : 240
    for j = 1:5
        if(strcmp(A{j}{i}, 'HC') == 1)
            valueList(1,1) = valueList(1,1)+ vector2(1, j);
        elseif(strcmp(A{j}{i}, 'BS') == 1)
            valueList(1,2) = valueList(1,2)+ vector2(1, j);
        elseif(strcmp(A{j}{i}, 'JK') == 1)
            valueList(1,3) = valueList(1,3)+ vector2(1, j);
        elseif(strcmp(A{j}{i}, 'TC') == 1)
            valueList(1,4) = valueList(1,4)+ vector2(1, j);
        else
            valueList(1,5) = valueList(1,5)+ vector2(1, j);
        end
    end
end

[~,~,sort]= unique(valueList);
for j = 1:5
    if sort(j,1) == 1
        ranking(1,j) = 5;
    elseif sort(j,1) == 2
        ranking(1,j) = 4;
    elseif sort(j,1) == 3
        ranking(1,j) = 3;
    elseif sort(j,1) == 4
        ranking(1,j) = 2;
    else
        ranking(1,j) = 1;
    end
end
disp('vector2 is: ')
disp(vector2);
disp('ranking of:');
disp('     HC    BS    JK    TC    DT');
disp(ranking);


vector3 = [30, 30, 30, 30, 10];
valueList = zeros(1,5);
for i = 1 : 240
    for j = 1:5
        if(strcmp(A{j}{i}, 'HC') == 1)
            valueList(1,1) = valueList(1,1)+ vector3(1, j);
        elseif(strcmp(A{j}{i}, 'BS') == 1)
            valueList(1,2) = valueList(1,2)+ vector3(1, j);
        elseif(strcmp(A{j}{i}, 'JK') == 1)
            valueList(1,3) = valueList(1,3)+ vector3(1, j);
        elseif(strcmp(A{j}{i}, 'TC') == 1)
            valueList(1,4) = valueList(1,4)+ vector3(1, j);
        else
            valueList(1,5) = valueList(1,5)+ vector3(1, j);
        end
    end
end

[~,~,sort]= unique(valueList);
for j = 1:5
    if sort(j,1) == 1
        ranking(1,j) = 5;
    elseif sort(j,1) == 2
        ranking(1,j) = 4;
    elseif sort(j,1) == 3
        ranking(1,j) = 3;
    elseif sort(j,1) == 4
        ranking(1,j) = 2;
    else
        ranking(1,j) = 1;
    end
end
disp('vector3 is: ')
disp(vector3);
disp('ranking of:');
disp('     HC    BS    JK    TC    DT');
disp(ranking);


vector4 = [30, 10, 4, 3, 1];
valueList = zeros(1,5);
for i = 1 : 240
    for j = 1:5
        if(strcmp(A{j}{i}, 'HC') == 1)
            valueList(1,1) = valueList(1,1)+ vector4(1, j);
        elseif(strcmp(A{j}{i}, 'BS') == 1)
            valueList(1,2) = valueList(1,2)+ vector1(1, j);
        elseif(strcmp(A{j}{i}, 'JK') == 1)
            valueList(1,3) = valueList(1,3)+ vector4(1, j);
        elseif(strcmp(A{j}{i}, 'TC') == 1)
            valueList(1,4) = valueList(1,4)+ vector4(1, j);
        else
            valueList(1,5) = valueList(1,5)+ vector4(1, j);
        end
    end
end

[~,~,sort]= unique(valueList);
for j = 1:5
    if sort(j,1) == 1
        ranking(1,j) = 5;
    elseif sort(j,1) == 2
        ranking(1,j) = 4;
    elseif sort(j,1) == 3
        ranking(1,j) = 3;
    elseif sort(j,1) == 4
        ranking(1,j) = 2;
    else
        ranking(1,j) = 1;
    end
end
disp('vector4 is: ')
disp(vector4);
disp('ranking of:');
disp('     HC    BS    JK    TC    DT');
disp(ranking);


vector5 = [10, 10, 10, 10, 10];
valueList = zeros(1,5);
for i = 1 : 240
    for j = 1:5
        if(strcmp(A{j}{i}, 'HC') == 1)
            valueList(1,1) = valueList(1,1)+ vector5(1, j);
        elseif(strcmp(A{j}{i}, 'BS') == 1)
            valueList(1,2) = valueList(1,2)+ vector5(1, j);
        elseif(strcmp(A{j}{i}, 'JK') == 1)
            valueList(1,3) = valueList(1,3)+ vector5(1, j);
        elseif(strcmp(A{j}{i}, 'TC') == 1)
            valueList(1,4) = valueList(1,4)+ vector5(1, j);
        else
            valueList(1,5) = valueList(1,5)+ vector5(1, j);
        end
    end
end

[~,~,sort]= unique(valueList);
for j = 1:5
    if sort(j,1) == 1
        ranking(1,j) = 5;
    elseif sort(j,1) == 2
        ranking(1,j) = 4;
    elseif sort(j,1) == 3
        ranking(1,j) = 3;
    elseif sort(j,1) == 4
        ranking(1,j) = 2;
    else
        ranking(1,j) = 1;
    end
end
disp('vector5 is: ')
disp(vector5);
disp('ranking of:');
disp('     HC    BS    JK    TC    DT');
disp(ranking);




disp('If w2 in vector w is close to w1 and is large, HC is likely to win');
disp('Otherwise, BS is likely to win');
disp('Either BS or HC could win if the vector w is modified.');
disp('However, because w = (w1,w2,w3,...wn) and w1 >= w2 >= ... >= wn');
disp('and');
disp('# of each position:      1  2  3  4  5')
disp('                    HC = 85 92 24 22 17');
disp('                    BS = 96 55 39 31 19');
disp('                    JK = 7 28 98 61 46');
disp('                    TC = 8 31 40 75 86');
disp('                    DT = 44 34 39 51 72');
disp('So HC and BS always beats the other three candidates');
disp('The other three  will never be able to win unless the vector is uniform');
disp('(has the same value in all its entries)');




