function val = oneint(n,f,a,b)
%  function val = oneint(n,f,a,b)
%
%  Approximate the value of integral
%
%        b
%    \int   f(x) dx
%        a
%
%  using the (n+1)-point Clenshaw-Curtis quadrature returned by
%  clenshaw.m.
%
%  Input parameters:
%    n - an integer specifying the length of the quadrature rule to
%     use; the (n+1)-point rule should be used
%    f - a MATLAB function handle
%    a - the left endpoint of the interval over which to integrate
%    b - the right endpoint of the interval over which to integrate
%
%
%
%
val = 0;
xs   = zeros(n+1,1);
whts = zeros(n+1,1);

[xs,whts] = clenshaw(n);

for i = 1:n+1
    coef = (b-a)/2;
    val = val + coef*(f(coef*xs(i)+((b+a)/2))*whts(i));
end

end
