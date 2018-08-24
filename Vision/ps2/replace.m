inputIm = imread('cola.jpg');
refIm = imread('board.jpg');

load replace_points.mat;

p1_laptop = selectedMovingPoints';
p2_laptop = selectedFixedPoints';

input = im2double(inputIm);
ref = im2double(refIm);

H = computeH(p1_laptop,p2_laptop);

H_matrix = [H(1) H(2) H(3); H(4) H(5) H(6); H(7) H(8) H(9)];


[warpIm, mergeIm] = warpImage(inputIm, refIm, H_matrix);