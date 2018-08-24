param M[{1..7}*{1..7}] := read "M.dat" as "n+" comment "#";
param P[{1..7}] := read "P.dat" as "n+" comment "#";

set N := {1..7};
var t[N] real >= -infinity;
var r[N] real >= -infinity;

minimize obj: sum<i> in N: t[i];
subto c1: forall <i> in N: 
		t[i] == sum <j> in N: (M[i,j]*r[j]-P[i])*(M[i,j]*r[j]-P[i]);
