

%  Test the evaluation of the Legendre polynomial of order 5 at
%  a collection of points.

n  = 5;
xs = [1/4 -1/4 1/2 1/3 0 1/10];
vals0 = [0.339721679687500  -0.339721679687500   0.089843750000000   ...
         0.333333333333333                   0   0.178828750000000];

vals = legendre(n,xs);

% display the difference ... which should be very small!

fprintf('difference = %f\n\n', max(abs(vals-vals0)))

