function val = diveval(xs,as,x)
%
%  Student_name: Xie Zhou
%  Student_id: 912143385 
%  Evaluate the polynomial p(x) of degree n-1 defined via the
%  formula
%
%    p(x) = a_1 + a_2 (x-x_1) + ... + a_n (x-x_1)...(x-x_{n-1})   (1)
%
%  at the point x.
%
%  Inputs:
%
%    xs - a vector of length n whose i^th entry specifies the
%         value of x_i
%
%    as - a vector of length n whose i^th entry specified the
%         value of a_i
%
%    x - a real number specifying the point at which to 
%         evaluate the polynomial p defined via (1)
%
%  Outputs:
%
%    val - the value of the polynomial p at the point x
%
%
val = as(1)
for i = 2:length(as)
	product = 1
	for j = 1:(i-1)
		product = product * (x - xs(j))
	end
	val = val + as(i) * product
end
end
