var b real >= -infinity;
var objective >= -infinity;
param all[{1..500}*{1..32}] := read "data.dat" as "n+" comment "#";

set row := {1..500};
set col := {1..32};
var w[col] real >= -infinity;
var t[row] real >= -infinity;

minimize obj: objective;
subto constraint: forall <i> in row do t[i] >= 1-all[i,2]*(sum <k> in col with k >= 3: w[k]*all[i,k]+b) and t[i] >= 0;
subto constraint2: objective == (sum <i> in row: t[i]/500)+(sum <k> in col with k >= 3: 0.1*w[k]*w[k]);
