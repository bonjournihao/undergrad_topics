img = imread('selectedImg.jpg');
R = img(:,:,1);
G = img(:,:,2);
B = img(:,:,3);

%%2-a
grey = rgb2gray(img);

subplot(3,2,1);
imshow(grey,'InitialMagnification', 200);
title('greyscale');
%%2-b
negative = 255 - grey;
subplot(3,2,2);
imshow(negative,'InitialMagnification', 200);
title('negative');

%%2-c
mirror = flip(img, 1);
subplot(3,2,3);
imshow(mirror,'InitialMagnification', 200);
title('mirror');

%%2-d
img_new = img;
img_new(:,:,2) = B;
img_new(:,:,3) = G;
subplot(3,2,4);
imshow(img_new,'InitialMagnification', 200);
title('switch green blue');

%%2-e
img1 = im2double(img);
img2 = im2double(mirror);
avg_img = (img1+img2)/2;
subplot(3,2,5);
imshow(avg_img,'InitialMagnification', 200);
title('average');

%%2-f
randnum = round(255*rand(1,1));
randMatrix = ones(360,480)*randnum;
add_result = uint8(randMatrix)+grey;
add_result(add_result < 0) = 0;
add_result(add_result > 255) = 255;
subplot(3,2,6);
imshow(add_result,'InitialMagnification', 200);
title('random clip');



