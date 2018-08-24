Student ID: 912143385
Student name: Xie Zhou

<Final Project--Written part>

Problem1:

Runnable Files: problem1_plurality.m problem1_avgRank.m problem1_borda.m problem1_Wborda.m problem1_Yamamoto.zpl problem1_Yamamoto_absolute.zpl problem1_Yamamoto_result.txt

The predictions are reported after each runnable file being run. 

The results for problem1_Yamamoto.zpl and problem1_Yamamoto_absolute.zpl are stored in problem1_Yamamoto_result.txt.

-> In my opinion, Yamamoto's model is the fairest because it compares every pair of candidates (# times candidate i is ahead of j - # times candidate j is ahead of i). The comparison is relative to each pair and therefore is the most representative. 

-> Plurality method is not reliable when there is an extreme situation. For example, one candidate might have the largest number of people that voted him as 1st, but also a lot of 5th place votes. In this case, only counting the number of 1st votes is not enough. 

-> Average rank and borda's method are also not reliable in this case because the resulting position/value could be very similar and it would be hard to rank.
->W-borda method is also not good because the way of designing the W vector could largely change the ranking result.

Problem 2:

Runnable Files: problem2_svd.m problem2_massey.zpl problem2_massey_result.txt problem2_colley.m

The predictions are reported after each runnable file being run.

-> SVD method interpretation:
Taking the top singular value decomposition will produce a matrix G that is an approximation of the original matrix A. Because G is generated with the largest singular value, G represents matrix A's features under the inluence of the problem's difficulty (difficulty corresponds to the scores for each problem; harder problems have lower scores). The difference of matrix A and matrix G is A - G. Columns of A - G represents their influence on A under the feature, "difficulty". Now, the task is to give an ordering of the columns(which represent the problems) of A - G. Because students get lower scores on harder problems, and problems with lower scores have less influence on the overall score, -> the columns in A - G with smaller value represent the problems that have less influence on the over all score, which are the more difficult ones.

Vector s represents the 31 students' ability to score on the 7 problems with "difficulty" being the factor. 

->problem2_massey.zpl interpretation:
Rating exam problems by difficulty is similar to the class example (rating movies). In the case of this problem, the ratings of each pair of movies from each user becomes the score of each pair of problems of each student. In this case, we are getting the differential scores by subtracting the score of problem j from the score of problem i in each competition. Each student represents a game where there are 6 competitions between pairs of problems. Each competition is an arc with flow value Si - Sj in the network.

The result for problem2_massey.zpl is stored in problem2_massey_result.txt

->problem2_colley.m
Colley's method produced the same ranking result as Massey's method.


