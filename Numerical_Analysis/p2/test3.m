%
%  Test the evaluation of Legendre expansions.
%

n     = 4;
as    = [1 0 1 0 1];
xs    = [1/2 1/4 -1/4 -1/2];
vals0 = [0.585937500000000   0.751464843750000   0.751464843750000   0.585937500000000];

vals = legeeval(n,as,xs);
if (size(vals,2)==1) 
vals=vals';
end

fprintf('difference = %f\n\n',max(abs(vals-vals0)))


n     = 14;
as    = [
   0.891426565922927
  -0.000000000000000
  -0.290967837403369
   0.000000000000000
  -0.093541486490263
   0.000000000000000
   0.006436521588142
   0.000000000000000
   0.000628947395610
   0.000000000000000
  -0.000029861849906
  -0.000000000000000
  -0.000001457896701
  -0.000000000000000
   0.000000058587431
  -0.000000000000000
   0.000000001677466
  -0.000000000000000
  -0.000000000062885
  -0.000000000000001
  -0.000000000001136
  -0.000000000000002
   0.000000000000042
  -0.000000000000000
   0.000000000000001
   0.000000000000002
   0.000000000000001
   0.000000000000001
   0.000000000000002
  -0.000000000000002
   0.000000000000001];

xs    = [1/2 1/4 -1/4 -1/2];
vals0 = [0.956876398567619   0.994933480608647   0.994933480608647   0.956876398567619];

vals = legeeval(n,as,xs);

if (size(vals,2)==1) 
vals=vals';
end

fprintf('difference = %f\n\n',max(abs(vals-vals0)))
