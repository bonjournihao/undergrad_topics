function vals = legendre(n,xs)
%
% student_name:Xie Zhou
% student_name_id: 912143385
%
% functions vals = legendre(n,xs)
%
% Return the values of the (n+1) Legendre polynomial of degree
  % n at a collection of points x_1,...,x_m in (-1,1).
%
% Input parameters:
%   n - the degree of the polynomial to evaluate
%   xs - a vector of length m whose jth entry specifies the point x_j
%
% Output parameters:
%   vals - a vector of length m whose jth entry specifies the value
%          of the Legendre polynomial of degree n at the point x_j
%

vals = zeros(size(xs))

for j = 1:length(xs)
	Pj = zeros(1, n+1)
	%insert each P_i(x) value into vals array
	for i = 1:n+1
		if (i == 1)
			Pj(i) = Pj(i)+1
		elseif (i == 2)
			Pj(i) = Pj(i)+xs(j)
		else 
			Pj(i) = ((2*(i-1-1)+1)/((i-1-1)+1))*xs(j)*Pj(i-1) - ((i-1-1)/(i-1-1+1))*Pj(i-2)
		end
	end
	vals(j) = Pj(n+1)
end


end
