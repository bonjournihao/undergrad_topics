#include <iostream>
#include "InternalNode.h"
using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize];
} // InternalNode::InternalNode()


int InternalNode::getMinimum()const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum() 


//1. check if leafNode insertion returns NULL, yes, returns NULL; no, split & return
//2. check if parent is full(check left right), no, insertion, yes, split and insert
//       split rule: 1.the new InternalNode holds >= #of leaves;
//                   2.the new InternalNode's level has to have count <= internalSize, otherwise, split its parent
//                   update keys after each split
InternalNode* InternalNode::insert(int value)
{
  //printSiblingStatus();
  BTreeNode *returnNode;
  //find spot to insert
  int keyIndex;
  if(count==1) {
    returnNode = children[0] -> insert(value);
    keyIndex = 0;
  }
  else if (keys[count - 1] <= value){
      returnNode = children[count - 1]->insert(value);
      keyIndex = count - 1;
  }
  else {
    for (int i = 0; i < count - 1; i++)
    {
      if (keys[i + 1] > value){
        returnNode = children[i]->insert(value);
        keyIndex = i;
        break;
      }
    }
  }
  //cout <<"keyIndex: " << keyIndex << endl;
  updateKeys();
 if(returnNode == NULL) {
     //split not happen, just update key
    //cout << "split not happen, just update key" << endl;
    /*
     keys[keyIndex] = children[keyIndex]->getMinimum();
     if(keyIndex > 1) {
       keys[keyIndex-1] = children[keyIndex-1] -> getMinimum();
     }
     if(keyIndex < count - 1 && children[keyIndex+1])
     {
       keys[keyIndex+1] = children[keyIndex+1] -> getMinimum();
     }*/
   return NULL;
 }
  //(check accuracy)
  //if current not full
  //cout << "count: " << count <<", internalSize: " << internalSize << endl;
  if (count != internalSize)
  {
    //insert in the new key and children ptr into arrays
    //set siblings
    /*
      int i = 0;
      for (; i < count; i++)
      {
        if (keys[i] > returnNode->getMinimum())
        {
          for (int j = count; j > i; j--)
            keys[j] = keys[j - 1];

          keys[i] = returnNode->getMinimum();
          children[i] = returnNode;
        }
      }
      if (i == count) 
      {
        keys[i] = returnNode->getMinimum();
        children[i] = returnNode;
      }
      count++;

      //set siblings
      for (int i = 0; i < count - 1; i++)
      {
        children[i]->setRightSibling(children[i + 1]);
        children[i + 1]->setLeftSibling(children[i]);
      }
    */
/*
     int i = 1;
  for (; i < count; i++)
  {
    if (keys[i] > returnNode->getMinimum())
    {
      children[i - 1]->setRightSibling(returnNode);
      returnNode->setLeftSibling(children[i - 1]);
      children[i]->setLeftSibling(returnNode);
      returnNode->setRightSibling(children[i]);
    }
  }
  if (i == count)
  {
    if(children[count-1]->getRightSibling())
     children[count-1]->getRightSibling()->setLeftSibling(returnNode);
    returnNode->setRightSibling(children[i-1]->getRightSibling());
    children[count-1]->setRightSibling(returnNode);
    returnNode->setLeftSibling(children[i-1]);
  }
*/
    insert(returnNode);
    updateKeys();
    return NULL;
  }

  else if (dynamic_cast<InternalNode *>(leftSibling) && dynamic_cast<InternalNode *>(leftSibling)->getCount() != internalSize) //use getCount if necessary
  {
    //cout << "leftSibling is not null and has space" << endl;
    InternalNode * left = dynamic_cast<InternalNode *>(leftSibling);
    if (returnNode->getMinimum() <= keys[0])
      left->insert(returnNode);
    else{ 
      left->insert(children[0]);
      for(int i = 0; i < count - 1; i++) {
        //cout << "i:" << i <<" :" << children[i]->getMinimum() << ", " << children[i+1]->getMinimum() << endl; 
        children[i] = children[i+1];
        keys[i] = keys[i+1];
      }
      children[count - 1] = NULL;
      count--;
      this->insert(returnNode);
    }
    updateKeys();
    return NULL;
  }
  else if (dynamic_cast<InternalNode *>(rightSibling) && dynamic_cast<InternalNode *>(rightSibling)->getCount() != internalSize)
  {
    //cout << "rightSibling is not null and has space" << endl;
    InternalNode * right = dynamic_cast<InternalNode *>(rightSibling);
    if (returnNode->getMinimum() >= keys[internalSize - 1])
      right->insert(returnNode);
    else{
      right->insert(children[internalSize - 1]);
      children[internalSize - 1] = NULL;
      count--;
      this->insert(returnNode);   
    }
    updateKeys();
    return NULL;
  }

  //need split
  InternalNode *newNode = insert_split(returnNode);
  updateKeys();
  return newNode;
} // InternalNode::insert()

