%uncertainty of unknown Sample 0.49 0.51 0.52 0.23 0.55 0.03 0.52 0.39
%sum of abs(mean-entry_i)
sample = [0.49 0.51 0.52 0.23 0.55 0.03 0.52 0.39];
A = inputdata();
sum = zeros(1,8);
avg = zeros(1,8);

for i = 1:1484
    for j = 1:8
        sum(1,j) = sum(1,j) + A(i,j);
    end
end

variation = 0;
featureSum = 0;
for i = 1:8
    avg(1,i) = sum(1,i)/1484;
    featureSum = featureSum + avg(1,i);
    variation = variation + abs(avg(1,i)-sample(i));
end

uncertainty = 100*variation/featureSum;
disp('variation: ');
fprintf('%f\n',variation);
disp('uncertainty: ');
fprintf('%%%f \n',uncertainty);
