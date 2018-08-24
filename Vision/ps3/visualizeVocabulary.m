addpath('provided_code/');
descriptor_list = [];
imname_list = [];
position_list = [];
scale_list = [];
orient_list = [];
k = 1500;
frame_num = 300;

randList = floor(6611.*rand(1,frame_num)) + 60;

numfeats1=0;
for i = randList
    string = sprintf('sift/friends_%010d.jpeg.mat',i);
    load(string,'imname', 'descriptors', 'positions', 'scales', 'orients','numfeats');
    if(isempty(descriptors))
        continue;
    end
    descriptor_list(1+numfeats1:numfeats1+numfeats,1:128) = descriptors;
    imname_list(1+numfeats1:numfeats1+numfeats,1) = i;
    position_list(1+numfeats1:numfeats1+numfeats,1:2) = positions;
    scale_list(1+numfeats1:numfeats1+numfeats,1) = scales;
    orient_list(1+numfeats1:numfeats1+numfeats,1) = orients;
    numfeats1 = size(descriptor_list,1);
end

[membership,means,~] = kmeansML(k,descriptor_list');
means = means';

%%inverted index

%%collect patches for 2 words, entries are indices
%%in struct_list
word1 = 500;
word2 = 1200;
sample_size = size(membership,1);


patches_list1 = [];
patches_index_list1 = [];
patches_list2 = [];
patches_index_list2 = [];

%%find all patches to a word
for i = 1:sample_size
    temp_word = membership(i);
    if(temp_word == word1)
        patches_list1 = [patches_list1; descriptor_list(i,:)];
        patches_index_list1 = [patches_index_list1; i];
    elseif(temp_word == word2)
        patches_list2 = [patches_list2; descriptor_list(i,:)];
        patches_index_list2 = [patches_index_list2; i];
    end
end

%%find the top 25 batches.
diff_list1 = zeros(size(patches_list1,1),1);
centriod1 = means(word1,:);
for i = 1:size(patches_list1,1)
    diff1 = dist2(centriod1,patches_list1(i,:));
    diff_list1(i) = diff1;
end

diff_list2 = zeros(size(patches_list2,1),1);
centriod2 = means(word2,:);
for i = 1:size(patches_list2,1)
    diff2 = dist2(centriod2,patches_list2(i,:));
    diff_list2(i) = diff2;
end

%%order: new order of the diff_list indices in result_list
[result_list1, order1] = sort(diff_list1);
[result_list2, order2] = sort(diff_list2);

%%get the top 25 patches' indices in big list;
%%use order -> patches_index_list
ordered_list1 = zeros(1,min(size(order1,1),25));
for i = 1:min(size(order1,1),25)
    ordered_list1(i) = patches_index_list1(order1(i));
end
ordered_list2 = zeros(1,min(size(order2,1),25));
for i = 1:min(size(order2,1),25)
    ordered_list2(i) = patches_index_list2(order2(i));
end

%%plot the 25 patches of the word
figure;
string1 = sprintf('frameNum:%d    word1: %d',frame_num,word1);

for i = 1:min(size(order1,1),25)
    this = ordered_list1(i);
    
    %%get sift parameters
    this_pos = position_list(this,:);
    this_scal = scale_list(this);
    this_ori = orient_list(this);
    im_name = sprintf('frames/friends_%010d.jpeg',imname_list(this));
    this_im = imread(im_name);
    grey = rgb2gray(this_im);
    %%get visual images of word
    this_patch = getPatchFromSIFTParameters(this_pos, this_scal, this_ori, grey);
    
    subplot(4,7,i);
    imshow(this_patch);
end
suptitle(string1);

figure;
string2 = sprintf('frameNum:%d    word2: %d',frame_num,word2);
for i = 1:min(size(order2,1),25)
    this = ordered_list2(i);
    
    %%get sift parameters
    this_pos = position_list(this,:);
    this_scal = scale_list(this);
    this_ori = orient_list(this);
    im_name = sprintf('frames/friends_%010d.jpeg',imname_list(this));
    this_im = imread(im_name);
    grey = rgb2gray(this_im);
    %%get visual images of word
    this_patch = getPatchFromSIFTParameters(this_pos, this_scal, this_ori, grey);
    
    subplot(4,7,i);
    imshow(this_patch);
end
suptitle(string2);

kMeans = means;
save('kMeans.mat','kMeans');