im = imread('friends.jpg');

vertical_num = 300;
horizontal_num = 50;
energyImage = energy_image(im);
[size_x,size_y] = size(energyImage);
color = im;
energy = energyImage;
for i=1:horizontal_num
    [color_result,energy_result] = reduce_height(color,energy);
    color = color_result;
    energy = energy_result;
end

for i=1:vertical_num
    [color_result,energy_result] = reduce_width(color,energy);
    color = color_result;
    energy = energy_result;
end

matlab_resize = imresize(im,[size_x-horizontal_num,size_y-vertical_num]);


subplot(3,2,1);
imshow(im);
title('original');

subplot(3,2,2);
imshow(color);
title('output');

subplot(3,2,3);
imshow(matlab_resize);
title('matlab_resize');

subplot(3,2,4);
inputstring = 'Input image is %dx%d';
outputstring = 'Output image is %dx%d';
x = size_x-horizontal_num;
y = size_y-vertical_num;
text(0.2,0.75,sprintf(inputstring,size_x,size_y));
text(0.2,0.5,sprintf(outputstring,x,y));

subplot(3,2,5);
seamstring1 = '#removed vertical seams: %d';
seamstring2 = '#removed horizontal seams: %d';
text(0.1,0.75,sprintf(seamstring1,vertical_num));
text(0.1,0.5,sprintf(seamstring2,horizontal_num));

subplot(3,2,6);
string = 'bad outcome! one person is cropped together with the background objects';
text(0,0.75,string);

%%https://s.yimg.com/ny/api/res/1.2/Q6BIJZiFrC03pl0LNdSxFg--/YXBwaWQ9aGlnaGxhbmRlcjtzbT0xO3c9NTY4O2g9Mzc4/http://media.zenfs.com/en/homerun/feed_manager_auto_publish_494/0bd23ddf233fe3cc759d95d117d36fca

