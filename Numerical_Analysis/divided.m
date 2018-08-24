function as = divided(xs,ys)
%
%  Student_name: Xie Zhou
%  Student_id: 912143385 
% Compute the coefficients a_1, ... a_n in the representation
%
%
%  p(x) = a_1 + a_2 (x-x_1) + ... + a_n (x-x_1) ... (x-x_{n-1})          (1)
%
% of the polynomial of degree n-1 whose graph passes through a
% specified collection (x_i,y_i), i=1,...,n, of points in the plane.
%
%
% Inputs:
%
%
%  xs - a vector of length n whose iˆth entry specifies the value
%       of x_i
%
%
%  ys - a vector of length n whose iˆth entry specifies the value
%       of y_i
%
%
% Outputs:
%
%
%  as - a vector of length n whose iˆth entry specifies the value
%       of the coefficient a_i in the representation (1)
%
%
%
n = length(xs)
F(1 : length(xs), 1) = xs
F(1 : length(ys), 2) = ys

for j = 1:n-1
	for i = 1:n-j
	    F(i, j+2) = (F(i+1, j+1) - F(i, j+1))/(F(i+j, 1) - F(i, 1))
        end
end

as = F(1,2:end)

end
