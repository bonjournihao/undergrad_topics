ranklist = zeros(1,5);
fd = fopen('problem1.dat');
A = textscan(fd, '%s %s %s %s %s', 'Delimiter', ',');
for i = 1 : 240
    if(strcmp(A{1}{i}, 'HC') == 1)
        ranklist(1,1) = ranklist(1,1) + 1;
    elseif(strcmp(A{1}{i}, 'BS') == 1)
        ranklist(1,2) = ranklist(1,2) + 1;
    elseif(strcmp(A{1}{i}, 'JK') == 1)
        ranklist(1,3) = ranklist(1,3) + 1;
    elseif(strcmp(A{1}{i}, 'TC') == 1)
        ranklist(1,4) = ranklist(1,4) + 1;
    else
        ranklist(1,5) = ranklist(1,5) + 1;
    end
end

[~,~,sort]= unique(ranklist);
ranking = zeros(1,5);
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