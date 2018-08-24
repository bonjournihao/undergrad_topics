set freq := {20,14,8,24};
set nums := {1,2,3,4};
param xs[nums] := <1>3,<2>10,<3>0,<4>4;
param ys[nums] := <1>9,<2>6,<3>12,<4>9;

var OA real >= 0;
var OB real >= 0;
var OC real >= 0;
var OD real >= 0;

var x real >= -infinity;
var y real >= -infinity;

minimize obj: 20*OA + 14*OB + 8*OC + 24*OD;
subto c1: OA == sqrt((x-xs[1])^2 + (y-ys[1])^2);
subto c2: OB == sqrt((x-xs[2])^2 + (y-ys[2])^2);
subto c3: OC == sqrt((x-xs[3])^2 + (y-ys[3])^2);
subto c4: OD == sqrt((x-xs[4])^2 + (y-ys[4])^2);

