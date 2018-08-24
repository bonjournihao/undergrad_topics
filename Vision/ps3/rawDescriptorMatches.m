addpath('provided_code/');
load('provided_code/twoFrameData.mat');

%%note selected_indices is not from 1 to max
selected_indices = selectRegion(im1, positions1);
num_selected = size(selected_indices,1);

selected_descriptors = descriptors1(selected_indices,:);
selected_orients = orients1(selected_indices,:);
selected_positions = positions1(selected_indices,:);
selected_scales = scales1(selected_indices,:);

num_feat2 = size(descriptors2,1); 

min_diff_list = distSqr(selected_descriptors',descriptors2');
indices = zeros(1,num_selected);
for i = 1:num_selected
    [~,indices(i)] = min(min_diff_list(i,:));
end

figure;
imshow(im2);
displaySIFTPatches(positions2(indices,:), scales2(indices), orients2(indices), im2);