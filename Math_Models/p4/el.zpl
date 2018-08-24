#Xueying Shen
#Sudoku Problem

set I:={1..9};
var X[I*I*I] binary;
set T1:={1..3};
set T2:={4..6};
set T3:={7..9};
param X[I*I*I]:=<2,1,6> 1,<2,2,7> 1,<2,3,2> 1,<1,4,1> 1,<1,5,5> 1, <1,6,6> 1,<1,9,3> 1,<2,5,8> 1,<2,7,1> 1,<3,5,2> 1,<3,6,9> 1,<3,7,4> 1,<3,8,8> 1,<4,1,9> 1, <4,2,3> 1, <4,3,4> 1, <4,9,1> 1,<5,4,9> 1, <5,5,1> 1,<5,8,7> 1, <5,9,4> 1,<6,1,8> 1,<6,4,6> 1, <6,6,5> 1, <7,3,1> 1,<7,5,9> 1,<7,6,8> 1,<7,8,4> 1,<8,1,5> 1,<8,2,4> 1,<8,3,9> 1,<8,4,2> 1,<8,7,3> 1,<9,3,6> 1, <9,4,4> 1,<9,6,1> 1,<9,7,2> 1,<9,9,9> 1;


minimize any:
        X[1,1,1];

subto col:
        forall <i,k>in I*I do
        sum <j> in I:X[i,j,k]==1;

subto row:
        forall <j,k> in I*I do
        sum <i> in I:X[i,j,k]==1;

subto small_matrix1:
        forall <k> in I do
        sum <t> in T1:sum <j> in T1:X[t,j,k]==1;
subto small_matrix2:
        forall <k> in I do
        sum <t> in T2:sum <j> in T1:X[t,j,k]==1;
subto small_matrix3:
        forall <k> in I do
        sum <t> in T3:sum <j> in T1:X[t,j,k]==1;
subto small_matrix4:
        forall <k> in I do
        sum <t> in T1:sum <j> in T2:X[t,j,k]==1;
subto small_matrix5:
        forall <k> in I do
        sum <t> in T2:sum <j> in T2:X[t,j,k]==1;
subto small_matrix6:
        forall <k> in I do
        sum <t> in T3:sum <j> in T2:X[t,j,k]==1;
subto small_matrix7:
        forall <k> in I do
        sum <t> in T1:sum <j> in T3:X[t,j,k]==1;
subto small_matrix8:
        forall <k> in I do
        sum <t> in T2:sum <j> in T3:X[t,j,k]==1;
subto small_matrix9:
        forall <k> in I do
        sum <t> in T3:sum <j> in T3:X[t,j,k]==1;