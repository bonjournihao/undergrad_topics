im = imread('inputSeamCarvingPrague.jpg');
energyImage = energy_image(im);

cumulativeEnergyMap = cumulative_minimum_energy_map(energyImage, 'VERTICAL');
verticalSeam = find_optimal_vertical_seam(cumulativeEnergyMap);
display_seam(im,verticalSeam,'VERTICAL');

cumulativeEnergyMap = cumulative_minimum_energy_map(energyImage, 'HORIZONTAL');
horizontalSeam = find_optimal_horizontal_seam(cumulativeEnergyMap);
display_seam(im,horizontalSeam,'HORIZONTAL');