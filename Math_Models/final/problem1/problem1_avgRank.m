position = zeros(1,5);
ranking = zeros(1,5);
fd = fopen('problem1.dat');
A = textscan(fd, '%s %s %s %s %s', 'Delimiter', ',');
HC = zeros(1, 5);
BS = zeros(1, 5);
JK = zeros(1, 5);
TC = zeros(1, 5);
DT = zeros(1, 5);
for i = 1 : 240
    for j = 1:5
        if(strcmp(A{j}{i}, 'HC') == 1)
            HC(1, j) = HC(1, j) + 1;
        elseif(strcmp(A{j}{i}, 'BS') == 1)
            BS(1, j) = BS(1, j) + 1;
        elseif(strcmp(A{j}{i}, 'JK') == 1)
            JK(1, j) = JK(1, j) + 1;
        elseif(strcmp(A{j}{i}, 'TC') == 1)
            TC(1, j) = TC(1, j) + 1;
        else
            DT(1, j) = DT(1, j) + 1;
        end
    end
end

for k = 1:5
    position(1,1) = position(1,1) + k*HC(1,k);
    position(1,2) = position(1,2) + k*BS(1,k);
    position(1,3) = position(1,3) + k*JK(1,k);
    position(1,4) = position(1,4) + k*TC(1,k);
    position(1,5) = position(1,5) + k*DT(1,k);
end

for l = 1:5
    position(1,l) = position(1,l)/240;
end

[~,~,ranking] = unique(position);
ranking = ranking';

disp('Position of:');
disp('    HC        BS        JK        TC        DT');
disp(position);
disp('Ranking of: ');
disp('     HC    BS    JK    TC    DT');
disp(ranking);