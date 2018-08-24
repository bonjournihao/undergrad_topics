function [warpIm, mergeIm] = warp2(inputIm, refIm, H_matrix)
    input = im2double(inputIm);
    ref = im2double(refIm);
    
    m_ref = size(ref,1); 
    n_ref = size(ref,2);
    m_input = size(input,1);
    n_input = size(input,2);

    %%forward warping
    [X1,Y1] = meshgrid(1:n_input,1:m_input);
    pp = [X1(:),Y1(:)];
    pp_1 = pp';
    temp_oness = ones(1,size(pp_1,2));
    pp_2 = [pp_1;temp_oness];
    pp_prime = H_matrix*pp_2;
    pp_prime_w = pp_prime(3,:);
    pp_prime_1 = pp_prime(1,:)./pp_prime_w;
    pp_prime_2 = pp_prime(2,:)./pp_prime_w;
    x_min = round(min(pp_prime_1));
    y_min = round(min(pp_prime_2));
    x_max = round(max(pp_prime_1));
    y_max = round(max(pp_prime_2));

    [Xp,Yp] = ndgrid(x_min:x_max,y_min:y_max);
    [width,height] = size(Xp);

    %%inverse wrapping
    p_prime = [Xp(:),Yp(:)];

    p_prime_1 = p_prime';
    temp_ones = ones(1,size(p_prime_1,2));
    p_prime_2 = [p_prime_1;temp_ones];%% ref image points

    p = inv(H_matrix)* p_prime_2;
    p_w = p(3,:);
    p_1 = p(1,:)./p_w;

    p_2 = p(2,:)./p_w; %%mapped input image points

    %%bilinear interpolation + map to new_ref
    warpIm = zeros(height,width,3);

    x = reshape(p_1,width,height)'; %%width is y
    y = reshape(p_2,width,height)'; %%height is x
    warpIm(:,:,1) = interp2(input(:,:,1),x,y);
    warpIm(:,:,2) = interp2(input(:,:,2),x,y);
    warpIm(:,:,3) = interp2(input(:,:,3),x,y);

    new_height = max(m_ref,height);
    new_width = max(n_ref,width);
    new_warp = zeros(new_height,new_width,3);
    new_ref = zeros(new_height,new_width,3);
    rx = max(1,1-y_min);
    ry = max(1,1-x_min);
    
    wx = max(1,y_min);
    wy = max(1,x_min);
    
    new_ref(rx:rx+m_ref-1,ry:ry+n_ref-1,:) = ref(:,:,:);
    new_warp(wx:wx+height-1,wy:wy+width-1,:) = warpIm(:,:,:);
    
    selected = new_warp(rx:rx+m_ref-1,ry:ry+n_ref-1,:);
    
    mergeIm = imfuse(new_ref,new_warp,'blend');
           
    figure;
    imshow(selected);
    title('image mosaic');
end