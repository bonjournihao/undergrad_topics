#include <iostream>
#include <fstream>
#include <cstdlib>
#include "BTree.h"
#include "BTreeNode.h"
#include "LeafNode.h"
#include "InternalNode.h"
using namespace std;

BTree::BTree(int ISize, int LSize):internalSize(ISize), leafSize(LSize)
{
  root = new LeafNode(LSize, NULL, NULL, NULL);
} // BTree::BTree()


void BTree::insert(const int value)
{
  //cout << "BTree insert:" << value << endl; 
  //BTreeNode *leafNodePtr;// = new LeafNode(leafSize, NULL, NULL, NULL);
  BTreeNode *splitRoot;// = new InternalNode(internalSize, leafSize, NULL, NULL, NULL);

  //insertion into an InternalNode
  if (InternalNode* v = dynamic_cast<InternalNode*>(root)){
    //cout << "Root is InternalNode" << endl;
    splitRoot = root->insert(value);
  }
  //first insertion into a leaf
  else {
   // cout << "Root is LeafNode" << endl;
    splitRoot = root->insert(value); 
  }
  //if returns a new leafnode to base class, must need a top InternalNode
  if (splitRoot)
  {
    //cout << "insert did split" << endl;
    InternalNode *newRoot = new InternalNode(internalSize, leafSize, NULL, NULL, NULL);
    newRoot->root_insert(root, splitRoot);
    root = newRoot;
  }   
} // BTree::insert()

/*
//update keys in an internal node and above
void BTree::update_curr_par_Keys(BTreeNode *ptr)
{
  for (; ptr; ptr->parent)
  {
    InternalNode *updatePtr;

    //check if the new node is at the leftmost side
    if (ptr->children[0]->left == NULL)
      updatePtr = ptr->children[0];
    else 
      updatePtr = ptr->children[0]->left;

    for (int i = 0; i < ptr->count; i++)
    {
      ptr->key[i] = updatePtr.getMinimum();
      ptr->children[i] = updatePtr;
      updatePtr = updatePtr->right;
    }
  }
  return;
}
*/


void BTree::print()
{
  BTreeNode *BTreeNodePtr;
  Queue<BTreeNode*> queue(1000);

  queue.enqueue(root);
  while(!queue.isEmpty())
  {
    BTreeNodePtr = queue.dequeue();
    BTreeNodePtr->print(queue);
  } // while
} // BTree::print()