int InternalNode::getCount()
{
  return count;
}

void InternalNode::updateKeys()
{
  for(int i = 0; i < count; i++)
    keys[i] = children[i] -> getMinimum();
}


void InternalNode::setCount(int c)
{
  count = c;
  return;
}

//split InternalNode
InternalNode* InternalNode::insert_split(BTreeNode *returnNode)
{
  //cout <<"InternalNode insert split" << endl;
  InternalNode *newInternalNode = new InternalNode(internalSize, leafSize, NULL, NULL, NULL);
  /*  
  int i = 1;
  for (; i < count; i++)
  {
    if (keys[i] > returnNode->getMinimum())
    {
      children[i - 1]->setRightSibling(returnNode);
      returnNode->setLeftSibling(children[i - 1]);
      children[i]->setLeftSibling(returnNode);
      returnNode->setRightSibling(children[i]);
    }
  }
  if (i == count) 
  {
    if(children[count-1]->getRightSibling())
     children[count-1]->getRightSibling()->setLeftSibling(returnNode);
    returnNode->setRightSibling(children[i-1]->getRightSibling());
    children[count-1]->setRightSibling(returnNode);
    returnNode->setLeftSibling(children[i-1]);
  }
*/

  newInternalNode->setLeftSibling(this);
  newInternalNode->setRightSibling(rightSibling);
  this->setRightSibling(newInternalNode);
  newInternalNode->setParent(parent);
  int skipNum = (internalSize +1)/2;
  ////cout << "skipNum: " << skipNum << endl;
  if (returnNode -> getMinimum() > keys[skipNum-1])
  {
   // cout << "returnNode kept in new node" << endl;
    for(int i = skipNum; i < internalSize; i++)
    {
     // cout << "move child :" << i << ", key:" << keys[i] << ", to new node";
      newInternalNode->insert(children[i]);
      count--;
      children[i] = NULL; 
    }
    newInternalNode->insert(returnNode);
  }
  else {
   // cout << "returnNode kept in old node" << endl;
    for(int i = skipNum - 1 > 0 ? skipNum-1 : 0; i < internalSize; i++)
    {
     // cout << "move child :" << i << ", key:" << keys[i] << ", to new node";
      newInternalNode->insert(children[i]);
      count--;
      children[i] = NULL;
    } 
     this->insert(returnNode);
  }
  //children[count-1]->setRightSibling(newInternalNode->children[0]);
  //newInternalNode->setLeftSibling(children[count-1]);
  
  //first insert


/*
  int i = 1;
  for (; i < count; i++)
  {
    if (keys[i] > returnNode->getMinimum())
    {
      children[i - 1]->setRightSibling(returnNode);
  //    returnNode->setLeftSibling(children[i - 1]);
      children[i]->setLeftSibling(returnNode);
      returnNode->setRightSibling(children[i]);
    }
  }
  if (i == count) 
  {
    children[i]->setRightSibling(returnNode);
    returnNode->setLeftSibling(children[i]);
  }


  int skipNum = internalSize / 2;
  BTreeNode *siblingPtr;

  //copy from half way of old to new, using linked nodes
  if (children[0]->getLeftSibling() == NULL)
    siblingPtr = children[0];
  else
    siblingPtr = children[0]->getLeftSibling();
  //skip #(skipNum) of nodes
  for (int i = 0; i < skipNum; i++)
  {
    siblingPtr = siblingPtr->getRightSibling();
  }

  //move children from old to new
  for (int i = 0, j = 0; i < internalSize + 1 - skipNum; i++, j++)
  {
    newInternalNode->children[j] = siblingPtr;
    //increment siblingPtr
    siblingPtr = siblingPtr->getRightSibling();
  }

  //clear old keys & children ptr
  if (i >= (internalSize + 1) / 2)
  {
    for (int i = (internalSize + 1) / 2; i < internalSize; i++)
    {
      keys[i] = 0;
      children[i] = NULL;
    }

  }
  else
  {
    for (int i = (internalSize - 1) / 2; i < internalSize; i++)
    {
      keys[i] = 0;
      children[i] = NULL;
    }
  }

  setCount((internalSize + 1) / 2);
  newInternalNode->setCount(((internalSize + 1) / 2) + (internalSize + 1) % 2);
  */
 // newInternalNode->setLeftSibling(this);
 // newInternalNode->setRightSibling(rightSibling);
 // this->setRightSibling(newInternalNode);
  //newInternalNode->setParent(parent);

  return newInternalNode;
}


