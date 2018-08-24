M = readtable('ecs171.dataset.txt');
C = table2cell(M);
[nrows,ncols] = size(C);
fid = fopen('gene.txt','w');
for row = 1:nrows
    fprintf(fid,['%s %s %s %s %s' repmat(' %f',[1 4496]) '\n'],C{row,1:4501});
end
fclose(fid);