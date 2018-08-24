function [val der] = legeder(n,x)
%  function [val der] = legeder(n,x)
%
%  Return the value of the Legendre polynomial of degree n and its derivative
%  at the point x.
%
%  Input parameters:
%    n - the degree of the polynomial to evaluate
%    x - a real number in the interval (-1,1)
%
%
%  Ouput parameters:
%    val - the value of the polynomial at the point x
%    der - the value of the derivative of the polynomial at the point x
%

val = 0;
der = 0;
Pj=zeros(1, n+1);
Pj_d = zeros(1, n+1);

for i = 1:n+1
    if (i == 1)
        Pj(i) = 1;
    elseif (i == 2)
        Pj(i) = x;
    else
        Pj(i) = ((2*(i-1)-1)/(i-1))*x*Pj(i-1)-((i-1-1)/(i-1))*Pj(i-2);
    end
val = Pj(n+1);
end

for i =1:n+1
    if (i==1)
        Pj_d(i) = 0;
    else
        Pj_d(i) = (x*Pj(i)-Pj(i-1))*((i-1)/(x^2-1));
    end
    der = Pj_d(n+1);
end

end
