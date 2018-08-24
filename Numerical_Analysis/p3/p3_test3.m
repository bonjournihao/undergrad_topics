
toler = 10^(-14);
%toler = 10^(-3);
f = @(x) 1./(1+x.^2) .* cos(333*x.^2);

val0 = 0.0343864808829704209358840867724;

for n=10:20
  val  = adapint(toler,n,f,0,2);
fprintf('difference = %d\n\n',abs(val0-val));
end
