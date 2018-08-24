addpath('provided_code/');

%frame = 2234;
%k = 550;
%frame = 1019;
%k = 500;
%frame = 1613;
%k = 200;
%frame = 2222;
%k = 100;



frame_name = sprintf('sift/friends_%010d.jpeg.mat',frame);
load(frame_name, 'descriptors', 'positions');

im_name = sprintf('frames/friends_%010d.jpeg',frame);
im = imread(im_name);

%%get new bag of word
selected_indices = selectRegion(im, positions);
des = descriptors(selected_indices,:);

[membership,means,~] = kmeansML(k,des');
kMeans = means'; 

%%query frame histgram
hist = getHistogram(kMeans,descriptors);

nscalprod = inf*ones(6671,1);
first_frame = 60;
last_frame = 6671;
count_empty = 0;

%%score frame histogram similarities
for i = first_frame:last_frame
    string = sprintf('sift/friends_%010d.jpeg.mat',i);
    load(string,'descriptors','numfeats');
    if(isempty(descriptors))
        count_empty = count_empty+1;
        continue;
    end
    if(i == 1000||i == 2000 || i == 3000 || i == 4000|| i == 5000 || i == 6000)
            fprintf("reached frame:%d \n",i);
    end
    his = getHistogram(kMeans,descriptors);
    result = (his'*hist)/(norm(his)*norm(hist));
    nscalprod(i) = result;
end

num_frames = last_frame-first_frame-count_empty+1;
[~,order] = sort(nscalprod);
matchings = order(num_frames-5:num_frames);

%%display query frame
figure;
original = sprintf("Region Query frame: %d",frame);
imshow(sprintf('frames/friends_%010d.jpeg',frame));
title(original);

%%display 6 matching frame; simlarity: most->least
figure;
for i = 1:6
    temp = 6-i+1;
    frame_name = sprintf('frames/friends_%010d.jpeg',matchings(temp));
    subplot(2,3,i);
    imshow(frame_name);
    rank = sprintf("matching frame: %d, rank: %d;",matchings(temp),i);
    title(rank);
end
suptitle('regionQueries');




