function as = legecoefs(n,fun)
%
% student_name:Xie Zhou
% student_name_id: 912143385
%
%  function coefs = legecoefs(n,funs)
%
%  Return the (n+1) coefficients a_0, a_1, ..., a_n such that
%
%    p(x) = a_0 P_0(x) + a_1 P_1(x) + ... + a_n P_n(x),
%
%  where P_j denotes the Legendre polynomial of degree j, is the 
%  unique polynomial of degree n which minimizes the quanitity
%
%          1 
%     \int      |p(x)-f(x)|^2 dx.
%         -1
%
%  Here, f is a user-specified external function.  Of course,
%  a_j is equal to
%
%                               1
  %     a_j = (2j+1)/2     \int   P_j(x) f(x) dx
%                              -1
%
%
%  Input parameters:
%    n - an integer specifying the  order of the approximating polynomial
%    fun - an external function which takes as input a vector of doubles
%      xs and returns the vector of doubles ys obtained by evaluating
%      f at each of the entries of the array xs; for example:
%
%              fun = @(x) x.^2
%
%              fun([1 2]) returns [2 4]
%
%
%  Output parameters:
%    as - a vector of length n+1 whose j_th entry is the value		   
%         of the coefficient a_(j-1)
% 
%
%

as = zeros(n+1,1)

for j=0:n
	f = @(x) fun(x).*legendre(j,x)
	as(j+1, 1) = ((2*j+1)/2) .* integral(f, -1, 1)
end
end
