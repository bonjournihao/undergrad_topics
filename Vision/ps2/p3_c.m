im1 = imread('squaretile_input.jpg');

inputIm = im2double(im1);
refIm = double(160*ones(N,N,3));

N = 200;

load p3_c_points.mat;

cc1 = selectedMovingPoints';
cc2 = [1 1 N N; 1 N N 1];

H = computeH(cc1,cc2);

H_matrix = [H(1) H(2) H(3); H(4) H(5) H(6); H(7) H(8) H(9)];

figure;
imshow(im1);
hold on;
scatter(cc1(1,:),cc1(2,:),'r');
hold off;

[warpIm, mergeIm] = warp2(inputIm, refIm, H_matrix);
