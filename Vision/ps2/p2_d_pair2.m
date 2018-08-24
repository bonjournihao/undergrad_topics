inputIm = imread('wdc1.jpg');
refIm = imread('wdc2.jpg');

%%[p1_points,p2_points] = cpselect(inputIm,refIm,'Wait',true);
load points.mat;

p1_laptop = p1_points';
p2_laptop = p2_points';

input = im2double(inputIm);
ref = im2double(refIm);

H = computeH(p1_laptop,p2_laptop);

H_matrix = [H(1) H(2) H(3); H(4) H(5) H(6); H(7) H(8) H(9)];


[warpIm, mergeIm] = warpImage(inputIm, refIm, H_matrix);