var T real;
var N real;
var P real;
var A real;

maximize audience: 5000*T+8500*N+2400*P+2800*A;
subto budget: 800*T+925*N + 290*P+380*A<=8000;
subto radioHour: P+A>=5;
subto radioCost: 290*P+380*A<=1800;
subto t: T<=12;
subto n: N<=5;
subto p: P<=25;
subto a: A<=20;
