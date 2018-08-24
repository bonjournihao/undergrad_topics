

%  Test the evaluation of computation of Legendre coefficients


fun   = @(x) 1;
vals0 = [1 0 0 0 0 0]';
vals  = legecoefs(5,fun);

if (size(vals,1) == 1) 
vals=vals';
end

% the difference should be small
fprintf('difference = %f\n\n',max(abs(vals0 - vals)))




fun   = @(x) x.^2;
vals0 = [1/3 0 2/3 0 0 0]';
vals  = legecoefs(5,fun);

if (size(vals,1) == 1) 
vals=vals';
end


% the difference should be small
fprintf('difference = %f\n\n',max(abs(vals0 - vals)))


fun   = @(x) cos(x.^2);
vals0 = [0.904524237900272  -0.000000000000000  -0.269300533183747   ...
	  0.000000000000000  -0.100744891056225   0.000000000000000]';
vals  = legecoefs(5,fun);

if (size(vals,1) == 1) 
vals=vals';
end


% the difference should be small
fprintf('difference = %f\n\n',max(abs(vals0 - vals)))
