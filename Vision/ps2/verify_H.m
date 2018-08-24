im1 = imread('crop1.jpg');
im2 = imread('crop2.jpg');

load 'cc1.mat';
load 'cc2.mat';


H = computeH(cc1,cc2);

H_matrix = [H(1) H(2) H(3); H(4) H(5) H(6); H(7) H(8) H(9)];

temp_ones = ones(1,size(cc1,2));
p = [cc1;temp_ones];
p_prime = H_matrix * p;
output = zeros(2,size(p_prime,2));
output(1,:) = p_prime(1,:)./p_prime(3,:);
output(2,:) = p_prime(2,:)./p_prime(3,:);

figure;
imshow(im2);
hold on;
scatter(cc2(1,:),cc2(2,:),'Filled','b');
hold on;
scatter(output(1,:),output(2,:),'r');