function [reducedColorImage,reducedEnergyImage] = reduce_width(img,energyImage)

    im = im2double(img);
    [size_x,size_y] = size(energyImage);

    cumulativeEnergyMap = cumulative_minimum_energy_map(energyImage, 'VERTICAL');
    verticalSeam = find_optimal_vertical_seam(cumulativeEnergyMap);

    energy = zeros(size_x,size_y-1);
    color = zeros(size_x,size_y-1,3);
    for i=1:size_x
        col_to_delete = verticalSeam(i);
        if(col_to_delete ~= 1 && col_to_delete ~= size_y)
            energy(i,1:col_to_delete-1) = energyImage(i,1:col_to_delete-1);
            energy(i,col_to_delete:end) = energyImage(i,col_to_delete+1:end);
            color(i,1:col_to_delete-1,:) = im(i,1:col_to_delete-1,:);
            color(i,col_to_delete:end,:) = im(i,col_to_delete+1:end,:);
        elseif(col_to_delete == size_y)
            energy(i,:) = energyImage(i,1:size_y-1);
            color(i,:,:) = im(i,1:size_y-1,:);
        else
            energy(i,:) = energyImage(i,2:end);
            color(i,:,:) = im(i,2:end,:);
        end
    end
    reducedColorImage = im2uint8(color);
    reducedEnergyImage = energy;
end