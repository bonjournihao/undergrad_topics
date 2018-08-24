%reads data from source file, output matrix M
%get rid of useless column (1st column)
%In output matrix, also represent classes by numbers
%{
  labels for 10 classes:
  CYT ->1
  NUC ->2
  MIT ->3
  ME3 ->4
  ME2 ->5
  ME1 ->6
  EXC ->7
  VAC ->8
  POX ->9
  ERL ->10
%}
function M = inputdata()
    rawData = importdata('yeast.data.txt');
    A = zeros(1484,9);
    line = cell(1,9);
    
    for i = 1:1484
        templine1 = char(rawData(i));
        templine2 = strsplit(templine1, {' '});
        line(1,:) = templine2(2:10);
        for j = 1:9
            if j == 9
                if(strcmp(line(1,9),'CYT') == 1)
                    line(1,9) = {'1'};
                elseif(strcmp(line(1,9),'NUC') == 1)
                    line(1,9) = {'2'};
                elseif(strcmp(line(1,9),'MIT') == 1)
                    line(1,9) = {'3'};
                elseif(strcmp(line(1,9),'ME3') == 1)
                    line(1,9) = {'4'};
                elseif(strcmp(line(1,9),'ME2') == 1)
                    line(1,9) = {'5'};
                elseif(strcmp(line(1,9),'ME1') == 1)
                    line(1,9) = {'6'};
                elseif(strcmp(line(1,9),'EXC') == 1)
                    line(1,9) = {'7'};
                elseif(strcmp(line(1,9),'VAC') == 1)
                    line(1,9) = {'8'};
                elseif(strcmp(line(1,9),'POX') == 1)
                    line(1,9) = {'9'};
                elseif(strcmp(line(1,9),'ERL') == 1)
                    line(1,9) = {'10'};
                end     
            end
            A(i,j) = str2double(char(line(1,j)));
        end
    end
M = A;
end