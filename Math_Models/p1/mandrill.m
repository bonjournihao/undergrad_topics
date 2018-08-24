image(X);color(map)
[U,S,V] = svd(X);
stem(diag(S));
grid
subplot(2,2,1)
Xk=U(:,1:1)*S(1:1,1:1)*V(:,1:1)';
image(Xk)

subplot(2,2,2)
Xk=U(:,1:6)*S(1:6,1:6)*V(:,1:6)';
image(Xk)

subplot(2,2,3)
Xk=U(:,1:11)*S(1:11,1:11)*V(:,1:11)';
image(Xk)

subplot(2,2,4)
Xk=U(:,1:31)*S(1:31,1:31)*V(:,1:31)';
image(Xk)

X=U*S*V
subplot(2,2,1)
Xk=U(:,1:1)*S(1:1,1:1)*V(:,1:1)';
image(X-Xk)

subplot(2,2,2)
Xk=U(:,1:6)*S(1:6,1:6)*V(:,1:6)';
image(X-Xk)

subplot(2,2,3)
Xk=U(:,1:11)*S(1:11,1:11)*V(:,1:11)';
image(X-Xk)

subplot(2,2,4)
Xk=U(:,1:31)*S(1:31,1:31)*V(:,1:31)';
image(X-Xk)
