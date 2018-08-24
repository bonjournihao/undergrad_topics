%inputs: hiddenLayerNum,nodesPerLayer,training Matrix,learning rate

%A = inputdata();
%hiddenLayerNum = 1;
%nodesPerLayer = 3;
%alpha = 0.0001;

function [w,wdisplay,error,w_initial,a_initial,w_first_round,changing_w_sum] = ann(Matrix, hiddenLayerNum,nodesPerLayer,alpha)

%add constant 1 column to x
M = ones(size(Matrix,1), 10);
M(:,2:10) = Matrix(:,1:9);

totalLayers = hiddenLayerNum+2;

%inputlayer x = (8+1)*1, w1 = nodesPerLayer * (8+1) z1 = none
%hiddenlayer z2 = w1x1 = nodesPL*1 a2 =nodesPerLayer*1   w2 = 10*(nodesPerLayer+1) 
%outputlayer z3 = w2a2 =  a3 = 10*1  w3 = none

%1)calculate a to top layer
%2)display weights, prediction, error
%3)update w backwards
max = 10;
if nodesPerLayer+1 > 10
    max = nodesPerLayer+1;
end

%initialize w
w = zeros(totalLayers,max,max);
w_last = w;
changing_w_sum = zeros(size(M,1),1);


for i = 1:totalLayers-1
    if i == 1 %input layer
        temp = rand(nodesPerLayer+1,9);
        w(i,1:nodesPerLayer+1,1:9) = temp(:,:);
    elseif i == totalLayers-1 %just below output layer
        temp = rand(10,nodesPerLayer+1);
        w(i,1:10,1:nodesPerLayer+1) = temp(:,:);
    else %hidden layer
        temp = rand(nodesPerLayer+1,nodesPerLayer+1);
        w(i,1:nodesPerLayer+1,1:nodesPerLayer+1) = temp(:,:);
    end
end


error = zeros(size(M,1),1);
for index = 1:size(M,1)
    y = zeros(1,10);
    class = M(index,10);
    y(1,class) = 1;
    
    a = zeros(totalLayers,max);
    z = zeros(totalLayers,max);
    
    %feed forward
    %calculate a,z for each layer
    for i = 1:totalLayers
        if i == 1 %input layer a = x
            a(i,1:9) = M(index,1:9);
        elseif i < totalLayers %hidden layers
            for j = 1:nodesPerLayer
                for r = 1:max
                    z(i,j) = z(i,j) + w(i-1,j,r)*a(i-1,r); 
                end
            end
            %z(i,1:nodesPerLayer) = w(i-1)*a(i-1); 
            for j = 1:nodesPerLayer+1
                if j == 1
                    a(i,j) = 1;
                else
                    a(i,j) = 1./(1+exp(-z(i,j-1)));
                end
            end
        else %output layer
            for j = 1:10
                for r = 1:max
                    z(i,j) = z(i,j) + w(i-1,j,r)*a(i-1,r); 
                end
            end
            for j = 1:10
                a(i,j) = 1./(1+exp(-z(i,j)));
            end
        end
    end
    
    for i = 1:10
        error(index) = error(index) + abs(y(1,i)-a(totalLayers,i));
    end
    

    %w TO wdisplay (adjust to displayable dimension)
    ws = zeros(max,max,totalLayers);
    for i = 1:totalLayers
       for j = 1 : max
           for k = 1:max
               ws(j,k,i) = w(i,j,k);
           end
       end
    end
    
    if index == 1
        a_initial = a;
        w_initial = ws;

    elseif index == 2
        w_first_round = ws;
    else
    end
    
    %back propagate
    sigma = zeros(totalLayers,max);
    for layers = 1:totalLayers  %update w(l-1) each time
        l = totalLayers - layers + 1;
        if l ~= 1 %not update for w(0)
            if l == totalLayers %output to hidden
                for k = 1:10 %output layer sigmas
                    sigma1 = y(1,k)-a(l,k);
                    sigma2 = a(l,k)*(1-a(l,k));
                    sigma(l,k) = sigma1*sigma2;
                    
                    for j = 1:nodesPerLayer+1 %update w
                        w(l-1,k,j) = w(l-1,k,j)-alpha*(-sigma(l,k)*a(l-1,j));
                    end
                end
            else %hidden to blahblahblah
                %check last layer is output or not
                %assign lastNodeNum to 10/nodesPerLayer
                if (l+1) == totalLayers
                    lastNodeNum = 10;
                else
                    lastNodeNum = nodesPerLayer+1;
                end
                %check next layer is input or not
                %assign nextNodeNum to 9/or nodesPerLayer
                if l == 2
                    nextNodeNum = 9;
                else
                    nextNodeNum = nodesPerLayer+1;
                end
                %update w
                for j = 1:nodesPerLayer+1
                    sum = 0;
                    for k = 1:lastNodeNum
                        sum = sum+sigma(l+1,k)*w(l,k,j);
                    end
                    sigma(l,j)=sum*a(l,j)*(1-a(l,j));
                    for i = 1:nextNodeNum %update w
                        w(l-1,j,i) = w(l-1,j,i)-alpha*(-sigma(l,j)*a(l-1,i));
                    end
                end
            end
        end
    end
    
    %calculate and store weight change (sum)
    weight_change = 0;
    for i = 1:totalLayers
        for j = 1:max
            for k = 1:max
                weight_change = weight_change + abs(w(i,j,k)-w_last(i,j,k));
            end
        end
    end
    changing_w_sum(index,1) = weight_change;
    w_last = w;
end

wdisplay = ws;
end
