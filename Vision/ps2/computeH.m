function H = computeH(t1, t2) %%2*n matrices
    A = [];
    [~,pairNum] = size(t1);
    for i=1:pairNum
        line1 = [t1(1,i) t1(2,i) 1 0 0 0 -t2(1,i)*t1(1,i) -t2(1,i)*t1(2,i) -t2(1,i)];
        line2 = [0 0 0 t1(1,i) t1(2,i) 1 -t2(2,i)*t1(1,i) -t2(2,i)*t1(2,i) -t2(2,i)];
        temp = [line1;line2];
        A = vertcat(A,temp);
    end
    
    m = A'*A;
    [vec,val] = eig(m);
    list = zeros(1,size(val,1));
    
    for i=1:size(val,1)
        list(1,i) = sum(val(:,i));
    end
    [~,index] = min(list);
    
    H = vec(:,index);
end