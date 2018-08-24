function energyImage = energy_image(im)
    img_gray = rgb2gray(im);
    [energyImage,~] = imgradient(img_gray,'sobel');
end
