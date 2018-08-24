inputIm = imread('crop1.jpg');
refIm = imread('crop2.jpg');

load 'cc1.mat';
load 'cc2.mat';

input = im2double(inputIm);
ref = im2double(refIm);

H = computeH(cc1,cc2);

H_matrix = [H(1) H(2) H(3); H(4) H(5) H(6); H(7) H(8) H(9)];


[warpIm, mergeIm] = warpImage(inputIm, refIm, H_matrix);

