inputIm = imread('laptop_1.jpg');
refIm = imread('laptop_2.jpg');

load own_points.mat;
p1_laptop = p1_points';
p2_laptop = p2_points';

input = im2double(inputIm);
ref = im2double(refIm);

H = computeH(p1_laptop,p2_laptop);

H_matrix = [H(1) H(2) H(3); H(4) H(5) H(6); H(7) H(8) H(9)];


[warpIm, mergeIm] = warpImage(inputIm, refIm, H_matrix);