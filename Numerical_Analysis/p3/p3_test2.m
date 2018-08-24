

f = @(x) (x-1/4).^2.*cos(16*x.^2).^2;
val0 = 0.0191017854417770942140902441840;

for n=2:2:30
val = oneint(n,f,1/4,3/4);
fprintf('n = %d,  difference = %d\n',n,abs(val0-val));
end