void InternalNode::root_insert(BTreeNode *oldRoot, BTreeNode *node2)
{ // Node must be the root, and node1
  //cout << "root insert" << endl;
  int oldMini = oldRoot->getMinimum();
  int node2Mini = node2->getMinimum();
  if(oldMini < node2Mini) {
    children[0] = oldRoot;
    children[1] = node2;
    keys[0] = oldMini;
    keys[1] = node2Mini;
  } else {
    children[0] = node2;
    children[1] = oldRoot;
    keys[0] = node2Mini;
    keys[1] = oldMini;
  }
  count = 2;
  //cout << "key0:" << keys[0] << ",key1:" << keys[1] << endl;
  children[0]->setParent(this);
  children[1]->setParent(this);
  children[0]->setRightSibling(children[1]);
  children[0]->setLeftSibling(NULL);
  children[1]->setLeftSibling(children[0]);
  children[1]->setRightSibling(NULL);
  return;
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *newNode) // from a sibling
{
  newNode->setParent(this); 
  keys[count] = newNode->getMinimum();
  children[count] = newNode;
  count++; 
  for(int i = count - 1; i >0; i--) {
    if(keys[i] < keys[i -1]) {
 //     cout << "key :" << i << ", "<< keys[i] << "key: "<< i -1 << ", "<<keys[i-1] << endl;
      int tmp = keys[i-1];
      keys[i -1] = keys[i];
      keys[i] = tmp;
      BTreeNode *tmpNode = children[i-1];
      children[i-1] = children[i];
      children[i] = tmpNode;
    }
  }

  for(int i = 0; i < count; i++) {
    if(i+1 < count)
      children[i]->setRightSibling(children[i+1]);
    if(i > 0)
      children[i]->setLeftSibling(children[i-1]);
  }
  if(getLeftSibling()) {
    BTreeNode *node  =dynamic_cast<InternalNode *>(getLeftSibling())->children[getLeftSibling()->getCount()-1];
    children[0]->setLeftSibling(node);
    if(node)
      node->setRightSibling(children[0]);
  }
  if(getRightSibling()) {
    BTreeNode *node= dynamic_cast<InternalNode *>(getRightSibling())->children[0];
    children[count-1]->setRightSibling(node);
    if(node)
      node->setLeftSibling(children[count-1]);
  }
 /* 
  //from right sibling
  if (newNode->getMinimum() > keys[0])
  {
    //insert after the last element
    keys[count] = newNode->getMinimum();
    children[count] = newNode;
  }

  //from left sibling
  if (newNode->getMinimum() < keys[0])
  {
    int i = count - 1;
    for (; i > 0; i--)
    {
      keys[i + 1] = keys[i];
      children[i + 1] = children[i];
    }
    //i should be 0
    keys[i] = newNode->getMinimum();
    children[i] = newNode;
  }
  count++;
*/
} // InternalNode::insert()


//int InternalNode::getCount()const
//{
//  return count;
//}


void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;

  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  cout << endl;

  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()

void InternalNode::printSiblingStatus() {
  for(int i =0; i < count;i ++) {
  cout << "key:" << keys[i];
  if(children[i]->getLeftSibling())
    cout << ",left Sibling" << children[i]->getLeftSibling()->getMinimum();
  else
    cout << ", No left sibiling"; 
  cout << endl;
  if(children[i]->getRightSibling())
    cout << ",right Sibling" << children[i]->getRightSibling()->getMinimum();
  else
    cout << ", No right sibiling";
  cout << endl;
}
}
