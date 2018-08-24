function [reducedColorImage,reducedEnergyImage] = reduce_height(img,energyImage)
    
    im = im2double(img);
    [size_x,size_y] = size(energyImage);

    cumulativeEnergyMap = cumulative_minimum_energy_map(energyImage, 'HORIZONTAL');
    horizontalSeam = find_optimal_horizontal_seam(cumulativeEnergyMap);

    energy = zeros(size_x-1,size_y);
    color = zeros(size_x-1,size_y,3);
    for j=1:size_y
        row_to_delete = horizontalSeam(j);
        if(row_to_delete ~= 1 && row_to_delete ~= size_x)
            energy(1:row_to_delete-1,j) = energyImage(1:row_to_delete-1,j);
            energy(row_to_delete:end,j) = energyImage(row_to_delete+1:end,j);
            color(1:row_to_delete-1,j,:) = im(1:row_to_delete-1,j,:);
            color(row_to_delete:end,j,:) = im(row_to_delete+1:end,j,:);
        elseif(row_to_delete == size_x)
            energy(:,j) = energyImage(1:size_x-1,j);
            color(:,j,:) = im(1:size_x-1,j,:);
        else
            energy(:,j) = energyImage(2:end,j);
            color(:,j,:) = im(2:end,j,:);
        end
    end
    reducedColorImage = im2uint8(color);
    reducedEnergyImage = energy;
end
