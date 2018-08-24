function root = legeroot(n,z0)
%  function root = legeroot(n,z0)
%
%  Approximate a root of the Legendre polynomial of degree n using Newton's
%  method with z0 as the initial guess.
%
%  Input parameters:
%    n - an integer speciying the degree of the polynomial
%    z0 - a real number specifying the initial guess for Newton's method
%
%  Ouput parameters:
%    root - the value of the root which is obtained by Newton's method
%    

root = 0;
next = z0;
P = 0;
Pd = 0;

error = 1;

while error >= 10^(-13)
    root = next;
    [P,Pd] = legeder(n,root);
    next = root - P/Pd;
    error = abs(next-root);
end
end
