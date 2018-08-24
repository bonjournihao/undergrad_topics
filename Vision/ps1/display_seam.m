function display_seam(im,seam,seamDirection)

figure;
imshow(im);
hold on;

energyImage = energy_image(im);
[size_x,size_y] = size(energyImage);

if strcmp(seamDirection,'HORIZONTAL') == 1
    seq = [1:size_y];
    plot(seq,seam,'Color','r','LineWidth',1);
else
    seq = [1:size_x];
    plot(seam,seq,'Color','r','LineWidth',1);
end

end