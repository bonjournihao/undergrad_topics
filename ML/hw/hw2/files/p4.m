%hiddenLayerNum: 1,2,3
%nodesPerLayer: 3,6,9,12
%call function ann
A = inputdata();
trainingMatrix = A(1:1039,:);
alpha = 0.1;
max = 10;
testingErrorM = zeros(3,4);

count = zeros(3,4);
accuracy = zeros(3,4);
for ii = 1:3
    for jj = 1:4
        hiddenLayerNum = ii;
        nodesPerLayer = jj*3;
        totalLayers = hiddenLayerNum+2;
        max = 10;
        if nodesPerLayer+1 > 10
            max = nodesPerLayer+1;
        end
        [w,wdisplay,error,w_initial,a_initial,w_first_round,changing_w_sum] = ann(trainingMatrix, hiddenLayerNum,nodesPerLayer,alpha);
        
        %testing
        testM = ones(size(A,1), 10); %testM -> A with constant column
        testM(:,2:10) = A(:,1:9);

        resultMatrix = zeros(445,max);
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
        %sum error
        %caluculate count for accuracy 
        testingYs = testM(1040:1484,10);
        for k = 1:445
            largest = 0;
            index = -1;
            for j = 1:10
                if largest < resultMatrix(k,j)
                    largest = resultMatrix(k,j);
                    index = j;
                end
            end
            if testingYs(k) == index
                count(ii,jj) = count(ii,jj) + 1;
            end
            testingErrorM(ii,jj) = testingErrorM(ii,jj) + testingError(k,1);
        end
        accuracy(ii,jj) = count(ii,jj)/445;
    end
end
%more nodes per hidden layer, larger error
%more hidden layers, more error with alpha > 1;
%more hidden layers, prediction with less variation (looking at resulting matrix for 445 samples)