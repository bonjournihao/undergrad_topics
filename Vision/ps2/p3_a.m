run('vlfeat-0.9.20/toolbox/vl_setup');

im1 = imread('wdc1.jpg');
im2 = imread('wdc2.jpg');

inputIm = single(rgb2gray(im1));
refIm = single(rgb2gray(im2));


[fa, da] = vl_sift(inputIm,'PeakThresh', 12) ;
[fb, db] = vl_sift(refIm,'PeakThresh', 12) ;
[matches, scores] = vl_ubcmatch(da, db) ;

input_indices = matches(1,:);
ref_indices = matches(2,:);

num_pairs = size(matches,2);
input_points = zeros(2,num_pairs);
ref_points = zeros(2,num_pairs);

for i = 1:num_pairs
    input_index = input_indices(1,i);
    ref_index = ref_indices(1,i);
    input_points(:,i) = fa(1:2,input_index);
    ref_points(:,i) = fb(1:2,ref_index);
end

figure;
imshow(im1);
hold on;
scatter(input_points(1,:),input_points(2,:),'r');
hold off;

figure;
imshow(im2);
hold on;
scatter(ref_points(1,:),ref_points(2,:),'r');
