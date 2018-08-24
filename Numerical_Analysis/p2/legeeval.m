function vals = legeeval(n,as,xs)
%
% student_name:Xie Zhou
% student_name_id: 912143385
%
% function vals = legeeval(n,as,xs)
%
% Evaluate a (n+1)-term Legendre expansion
%
%   p(x) = a_0 P_0(x) + a_1 P_1(x) + ... + a_n P_n(x)
%
%  at a collection of user-specified points x_1,...,x_m in (-1,1).
%
%  Input parameters:
%
%    n - the number of terms in the expansion
%    as - a vector of length n+1 whose j^th entry specifies the
%        coefficient a_(j-1)
%    xs - a vector of length m whose j^th entry is the point x_j
%     
%
%  Output parameters:
%
%    vals - a vector of length m whose j^th entry is the value
%          of p at the point x_j
% 

vals = zeros(size(xs))

for j = 1:length(xs)
	Pk = zeros(1, n+1)
	% as getting each P_i, multiply by a_i;sum all up; insert into vals
	for i = 1:n+1
		if (i == 1)
			Pk(i) = Pk(i)+1
		elseif (i == 2)
			Pk(i) = Pk(i)+xs(j)
		else 
			Pk(i) = ((2*(i-1-1)+1)/((i-1-1)+1))*xs(j)*Pk(i-1) - ((i-1-1)/(i-1-1+1))*Pk(i-2)
		end
		vals(j) = vals(j)+as(i)*Pk(i)
		%disp(length(xs))
		%disp(j)
	end
end

end

