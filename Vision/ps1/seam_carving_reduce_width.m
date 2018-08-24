im1 = imread('inputSeamCarvingPrague.jpg');
im2 = imread('inputSeamCarvingMall.jpg');
energyImage1 = energy_image(im1);
energyImage2 = energy_image(im2);

color1 = im1;
color2 = im2;
energy1 = energyImage1;
energy2 = energyImage2;
for i=1:100
    [color1_result,energy1_result] = reduce_width(color1,energy1);
    color1 = color1_result;
    energy1 = energy1_result;
    [color2_result,energy2_result] = reduce_width(color2,energy2);
    color2 = color2_result;
    energy2 = energy2_result;
end

figure;
imshow(color1);
figure;
imshow(color2);