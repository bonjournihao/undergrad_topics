addpath('provided_code/');
descriptor_list = [];
imname_list = [];
position_list = [];
scale_list = [];
orient_list = [];
k = 1500;
frame_num = 200;

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

[word_his,~] = histcounts(membership',k);
[~,order] = sort(word_his,'descend');

stop_num = 500;
stop_list = order(1,1:stop_num);
means(stop_list,:) = [];
kMeans = means;

k_new = k - stop_num;

frame1 = 4125;

sift1 = sprintf('sift/friends_%010d.jpeg.mat',frame1);
load(sift1,'descriptors','numfeats');
des1 = descriptors;
numfeats1 = numfeats;


first_frame = 60;
last_frame = 6671;
count_empty = 0;

%%check existence of each word
count_exist = zeros(1,k_new);
count_occurance = zeros(1,k_new);
total_words_frame = zeros(last_frame-first_frame+1,1);
his = zeros(last_frame-first_frame+1,k_new);
for i = first_frame:last_frame
        string = sprintf('sift/friends_%010d.jpeg.mat',i);
        load(string,'descriptors','numfeats');
        if(isempty(descriptors))
            count_empty = count_empty + 1;
            continue;
        end
        if(i == 1000||i == 2000 || i == 3000 || i == 4000|| i == 5000 || i == 6000)
            fprintf("checking word count reached frame:%d \n",i);
        end
        his(i,:) = getHistogram(kMeans,descriptors)';
        temp = his(i,:);
        temp(temp ~= 0) = 1;
        
        total_words_frame(i) = sum(temp);
        count_exist = count_exist + temp;
end

num_frames = 6671 - 60 - count_empty +1;

%weighted_hist for query frame
hist1 = getHistogram(kMeans,des1);
weighted1 = zeros(1,k_new);
for j = 1:k_new
    weighted1(j) = (hist1(j)/total_words_frame(frame1))*log((num_frames/count_exist(j)));
end

%%calculated scores tf-idf
nscalprod1 = inf*ones(6671,1);
    
for i = first_frame:last_frame
        weighted = zeros(k_new,1);
        for j = 1:k_new
            weighted(j) = (his(i,j)/total_words_frame(i))*log((num_frames/count_exist(j)));
        end
        result1 = (weighted1*weighted)/(norm(weighted)*norm(weighted1));
        nscalprod1(i) = result1;
end

%%find the max 6(5+self) for query frames

[~,order1] = sort(nscalprod1);
matchings1 = order1(num_frames-5:num_frames);

%%display query frame
figure;
original = sprintf("Query frame1: %d",frame1);
imshow(sprintf('frames/friends_%010d.jpeg',frame1));
title(original);

%%display 6 matching frame; simlarity: most->least
figure;
for i = 1:6
    temp = 6-i+1;
    frame_name = sprintf('frames/friends_%010d.jpeg',matchings1(temp));
    subplot(2,3,i);
    imshow(frame_name);
    rank = sprintf("matching frame: %d, rank: %d;",matchings1(temp),i);
    title(rank);
end