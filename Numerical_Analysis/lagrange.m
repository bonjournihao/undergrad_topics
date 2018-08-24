function val =lagrange(xs,ys,x)
%
%  Student_name: Xie Zhou
%  Student_id: 912143385 
% Compute the value of the polynomial p(x) of degree n-1 whose graph
% passes through the specified collection (x_i,y_i), i=1,..,n, of
% points in the plane at the point x using the Lagrange formula.
%
% Inputs:
%
%
%  xs - a vector of length n whose iˆth entry specifies the 
%       value of x_i
%
%
%  ys - a vector of length n whose iˆth entry specifies the 
%       value of y_i
%
%
%   x - a real number specifying the point at which to evaluate the
%       interpolation polynomial
%
%
% Outputs:
%
%   val - a real number specifying the value of interpolating polynomial p 
%         at x
%
%
sum = 0
n = length(xs)
upper_product = ones(1, n)
lower_product = ones(1, n)
for i = 1 : n
	for j = 1 : n
		if (i == j)
			continue
		end
		upper_product(1, i) = upper_product(1, i) * (x - xs(j))
		lower_product(1, i) = lower_product(1, i) * (xs(i) - xs(j))
	end
	sum = sum + (upper_product(1, i) / lower_product(1, i)) * ys(i)
end

val = sum

end
