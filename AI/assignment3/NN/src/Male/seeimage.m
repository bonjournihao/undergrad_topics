%%male folder

M = importdata('3_2_1.txt');
N = reshape(M', [128,120]);
R = N';

%%R(1:100,1:128) = 255*ones(100,128);

%%0 -> black;255 -> white
%%Note: lots of noise

firsty = 1000;
firstx = 1000;
found_first = 0;
y_list = [];

for i=1:120
    for j=1:128
        %%scan 60 pixels downward
        if(i > firstx + 60)
            break;
        end
        if(R(i,j) > 100)
            if (found_first == 0 && j<100 && j>20)%%noise
                found_first = 1;
                firstx = i;
                firsty = j;
            end
            y_list = [y_list j];
        end
    end
end
if(firsty ~= 1000 && firstx ~= 1000)
    %%give additional weight to firsty
    y_min = min(y_list);
    y_max = max(y_list);
    y_avg = floor((mean(y_list)+2*firsty)/3);
    x_avg = firstx+35;%%somebody with bang is really avoiding!
    
    %%55x50
    low_x = x_avg-35;
    low_y = y_avg-25;
    high_x = x_avg+20;
    high_y = y_avg+25;
    I = R(low_x:high_x, low_y:high_y);

    imagesc(I);
    %%imagesc(R);
    colormap('gray');
end



