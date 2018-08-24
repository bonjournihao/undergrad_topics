param Matrix[{1..9}*{1..9}] := read "sudoku4.dat" as "n+" comment "#";
param c[{1..9}*{1..9}*{1..9}] := read "values.dat" as "n+" comment "#";

set xs := {1..9};
set ys := {1..9};
set zs := {1..9};
var x[xs*ys*zs] binary;

minimize obj: sum <i> in xs: sum <j> in ys: sum <k> in zs: c[i,j,k]*x[i,j,k];
subto c1: forall <i> in xs: forall <j> in ys: sum <k> in zs: x[i,j,k] == 1;
subto c2: forall <i> in xs: forall <k> in zs: sum <j> in ys: x[i,j,k] == 1;
subto c3: forall <j> in ys: forall <k> in zs: sum <i> in xs: x[i,j,k] == 1;
subto c4: forall <i> in xs: forall <j> in ys:
                if Matrix[i,j] != 0
                        then x[i,j,Matrix[i,j]] == 1 end;

subto d1: forall <k> in zs:
			sum <i> in xs with i >= 1 and i <= 3:
                sum <j> in ys with j >= 1 and j <= 3:
                        x[i,j,k] == 1;
subto d2: forall <k> in zs:
			sum <i> in xs with i >= 1 and i <= 3:
                sum <j> in ys with j >= 4 and j <= 6:
                        x[i,j,k] == 1;
subto d3: forall <k> in zs:
			sum <i> in xs with i >= 1 and i <= 3:
                sum <j> in ys with j >= 7 and j <= 9:
                        x[i,j,k] == 1;
subto d4: forall <k> in zs:
			sum <i> in xs with i >= 4 and i <= 6:
                sum <j> in ys with j >= 1 and j <= 3:
                        x[i,j,k] == 1;
subto d5: forall <k> in zs:
			sum <i> in xs with i >= 4 and i <= 6:
                sum <j> in ys with j >= 4 and j <= 6:
                        x[i,j,k] == 1;
subto d6: forall <k> in zs:
			sum <i> in xs with i >= 4 and i <= 6:
                sum <j> in ys with j >= 7 and j <= 9:
                        x[i,j,k] == 1;
subto d7: forall <k> in zs:
			sum <i> in xs with i >= 7 and i <= 9:
                sum <j> in ys with j >= 1 and j <= 3:
                        x[i,j,k] == 1;
subto d8: forall <k> in zs:
			sum <i> in xs with i >= 7 and i <= 9:
                sum <j> in ys with j >= 4 and j <= 6:
                        x[i,j,k] == 1;
subto d9: forall <k> in zs:
			sum <i> in xs with i >= 7 and i <= 9:
                sum <j> in ys with j >= 7 and j <= 9:
                        x[i,j,k] == 1;

