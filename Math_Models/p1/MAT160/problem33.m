function problem22(A)
[U,S,V] = svd(A);

S = diag(S);

angle = 0:2*pi/360:2*pi;
domain = [cos(angle); sin(angle)];
range = A*domain;

subplot(2,1,1);
plot(domain(1,:),domain(2,:),'k.'),axis('image'),grid,hold;
title('right singular vectors');
quiver(0,0,V(1,1),V(2,1),0);
quiver(0,0,V(1,2),V(2,2),0);

subplot(2,1,2);
plot(range(1,:),range(2,:),'b.'),axis('image'),grid,hold;
title('left singular vectors');
quiver(0,0,S(1)*U(1,1),S(1)*U(2,1),0);
quiver(0,0,S(2)*U(1,2),S(2)*U(2,2),0);
