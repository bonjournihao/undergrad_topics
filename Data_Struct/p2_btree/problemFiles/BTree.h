#ifndef BTREE_H
  #define BTREE_H

#include "BTreeNode.h"

class BTree
{
  BTreeNode *root;
  int internalSize;
  int leafSize;
public:
  BTree(int ISize, int LSize);
  void insert(int value);
  void create_top_internalNode(BTreeNode *&BTreeNodePtr);
  void update_curr_par_Keys(BTreeNode *ptr);
  void print();
  
}; // BTree class
#endif

