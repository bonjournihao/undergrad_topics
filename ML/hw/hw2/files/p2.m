%calls function ann
%prints final activation functions
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

%testing training set
testM = ones(size(A,1), 10); %testM -> A with constant column
testM(:,2:10) = A(:,1:9);

resultMatrix = zeros(1484,10);
trainingError = zeros(1484,1);
for index = 1:1484
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
    resultMatrix(index,:) = atest(totalLayers,:);
    for i = 1:10
        trainingError(index,1) = trainingError(index,1) + abs(y(1,i)-resultMatrix(index,i));
    end
end

%calculating error
trainingYs = testM(:,10);
count = 0;
errorSum = 0;
for i = 1:1484
    largest = 0;
    index = -1;
    for j = 1:10
        if largest < resultMatrix(i,j)
            largest = resultMatrix(i,j);
            index = j;
        end
    end
    if trainingYs(i) == index
        count = count + 1;
    end
    errorSum = trainingError(i,1) + errorSum;
end

accuracy = count/1484;
disp('accuracy:');
disp(accuracy);
disp('errorSum:');
disp(errorSum);
%activation functions a_i = 1./(1-exp(-z))
%z = wx / = wa
%layer 2: a_{1}2 = g(z_{1}2) a_{2}2 = g(z_{2}2) a_{3}2 = g(z_{3}2)
%         z_{1}2 = w(1,1,0)x0+w(1,1,1)x1,+...+w(1,1,9)x9
%         z_{2}2 = w(1,2,0)x0+w(1,2,1)x1,+...+w(1,2,9)x9......
%layer 3: a_{1}3 = g(z_{1}3) .... a_{10}3 = g(z_{10}3)
%         z_{1}3 = w(2,1,0)x0+w(2,1,1)a_{1}2+...+w(2,1,3)a_{3}2
%     ....z_{10}3 = w(2,10,0)x0+w(2,10,1)a_{1}2+...+w(2,10,3)a_{3}2

disp('layer2 w_0 = 1/(1-e^(-(0.5799 + 0.2050*x1 + 0.0185*x2 + 0.7567*x3 + 0.2863*x4 + 0.2050*x5 + 0.7936*x6 + 0.8179*x7 + 0.2333*x8)))');
disp('layer2 a_1 = 1/(1-e^(-(2.4306 + 1.3397*x1 + 1.4137*x2 + 1.3028*x3 + 1.2038*x4 + 1.2391*x5 + 0.8369*x6 + 1.5090*x7 + 0.6184*x8)))');
disp('layer2 a_2 = 1/(1-e^(-(0.7457 + 0.2568*x1 + 1.0288*x2 + 0.3887*x3 + 0.2940*x4 + 0.2364*x5 + 0.9021*x6 + 0.1357*x7 + 0.2316*x8)))');
disp('layer2 a_3 = 1/(1-e^(-(1.9900 + 1.0449*x1 + 1.4448*x2 + 0.8066*x3 + 1.0059*x4 + 0.9440*x5 + 0.5672*x6 + 0.7562*x7 + 0.2590*x8)))');
disp('layer3 a_1 = 1/(1-e^(-(-0.3705*w0 - 0.1689*a_1(layer2) - 0.4609*a_2(layer2) - 0.2373*a_3(layer2))))');
disp('layer3 a_2 = 1/(1-e^(-(0.0151*w0 - 0.3841*a_1(layer2) + 0.0524*a_2(layer2) - 0.9907*a_3(layer2))))');
disp('layer3 a_3 = 1/(1-e^(-(-0.8521*w0 - 0.8275*a_1(layer2) - 0.3918*a_2(layer2) - 0.0651*a_3(layer2))))');
disp('layer3 a_4 = 1/(1-e^(-(-0.8708*w0 - 0.4023*a_1(layer2) - 0.5735*a_2(layer2) - 0.6999*a_3(layer2))))');
disp('layer3 a_5 = 1/(1-e^(-(-1.3159*w0 - 1.0144*a_1(layer2) - 0.7337*a_2(layer2) - 0.6985*a_3(layer2))))');
disp('layer3 a_6 = 1/(1-e^(-(-1.0290*w0 - 1.0607*a_1(layer2) - 0.6715*a_2(layer2) - 0.2691*a_3(layer2))))');
disp('layer3 a_7 = 1/(1-e^(-(-1.0740*w0 - 0.6608*a_1(layer2) - 1.6932*a_2(layer2) - 0.6491*a_3(layer2))))');
disp('layer3 a_8 = 1/(1-e^(-(-1.0353*w0 - 0.9082*a_1(layer2) - 0.8234*a_2(layer2) - 1.1014*a_3(layer2))))');
disp('layer3 a_9 = 1/(1-e^(-(-1.7476*w0 - 0.9713*a_1(layer2) - 0.9509*a_2(layer2) - 0.7257*a_3(layer2))))');
disp('layer3 a_10 = 1/(1-e^(-(-1.4387*w0 - 1.0934*a_1(layer2) - 1.1992*a_2(layer2) - 1.0989*a_3(layer2))))');



