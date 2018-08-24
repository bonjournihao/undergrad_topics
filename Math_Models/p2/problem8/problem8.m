A = importdata('breastcancer.mat'); 
idx = kmeans(A(:,2:10), 2);

 a = 0;
 b = 0;
 c = 0;
 d = 0;
 for(i=1:683)
     if(A(i,11) == 2 & idx(i)==2)
         a=a+1;
     end
     if(A(i,11) == 4 & idx(i)==1)
         b=b+1;
     end
     if(A(i,11) == 2 & idx(i)==1)
         c=c+1;
     end
     if(A(i,11) == 4 & idx(i)==2)
         d=d+1;
     end
 end
 t1 = 0;
 t2 = 0;
 t3 = 0;
 t4 = 0;
 
 false_positive = 0;
 false_nagative = 0;
 if(a>c)
     M = [b;c];
     B = [d;a];
     t1 = t1+b;
     t2 = t2+d;
     t3 = t3+c
     t4 = t4+a;
     false_nagative = d/(b+d);
     false_positive = c/(a+c);
 else
     M = [d;a];
     B = [b;c];
     t1 = t1+d;
     t3 = t3+a;
     t2 = t2+b;
     t4 = t4+c;
     false_nagative = b/(d+b);
     false_positive = a/(c+a);
 end
 
 f = figure('Position',[400 500 700 100]);
 d = [M B];
 cnames = {'M','B'};
 rnames = {'M','B'};
 t = uitable(f,'Data',d,'ColumnName',cnames,'RowName',rnames,'ColumnWidth',{100});

 t.Position(3) = t.Extent(3);
t.Position(4) = t.Extent(4);

%%c)

%%repete for 9 more times
for j= 2:10 
    idx = kmeans(A(:,2:10), 2);
    false_positive = 0;
    false_nagative = 0;
    a=0;
    b=0;
    c=0;
    d=0;
 for i=1:683
     if(A(i,11) == 2 & idx(i)==2)
         a=a+1;
     end
     if(A(i,11) == 4 & idx(i)==1)
         b=b+1;
     end
     if(A(i,11) == 2 & idx(i)==1)
         c=c+1;
     end
     if(A(i,11) == 4 & idx(i)==2)
         d=d+1;
     end
 end
 if(a>c)
     t1 = t1+b;
     t2 = t2+d;
     t3 = t3+c
     t4 = t4+a;
 else
     t1 = t1+d;
     t3 = t3+a;
     t2 = t2+b;
     t4 = t4+c;
 end
end
M = [t1/10;t2/10];
B = [t3/10;t4/10];

 f = figure('Position',[200 500 700 100]);
 d = [M B];
 cnames = {'M','B'};
 rnames = {'M','B'};
 t = uitable(f,'Data',d,'ColumnName',cnames,'RowName',rnames,'ColumnWidth',{100});
 t.Position(3) = t.Extent(3);
t.Position(4) = t.Extent(4);
