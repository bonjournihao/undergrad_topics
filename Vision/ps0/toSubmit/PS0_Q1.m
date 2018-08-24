input = matfile('PS0_A.mat');
A = input.A;
list = A(:);
sorted = sort(list,'descend');

%%display figure 4a
figure3 = figure;
axes1 = axes('Parent',figure3);
hold(axes1,'on');

plot(sorted);
xlabel({'pixels'});
ylabel({'intensities'});
title({'Intensities of 2500 Pixels in Descending Order'});
box(axes1,'on');

%%display figure 4b
figure2 = figure;
axes2 = axes('Parent',figure2);
hold(axes2,'on');
histogram('Parent',axes2,...
    'BinEdges',[0 26 52 78 104 130 156 182 208 234 260],...
    'BinCounts',[232 232 261 247 252 277 240 251 286 222]);
xlabel('Intensities');
title({'Intensities of Pixels'},'HorizontalAlignment','center',...
    'FontWeight','bold',...
    'FontSize',14);
ylabel('Number of Pixels','HorizontalAlignment','center');
box(axes2,'on');
set(axes2,'XTick',[0 26 52 78 104 130 156 182 208 234 260],'XTickLabel',...
    {'0','26','52','78','104','130','156','182','208','234','260'});

%%display figure 4c
Z = A(26:50,1:25);
figure;
imagesc(Z);
colormap('gray');

%%display figure 4d
mean_int = mean(list);
W = A - mean_int;
%%W(W<0) = 0;
figure;
title('W')
imagesc(W);
colormap('gray');

%%display figure 4e
R = zeros(50,50);
G = zeros(50,50);
B = zeros(50,50);
im2show = zeros(50,50,3);

indices = find(A > mean_int);
R(indices) = 255;

im2show(:,:,1) = R;
im2show(:,:,2) = G;
im2show(:,:,3) = B;
figure;
%%imshow(im2show);
imshow(im2show, 'InitialMagnification', 1600);


