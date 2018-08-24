im = imread('inputSeamCarvingPrague.jpg');
energyImage = energy_image(im);
cumulative_vertical = cumulative_minimum_energy_map(energyImage, 'VERTICAL');
cumulative_horizontal = cumulative_minimum_energy_map(energyImage, 'HORIZONTAL');

figure;
imshow(energyImage,[]);


figure;
imagesc(cumulative_vertical);
figure;
imagesc(cumulative_horizontal);


