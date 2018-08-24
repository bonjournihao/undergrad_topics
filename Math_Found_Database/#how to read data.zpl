#how to read data

set V	:= {read "data.dat" as "<1s>" comment "#"}

param a[V] := read "data.dat" as "<1s> 2n" comment "#";
param b[V] := read "data.dat" as "<1s> 3n" comment "#";


var x real;
var y real;
var z real;

minimize l: z

subto equ: forall ....
.....

subto dummy
....some function