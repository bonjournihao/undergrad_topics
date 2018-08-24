function hist = getHistogram(words,descriptors)
    k = size(words,1);
    hist = zeros(k,1);
    
    diff_M = distSqr(descriptors',words');
    
    numfeats = size(descriptors,1);
    for i = 1:numfeats
        %%assign one of the words to a descriptor
        [~, min_index] = min(diff_M(i,:));
        hist(min_index) = hist(min_index)+1;
    end
end