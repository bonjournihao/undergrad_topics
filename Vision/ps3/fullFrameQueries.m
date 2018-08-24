addpath('provided_code/');
load('kMeans.mat');

frame1 = 2247;
frame2 = 1288;
frame3 = 5407;

sift1 = sprintf('sift/friends_%010d.jpeg.mat',frame1);
sift2 = sprintf('sift/friends_%010d.jpeg.mat',frame2);
sift3 = sprintf('sift/friends_%010d.jpeg.mat',frame3);
load(sift1,'descriptors','numfeats');
des1 = descriptors;
numfeats1 = numfeats;
load(sift2,'descriptors');
des2 = descriptors;
numfeats2 = numfeats;
load(sift3,'descriptors');
des3 = descriptors;
numfeats3 = numfeats;

%%compare similarity histogram scores(norm.scal.product)of descriptors (given & all)
%%pick the top 5

%hist for query frames
hist1 = getHistogram(kMeans,des1);
hist2 = getHistogram(kMeans,des2);
hist3 = getHistogram(kMeans,des3);

%%iterate through all frames
    %%for each frame, find the histogram of all descriptors
    %%compute simlarity score for query frames
nscalprod1 = inf*ones(6671,1);
nscalprod2 = inf*ones(6671,1);
nscalprod3 = inf*ones(6671,1);
first_frame = 60;
last_frame = 6671;

count_empty = 0;
    
for i = first_frame:last_frame
        string = sprintf('sift/friends_%010d.jpeg.mat',i);
        load(string,'descriptors','numfeats');
        if(isempty(descriptors))
            count_empty = count_empty + 1;
            continue;
        end
        if(i == 1000||i == 2000 || i == 3000 || i == 4000|| i == 5000 || i == 6000)
            fprintf("reached frame:%d \n",i);
        end
        his = getHistogram(kMeans,descriptors);

        result1 = (his'*hist1)/(norm(his)*norm(hist1));
        nscalprod1(i) = result1;

        result2 = (his'*hist2)/(norm(his)*norm(hist2));
        nscalprod2(i) = result2;
        
        result3 = (his'*hist3)/(norm(his)*norm(hist3));
        nscalprod3(i) = result3;
end

%%find the max 6(5+self) for query frames
num_frames = 6671 - 60 - count_empty +1;

[~,order1] = sort(nscalprod1);
matchings1 = order1(num_frames-5:num_frames);
[~,order2] = sort(nscalprod2);
matchings2 = order2(num_frames-5:num_frames);
[~,order3] = sort(nscalprod3);
matchings3 = order3(num_frames-5:num_frames);

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
suptitle('1');


%%display query frame
figure;
original = sprintf("Query frame2: %d",frame2);
imshow(sprintf('frames/friends_%010d.jpeg',frame2));
title(original);

%%display 6 matching frame; simlarity: most->least
figure;
for i = 1:6
    temp = 6-i+1;
    frame_name = sprintf('frames/friends_%010d.jpeg',matchings2(temp));
    subplot(2,3,i);
    imshow(frame_name);
    rank = sprintf("matching frame: %d, rank: %d;",matchings2(temp),i);
    title(rank);
end
suptitle('2');



%%display query frame
figure;
original = sprintf("Query frame3: %d",frame3);
imshow(sprintf('frames/friends_%010d.jpeg',frame3));
title(original);

%%display 6 matching frame; simlarity: most->least
figure;
for i = 1:6
    temp = 6-i+1;
    frame_name = sprintf('frames/friends_%010d.jpeg',matchings3(temp));
    subplot(2,3,i);
    imshow(frame_name);
    rank = sprintf("matching frame: %d, rank: %d;",matchings3(temp),i);
    title(rank);
end
suptitle('3');
