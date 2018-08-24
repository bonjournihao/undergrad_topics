param A[{1..240}*{1..5}] := read "problem1_revisedData.dat" as "n+" comment "#";
param C[{1..5}*{1..5}] := read "C_abs.dat" as "n+" comment "#";
var x[{1..5}*{1..5}] binary;
set num := {1..5};


maximize obj: sum<i> in num: sum<j> in num: C[i,j]*x[i,j];
subto c1: forall <i> in num: forall <j> in num with j!= i: x[i,j] + x[j,i] == 1;
subto c2: forall <i> in num: forall <j> in num with j != i:
                forall <k> in num with(k != i and k != j): x[i,j] + x[j,k] + x[k,i] <= 2;
