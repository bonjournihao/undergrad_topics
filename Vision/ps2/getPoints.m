function [p1_points, p2_points] = getPoints(im1, im2)
    [selectedMovingPoints,selectedFixedPoints] = cpselect(im1,im2,'Wait',true);
    
    p1_points = selectedMovingPoints';
    p2_points = selectedFixedPoints';
    
end