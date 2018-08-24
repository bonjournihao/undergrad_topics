im = imread('inputSeamCarvingPrague.jpg');
energyImage = energy_image(im);
%%imshow(energyImage,[])


%%cumulativeEnergyMap = cumulative_minimum_energy_map(energyImage, 'VERTICAL');
%%verticalSeam = find_optimal_vertical_seam(cumulativeEnergyMap);
%%display_seam(im,verticalSeam,'VERTICAL');

[c1,e1] = reduce_width(im,energyImage);
[c2,e2] = reduce_height(im,energyImage);

%{
cumulativeEnergyMap = cumulative_minimum_energy_map(energyImage, 'HORIZONTAL');
horizontalSeam = find_optimal_horizontal_seam(cumulativeEnergyMap);
display_seam(im,horizontalSeam,'HORIZONTAL');
%}