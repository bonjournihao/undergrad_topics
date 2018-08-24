function cumulativeEnergyMap = cumulative_minimum_energy_map(energyIm,seamDirection)
    
    if strcmp(seamDirection, 'VERTICAL') == 1
        energyImage = energyIm;
    else
        energyImage = rot90(energyIm,3);
    end
    
    
    [size_x,size_y] = size(energyImage);
    cumulative = zeros(size_x,size_y);
    cumulative(size_x,:) = energyImage(size_x,:);

    for i = (size_x-1):-1:1
        for j = 1:size_y
            if j == 1 %%first pixel in row
                a = 1000;
                c = cumulative(i+1,j+1);
            elseif j == size_y %%last pixel in row
                a = cumulative(i+1,j-1);
                c = 1000;
            else
                a = cumulative(i+1,j-1);
                c = cumulative(i+1,j+1);
            end
            b = cumulative(i+1,j);
            list = [a b c];

            cumulative(i,j) = energyImage(i,j) + min(list);
        end
    end 
    
    if strcmp(seamDirection, 'VERTICAL') == 1
        cumulativeEnergyMap = cumulative;
    else
        cumulativeEnergyMap = rot90(cumulative,1);
    end  
    
end
