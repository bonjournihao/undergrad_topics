function val = adapint(toler,n,f,a,b)
%
%  Use the adaptive integration procedure described in class to
%  approximate
%
%        b
%    \int f(x) dx.                                                 (1)
%        a 
%
%  The (n+1)-point Clenshaw-Curtis quadrature rule should be used on
%  each subinterval.
%
%  Input parameters:
%
%    toler - the numerical tolerance for the procedure (as described in class)
%    n - an integer parameter speciying the length of the rule to use
%    f - a MATLAB function handle
%    (a,b) - the interval of integration
%
%  Output parameters:
%
%    val - the obtained approximation of (1)
%
%

val = 0;
dif = 0;

val = oneint(n,f,a,b);
m = (a+b)/2;

dif = abs(val-oneint(n,f,a,m)-oneint(n,f,m,b));
if dif > toler
	val = adapint(toler,n,f,a,m) + adapint(toler,n,f,m,b);
else
	return;
end


end
