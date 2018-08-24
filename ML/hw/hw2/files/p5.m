%Unknown Sample 0.49 0.51 0.52 0.23 0.55 0.03 0.52 0.39
%use 3 nodesPerLayer, 3 hiddenLayers
%call ann

nodesPerLayer = 3;
hiddenLayerNum = 3;
totalLayers = hiddenLayerNum+2;
alpha = 0.01;
max = 10;
if nodesPerLayer+1 > 10
    max = nodesPerLayer+1;
end

A = inputdata();
trainingMatrix = A(1:1484,:);

[w,wdisplay,changing_error,w_initial,a_initial,w_first_round,changing_w_sum] = ann(trainingMatrix,hiddenLayerNum,nodesPerLayer,alpha);

atest = zeros(totalLayers,max); %initialize a and z
ztest = zeros(totalLayers,max);%outputs are a(3,1:10)
for i = 1:totalLayers
    if i == 1 %input layer a = x
        atest(i,1:9) = [1 0.49 0.51 0.52 0.23 0.55 0.03 0.52 0.39];
    elseif i < totalLayers %hidden layers
        for j = 1:nodesPerLayer
            for r = 1:max
                ztest(i,j) = ztest(i,j) + w(i-1,j,r)*atest(i-1,r); 
            end
        end
        for j = 1:nodesPerLayer+1
            if j == 1
                atest(i,j) = 1;
            else
                atest(i,j) = 1./(1+exp(-ztest(i,j-1)));
            end
        end
    else %output layer
        for j = 1:10
            for r = 1:max
                ztest(i,j) = ztest(i,j) + w(i-1,j,r)*atest(i-1,r); 
            end
        end
        for j = 1:10
            atest(i,j) = 1./(1+exp(-ztest(i,j)));
        end
    end
end

largest = 0;
index = -1;
for j = 1:10
    if largest < atest(totalLayers,j)
        largest = atest(totalLayers,j);
        index = j;
    end
end

disp('prediction output: '),
disp(atest(totalLayers,:));
disp('Class:');
disp(index); %most of the time class 1 sometimes class 2
