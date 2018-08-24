image(X);colormap(map)

[U,S,V] = svd(X);

stem(diag(S));grid;

subplot(2,2,1);
Xk=U(:,1:1)*S(1:1,1:1)*V(:,1:1)';
image(Xk);

subplot(2,2,2);
Xk=U(:,1:6)*S(1:6,1:6)*V(:,1:6)';
image(Xk);

subplot(2,2,3);
Xk=U(:,1:11)*S(1:11,1:11)*V(:,1:11)';
image(Xk);

subplot(2,2,4);
Xk=U(:,1:31)*S(1:31,1:31)*V(:,1:31)';
image(Xk);

X = U*S*V;
subplot(2,2,1);
Xk=U(:,1:1)*S(1:1,1:1)*V(:,1:1)';
image(X-Xk);

subplot(2,2,2);
Xk=U(:,1:6)*S(1:6,1:6)*V(:,1:6)';
image(X-Xk);

subplot(2,2,3);
Xk=U(:,1:11)*S(1:11,1:11)*V(:,1:11)';
image(X-Xk);

subplot(2,2,4);
Xk=U(:,1:31)*S(1:31,1:31)*V(:,1:31)';
image(X-Xk);


Xk=U(:,1:1)*S(1:1,1:1)*V(:,1:1)';
error = abs((S(2,2)-norm(X-Xk)))/S(2,2);
display(error); 

Xk=U(:,1:6)*S(1:6,1:6)*V(:,1:6)';
error = abs((S(7,7)-norm(X-Xk)))/S(7,7);
display(error); 

Xk=U(:,1:11)*S(1:11,1:11)*V(:,1:11)';
error = abs((S(12,12)-norm(X-Xk)))/S(12,12);
display(error); 

Xk=U(:,1:31)*S(1:31,1:31)*V(:,1:31)';
error = abs((S(32,32)-norm(X-Xk)))/S(32,32);
display(error); 