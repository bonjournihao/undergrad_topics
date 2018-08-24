function [xs whts] = legequad(n)
%  function [xs whts] = legequad(n)
%
%  Return the n-point Gauss-Legendre quadrature rule on the interval
%  (-1,1).  
%
%  Input parameters:
%    n - the length of the quadrature rule to compute
%
%  Output parameters:
%    xs - an (n,1) array containing the quadrature nodes; they are IN INCREASING
%      ORDER
%
%    whts - an (n,1) array containing the corresponding quadrature weights
%   
%

xs = zeros(n,1);
whts = zeros(n,1);
P = 0;
Pd = 0;


for i=1:n
    z0 = cos(pi*i/(n+1));
    xs(i) = legeroot(n,z0);
end

for i=1:n
    [P,Pd] = legeder(n+1,xs(i));
    whts(i) = (2*(1-xs(i).^2))/((n+1)^2*P^2);
    
end
