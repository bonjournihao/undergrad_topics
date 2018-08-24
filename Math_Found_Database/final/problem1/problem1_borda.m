ranking = zeros(1,5);
fd = fopen('problem1.dat');
A = textscan(fd, '%s %s %s %s %s', 'Delimiter', ',');
vector = [4, 3, 2, 1, 0];
valueList = zeros(1,5);
for i = 1 : 240
    for j = 1:5
        if(strcmp(A{j}{i}, 'HC') == 1)
            valueList(1,1) = valueList(1,1)+ vector(1, j);
        elseif(strcmp(A{j}{i}, 'BS') == 1)
            valueList(1,2) = valueList(1,2)+ vector(1, j);
        elseif(strcmp(A{j}{i}, 'JK') == 1)
            valueList(1,3) = valueList(1,3)+ vector(1, j);
        elseif(strcmp(A{j}{i}, 'TC') == 1)
            valueList(1,4) = valueList(1,4)+ vector(1, j);
        else
            valueList(1,5) = valueList(1,5)+ vector(1, j);
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
disp('ranking of:');
disp('     HC    BS    JK    TC    DT');
disp(ranking);