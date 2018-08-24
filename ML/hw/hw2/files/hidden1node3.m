%calls function ann
%changing_w_sum/changing_error = change of weights/error in training set for each iteration
%testing outputs = resultMatrix
%testing error = testErrorM
nodesPerLayer = 3;
hiddenLayerNum = 1;
totalLayers = hiddenLayerNum+2;
alpha = 1;
max = 10;
if nodesPerLayer+1 > 10
    max = nodesPerLayer+1;
end

A = inputdata();
trainingMatrix = A(1:1039,:);

[w,wdisplay,changing_error,w_initial,a_initial,w_first_round,changing_w_sum] = ann(trainingMatrix,hiddenLayerNum,nodesPerLayer,alpha);

%testing
testM = ones(size(A,1), 10); %testM -> A with constant column
testM(:,2:10) = A(:,1:9);

resultMatrix = zeros(445,10);
testingError = zeros(445,1);
for index = 1040:1484
    y = zeros(1,10);
    class = testM(index,10);
    y(1,class) = 1;
    
    atest = zeros(totalLayers,max); %initialize a and z
    ztest = zeros(totalLayers,max);%outputs are a(3,1:10)
    for i = 1:totalLayers
        if i == 1 %input layer a = x
            atest(i,1:9) = testM(index,1:9);
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
    resultMatrix(index-1039,:) = atest(totalLayers,:);
    for i = 1:10
        testingError(index-1039,1) = testingError(index-1039,1) + abs(y(1,i)-resultMatrix(index-1039,i));
    end
end