function verticalSeam = find_optimal_vertical_seam(cumulativeEnergyMap)
    %%find the optimal seam (with min cumulative value)
    cumulative = cumulativeEnergyMap;
    [size_x,size_y] = size(cumulative);
    
    min_val = min(cumulative(1,:));
    start_col = 0;
    for j = 1:size_y
        if cumulative(1,j) == min_val
            start_col = j;
            break;
        end
    end
    
    temp_col = start_col;
    seamArray = zeros(1,size_x);
    seamArray(1,1) = start_col;
    for i = 2:size_x
        if temp_col == 1 %%first pixel in row
            a = inf;
            c = cumulative(i,temp_col+1);
        elseif temp_col == size_y %%last pixel in row
            a = cumulative(i,temp_col-1);
            c = inf;
        else
            a = cumulative(i,temp_col-1);
            c = cumulative(i,temp_col+1);
        end
        b = cumulative(i,temp_col);
        list = [a b c];
        min_val = min(list);
        for index=1:3
            if list(index) == min_val
                temp_col = temp_col + index - 2;
                break;
            end
        end
        seamArray(1,i) = temp_col;
    end
    
    verticalSeam = seamArray;
    %{
    %%initialized 1st element of all seams;
    seamArray = zeros(size_y,size_x);
    seamArray(:,1) = [1:size_y]';
    seamValues = zeros(1,size_y);
    
    for j = 1:size_y
        temp_col = j;
        for i = 2:size_x
            if temp_col == 1 %%first pixel in row
                a = inf;
                c = cumulative(i,temp_col+1);
            elseif temp_col == size_y %%last pixel in row
                a = cumulative(i,temp_col-1);
                c = inf;
            else
                a = cumulative(i,temp_col-1);
                c = cumulative(i,temp_col+1);
            end
            b = cumulative(i,temp_col);
            list = [a b c];
            min_val = min(list);
            
            for index=1:3
                if list(index) == min_val
                    temp_col = temp_col + index - 2;
                    break;
                end
            end
            %%store temp_col into seamArray(j,i);
            seamArray(j,i) = temp_col;
        end
    end
    %}
end