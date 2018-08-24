%%[result] = increase_size(img,height,width)
img = imread('inputSeamCarvingPrague.jpg');
energyImage = energy_image(img);
height_num = 10;
width_num = 10;

im = im2double(img);



energy = energyImage;
color = im;
[x1,y1] = size(energy);
horizontalList = zeros(height_num,y1);

for i=1:height_num
    cumulativeEnergyMap = cumulative_minimum_energy_map(energy, 'HORIZONTAL');
    horizontalSeam = find_optimal_horizontal_seam(cumulativeEnergyMap);
    
    horizontalList(i,:) = horizontalSeam;
    [color_temp,energy_temp] = reduce_height(color, energy);
    energy = energy_temp;
    color = color_temp;
end

%%do up-down stretching
top = 0;
bottom = 0;
%%2 seams to be inserted
new1 = zeros(height_num,y1,3);
new2 = zeros(height_num,y1,3);
for i = 1:height_num
    for j = y1
        curr_row = horizontalList(i,j);
        if curr_row == 0
            new1(i,j,:) = color(j,curr_row);
            new2(i,j,:) = (color(j,curr_row+1)+color(j,curr_row))/2;
        elseif curr_row == x1
            new1(i,j,:) = (color(j,curr_row-1)+color(j,curr_row))/2;
            new2(i,j,:) = color(j,curr_row);
        else
            new1(i,j,:) = (color(j,curr_row-1)+color(j,curr_row))/2;
            new2(i,j,:) = (color(j,curr_row+1)+color(j,curr_row))/2;
        end
    end
end

%%horizontalList items increase y by 1,2,3,4,5,6,7,...
add_list = [1:height_num]-1;
newcolor = zeros(x1,y1+sum(add_list),3);
for i = 1:height_num
    horizontalList(i,:) = horizontalList(i,:) + add_list(i);
    for j = 1:y1
        row_to_insert = horizontalList(i,j);
        if(row_to_insert ~= 1 && row_to_insert ~= x1)
            newcolor(1:row_to_insert-1,j,:) = color(1:row_to_insert-1,j,:);
            newcolor(row_to_insert+1:end,j,:) = color(row_to_insert+1:end,j,:);
        elseif(row_to_insert == x1)
            newcolor(:,j,:) = im(1:x1-1,j,:);
        else
            newcolor(3:end,j,:) = im(2:end,j,:);
        end
        newcolor(row_to_insert,j,:) = new1(i,j,:);
        newcolor(row_to_insert+1,j,:) = new2(i,j,:);
    end
end

[x2,y2] = size(energy);
verticalList = zeros(width_num,x2);

for i=1:width_num
    cumulativeEnergyMap = cumulative_minimum_energy_map(energy, 'VERTICAL');
    verticalSeam = find_optimal_vertical_seam(cumulativeEnergyMap);
    
    verticalList(i,:) = verticalSeam;
    [color_temp,energy_temp] = reduce_width(color, energy);
    energy = energy_temp;
    color = color_temp;
end

%%do left-right stretching 

