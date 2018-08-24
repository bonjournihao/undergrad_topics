#ifndef InternalNodeH
  #define InternalNodeH

#include "BTreeNode.h"

class InternalNode:public BTreeNode
{
  int internalSize;
  BTreeNode **children;
  int *keys;
  void printSiblingStatus();
public:
  InternalNode(int ISize, int LSize, InternalNode *p,
    BTreeNode *left, BTreeNode *right);
  int getMinimum()const; //for updating key
  InternalNode* insert(int value); // returns pointer to new InternalNode
    // if it splits else NULL
  InternalNode* insert_split(BTreeNode *returnNode);
  int getCount();
  void setCount(int c);
  void root_insert(BTreeNode *oldRoot, BTreeNode *node2); // if root splits use this
  void insert(BTreeNode *newNode); // from a sibling
  void print(Queue <BTreeNode*> &queue);
  void updateKeys();
}; // InternalNode class

#endif
