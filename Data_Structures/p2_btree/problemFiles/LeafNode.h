#ifndef LeafNodeH
#define LeafNodeH

#include "BTreeNode.h"

class LeafNode:public BTreeNode
{
  int *values;
public:
  LeafNode(int LSize, InternalNode *p, BTreeNode *left,
    BTreeNode *right);
  int getMinimum() const;
  LeafNode* insert(int value); // returns pointer to new Leaf if splits
  // else NULL
  void increment_count();
  void insertInSortedList(int &value);
  void moveUpOne();
  int getMax_insert(int &value);
  int getCount()const;
  void print(Queue <BTreeNode*> &queue);
}; //LeafNode class

#endif
