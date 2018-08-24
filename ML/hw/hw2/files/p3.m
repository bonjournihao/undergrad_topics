%hand prove w_initial becomes w_first_round after backpropogation
nodesPerLayer = 3;
hiddenLayerNum = 1;
totalLayers = hiddenLayerNum+2;
alpha = 1;
max = 10;
if nodesPerLayer+1 > 10
    max = nodesPerLayer+1;
end

A = inputdata();
trainingMatrix = A(1:1484,:);

[w,wdisplay,changing_error,w_initial,a_initial,w_first_round,changing_w_sum] = ann(trainingMatrix,hiddenLayerNum,nodesPerLayer,alpha);