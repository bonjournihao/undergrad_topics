


n   = 15;
m   = 100;
fun = @(x) cos(x.^2);


%
%  Use an (n+1)-term Legendre expansion to approximate fun;
%  test the error in the approximation at m random points
%  in (-1,1).
%

as    = legecoefs(n,fun);
xs    = sort(rand(m,1)*2-1);
vals  = legeeval(n,as,xs);
vals0 = fun(xs);

errs = abs(vals - vals0);

maxerror = max(errs)

fprintf('maximum error = %f\n',maxerror);


% plot the magnitude of the coefficients and the magnitude of the error

figure;
plot(0:n,log10(abs(as)));
figure;
plot(xs,log10(abs(vals0-vals)));
