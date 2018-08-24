im = imread('villagescene.jpg');

vertical_num = 0;
horizontal_num = 300;
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
string = 'The output image eliminates horizontal seams\nthat are near the bottom';
text(0,0.75,string);

%%https://uploads2.wikiart.org/images/childe-hassam/village-scene-1885.jpg

