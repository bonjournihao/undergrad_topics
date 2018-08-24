function horizontalSeam = find_optimal_horizontal_seam(cumulativeEnergyMap)
    
    %%270 rotation counterclockwise
    cumulative = rot90(cumulativeEnergyMap,3);
    [size_x,~] = size(cumulativeEnergyMap);
    
    horizontal_seamArray = find_optimal_vertical_seam(cumulative);
    
    horizontalSeam = size_x - horizontal_seamArray + 1;
end