set nums := {1,2,3,4,5};
param r[nums] := <1>3, <2>5, <3>7, <4>4, <5>8;
var x[nums] real >= -infinity;
var y[nums] real >= -infinity;
var half real;

minimize obj: half;
subto a: forall <i> in nums do
		forall <j> in nums with j>i do
			(x[i]-x[j])^2+(y[i]+y[j])^2 >= (r[i]+r[j])^2;
subto b: forall <k> in nums do
		x[k] <= half-r[k] and y[k]<=half-r[k];
subto c: forall <l> in nums do
		x[l] >= -half+r[l] and y[l]>=-half+r[l];
