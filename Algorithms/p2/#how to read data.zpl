#how to read data

set V	:= {read "data.dat" as "<ls>" comment "#"}

param a[V] := read "data.dat" as "<ls> 2n" comment "#";
param b[V] := read "data.dat" as "<ls> 3n" comment "#";


var x real;
var y real;
var z real;

minimize l: z

subto equ: for all....
.....

subto dummy
....some function