# Problem 1
#When lamda is 1000, there are 12 non-zero decision variables,
#After increasing lamda to 10000, only X56 and X57 are left.
#Therefore, the most significant characteristics that define the spam emails is at the 56 and 57 columns
#col 56 : length of longest uninterrupted sequence of capital letters
#col 57 : total number of capital letters in the e-mail
#For lamda values smaller than 1000, the problem cannot be quickly sloved by SCIP(very slow)

set cols:={1..4601};
set rows:={1..57};

param X1[cols]:= read "spambase.data" as "1n";
param X2[cols]:= read "spambase.data" as "2n";
param X3[cols]:= read "spambase.data" as "3n";
param X4[cols]:= read "spambase.data" as "4n";
param X5[cols]:= read "spambase.data" as "5n";
param X6[cols]:= read "spambase.data" as "6n";
param X7[cols]:= read "spambase.data" as "7n";
param X8[cols]:= read "spambase.data" as "8n";
param X9[cols]:= read "spambase.data" as "9n";
param X10[cols]:= read "spambase.data" as "10n";
param X11[cols]:= read "spambase.data" as "11n";
param X12[cols]:= read "spambase.data" as "12n";
param X13[cols]:= read "spambase.data" as "13n";
param X14[cols]:= read "spambase.data" as "14n";
param X15[cols]:= read "spambase.data" as "15n";
param X16[cols]:= read "spambase.data" as "16n";
param X17[cols]:= read "spambase.data" as "17n";
param X18[cols]:= read "spambase.data" as "18n";
param X19[cols]:= read "spambase.data" as "19n";
param X20[cols]:= read "spambase.data" as "20n";
param X21[cols]:= read "spambase.data" as "21n";
param X22[cols]:= read "spambase.data" as "22n";
param X23[cols]:= read "spambase.data" as "23n";
param X24[cols]:= read "spambase.data" as "24n";
param X25[cols]:= read "spambase.data" as "25n";
param X26[cols]:= read "spambase.data" as "26n";
param X27[cols]:= read "spambase.data" as "27n";
param X28[cols]:= read "spambase.data" as "28n";
param X29[cols]:= read "spambase.data" as "29n";
param X30[cols]:= read "spambase.data" as "30n";
param X31[cols]:= read "spambase.data" as "31n";
param X32[cols]:= read "spambase.data" as "32n";
param X33[cols]:= read "spambase.data" as "33n";
param X34[cols]:= read "spambase.data" as "34n";
param X35[cols]:= read "spambase.data" as "35n";
param X36[cols]:= read "spambase.data" as "36n";
param X37[cols]:= read "spambase.data" as "37n";
param X38[cols]:= read "spambase.data" as "38n";
param X39[cols]:= read "spambase.data" as "39n";
param X40[cols]:= read "spambase.data" as "40n";
param X41[cols]:= read "spambase.data" as "41n";
param X42[cols]:= read "spambase.data" as "42n";
param X43[cols]:= read "spambase.data" as "43n";
param X44[cols]:= read "spambase.data" as "44n";
param X45[cols]:= read "spambase.data" as "45n";
param X46[cols]:= read "spambase.data" as "46n";
param X47[cols]:= read "spambase.data" as "47n";
param X48[cols]:= read "spambase.data" as "48n";
param X49[cols]:= read "spambase.data" as "49n";
param X50[cols]:= read "spambase.data" as "50n";
param X51[cols]:= read "spambase.data" as "51n";
param X52[cols]:= read "spambase.data" as "52n";
param X53[cols]:= read "spambase.data" as "53n";
param X54[cols]:= read "spambase.data" as "54n";
param X55[cols]:= read "spambase.data" as "55n";
param X56[cols]:= read "spambase.data" as "56n";
param X57[cols]:= read "spambase.data" as "57n";
param Y[cols]:= read "spambase.data" as "58n";

var z real;
var w real;
var x[rows] real>=-infinity;
var t[cols] real>=-infinity;
param lamda:=10000;

minimize objective: z+w*lamda;

subto c1: z == (sum <i> in cols: t[i]*t[i]);
subto c2: w == (sum <i> in rows: abs(x[i]));
subto t:
	forall <i> in cols do 
	t[i]==X1[i]*x[1]+X2[i]*x[2]+X3[i]*x[3]+X4[i]*x[4]+X5[i]*x[5]+X6[i]*x[6]+X7[i]*x[7]+X8[i]*x[8]+X9[i]*x[9]+X10[i]*x[10]+X11[i]*x[11]+X12[i]*x[12]+X13[i]*x[13]+X14[i]*x[14]+X15[i]*x[15]+X16[i]*x[16]+X17[i]*x[17]+X18[i]*x[18]+X19[i]*x[19]+X20[i]*x[20]+X21[i]*x[21]+X22[i]*x[22]+X23[i]*x[23]+X24[i]*x[24]+X25[i]*x[25]+X26[i]*x[26]+X27[i]*x[27]+X28[i]*x[28]+X29[i]*x[29]+X30[i]*x[30]+X31[i]*x[31]+X32[i]*x[32]+X33[i]*x[33]+X34[i]*x[34]+X35[i]*x[35]+X36[i]*x[36]+X37[i]*x[37]+X38[i]*x[38]+X39[i]*x[39]+X40[i]*x[40]+X41[i]*x[41]+X42[i]*x[42]+X43[i]*x[43]+X44[i]*x[44]+X45[i]*x[45]+X46[i]*x[46]+X47[i]*x[47]+X48[i]*x[48]+X49[i]*x[49]+X50[i]*x[50]+X51[i]*x[51]+X52[i]*x[52]+X53[i]*x[53]+X54[i]*x[54]+X55[i]*x[55]+X56[i]*x[56]+X57[i]*x[57]-Y[i];

	


