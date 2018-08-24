Btree deletion! 

Binary Tree:
	AVL
	Splay


	Array-> [1][2][3][4][5]   left: 2i+1, right: 2i+2
	BinaryTree:
	    1
	   / \
	  2   3
	 / \  
	4   5


Binary Search Tree: worstCase: O(N)

AVL Tree & Splay Tree -->self-balanced Binary Search Tree

after deletiion, promote the minimum of the right subtree

AVL Tree -- left right subtrees any node have to have height difference less than 2

	   50(children = -1)
	  /
	 40 (height = 1)
	/
  30 

    "zig-zig"
  		3
  	   / \
  	  2   D
  	 / \
  	1   C
   / \
  A   B

         2
        / \
       /   \
      1     3
     / \   / \
    A   B C   D

    "zig-zag"

        1 (problem parent)  1-2-4
       / \
      2   5
     / \
    3   4 
       / \
      6   A
     / \
    B   C

         1
        / \
       4   5
      / \
     2   A
    / \
   3   6
      / \ 
     B   C

         1
        / \
       4   1
      /   / \
     2   A   5
    / \
   3   6
      / \
     B   C


        




