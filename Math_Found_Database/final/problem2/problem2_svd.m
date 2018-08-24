A = importdata('problem2.dat');
[s,v,q] = svd(A);
G = v(1,1)*s(:,1)*q(:,1)';
diff = A - G;
%%least significant column is the one with smallest sum of entries in diff
ranklist = zeros(1,7);
for i = 1:31
    for j = 1:7
        ranklist(1,j) = diff(i,j);
    end
end

[~,~,ranking] = unique(ranklist);
rating = ranking';

disp('Difficulty scores for problem 1 2 3 4 5 6 7 are:');
disp('(--with most difficult problem being 1--)');

disp('    #1    #2    #3    #4    #5    #6    #7');
disp(rating);

disp('Meaning of vector s: each student''s ability to get scores on the 7 problems with difficulty being the influence');
disp('---More details are explained in readMe.txt');