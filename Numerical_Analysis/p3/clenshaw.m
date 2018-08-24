function [xs,whts] = clenshaw(n)
%  function [xs, whts] = clenshaw(n)
%
%  Return the nodes and weights of the (n+1)-point Clenshaw-Curtis
%  quadrature we derived in class.  A derivation of it is also 
%  outlined in Homework 4.  
%
%  The (n+1)-point Clenshaw-Curtis quadrature rule is the formula
%  of the form
%
%         1                    n
%    \int f(x) dx  \approx   \sum  f(x_k) w_k                 (1)
%        -1                   k=0
%
%  such that
%
%     x_k = \cos((2*k+1)/(2*n+1) pi)   for k=0,1,..,n         (2)
%
%  and the weights w_0,...w_n are chosen so that the quadrature
%  rule is exact for polynomials of degree less than or equal to n.
%
%  Input parameters:
%    n - the degree of polynomials for which the formula (1) is exact;
%      the number of points in the rule will be n+1
%
%  Output parameters:
%     xs - a row vector of length n+1 whose j^th entry contains the
%      nodes x_j defined via (2)
%
%     whts - a row vector of length n+1 whose j^th entry contains
%      the weight w_j
%
% 

xs   = zeros(n+1,1);
whts = zeros(n+1,1);
%d = floor(n/2)


for k = 1:n+1
    xs(k) = cos(((2*(k-1)+1)/(2*n+1))*pi);
    if k == (n+1)
		sum1 = 0;
		for i=1:floor(n/2)
			sum1 = 1/(1-4*(i^2)) + sum1;
		end
		whts(k) = (4/(2*n+1))*(0.5+sum1);
    else
		sum2 = 0;
		for i=1:floor(n/2)
			sum2 = (1/(1-4*(i^2)))*(cos(2*i*((2*(k-1)+1)/(2*n+1))*pi))+sum2;
		end
		whts(k) = (8/(2*n+1))*(0.5+sum2);
    end
end

end
