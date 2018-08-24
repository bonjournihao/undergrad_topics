#include <iostream>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()



int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()

int LeafNode::getCount()const
{
  return count;
}


void LeafNode::increment_count()
{
  count++;
  return;
}


LeafNode* LeafNode::insert(int value)
{
  //cout << "LeafNode insert:" << value << ",count:" << count<< ",leafSize:" <<leafSize <<endl;
  if (count != leafSize)//regular insertion
  {
    insertInSortedList(value);
    if(parent){
      parent->updateKeys();
      //cout << "parent is" << parent->getMinimum()<< endl;
    }
    return NULL; 
  }
  else //if leaf is full
  {
    //if left sibling exists and is not full
    if (dynamic_cast<LeafNode *>(leftSibling) && (dynamic_cast<LeafNode *>(leftSibling)->getCount() != leafSize))
    {
   //   cout << "LeafNode has leftSibling and it has space" << endl;
      // if value not minimum in the current leaf, move current[0] to left[last]
      //insert value in current leaf
      if (values[0] <= value)  
      {
        dynamic_cast<LeafNode *>(leftSibling)->insert(values[0]);
        moveUpOne();
        count--; 
        insertInSortedList(value);
      }
      // if value is the minimum in current leaf, insert value at left[last]
      else 
      {
        dynamic_cast<LeafNode *>(leftSibling)->insert(value);
      }
      return NULL;
    }

    //if right sibling exists and is not full
    else if (dynamic_cast<LeafNode *>(rightSibling) && (dynamic_cast<LeafNode *>(rightSibling)->count != leafSize))
    {
     // cout << cout << "LeafNode has rightSibling and it has space" << endl;
      //if current[last] is greater than value, move current[last] to right[0]
      //insert value to current
      if (values[leafSize - 1] >= value)
      {
        dynamic_cast<LeafNode *>(rightSibling)->insert(values[leafSize - 1]);
        count--;
        insertInSortedList(value);
      }
      //if value is greatest in current, insert value in right sibling
      else
      {
        dynamic_cast<LeafNode *>(rightSibling)->insert(value);
      }
      return NULL;
    }

    else// left&right siblings are full or NULL, split leaves, set siblings
    {
      //cout << "left&right siblings are full or NULL" << endl;
      //NOT SURE if the initialization is CORRECT
      LeafNode *newLeafNode = new LeafNode(leafSize, parent, NULL, NULL);
      int skipNum = (leafSize +1)/2;
      if(value > values[skipNum -1]) {
        for(int i = skipNum; i < leafSize; i++) {
          newLeafNode->insert(values[i]);
          count--;
        }
        newLeafNode->insert(value);
      }
      else
      {
        for(int i = skipNum-1; i < leafSize;i++) {
          newLeafNode->insert(values[i]);
          count--;
        }
        insert(value);
     }
/*
      int maxNum = 0;

      maxNum = getMax_insert(value);
      LeafNode *newLeafNode = new LeafNode(leafSize, parent, NULL, NULL);
      cout << "maxNum is:" << maxNum << endl;
      //insert value into new leafnode
      //rule: split into two, with (# of elements) new >= current
      
     


      int j = 0;
      for (int i = leafSize - (leafSize/2 + leafSize % 2); i < leafSize; i++)
      {
        cout << "move value postiion :" << i << "value:" << values[i] << "to new leaf"<< endl;
        //newLeafNode->values[j] = values[i];
        newLeafNode->insert(values[i]);
        //j++;
        count--;
      }
      newLeafNode->insert(maxNum);
      //newLeafNode->values[j] = maxNum;
 */     
      //newLeafNode->setParent(parent);//newNode's parent is this->parent
      return newLeafNode;
    }
  }
}  // LeafNode::insert()

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()

void LeafNode::insertInSortedList(int &value)
{
  //cout << "insertInSortedList" << endl;
  values[count] = value;
  count++;
  int temp;

  for (int i = count - 1; i > 0; i--)
  {
    if (values[i - 1] > values[i])
    {
      temp = values[i - 1];
      values[i - 1] = values[i];
      values[i] = temp;
    } // if value is less, move up
  }//insert value into the sorted leaf list
  //cout << "leave insertInSortedList" << endl;
  if(parent)
    parent->updateKeys();
  return;
}

void LeafNode::moveUpOne()
{
  for (int i = 0; i < count - 1; i++)
    values[i] = values[i + 1];

  return;
}

//if value is largest, return value;
//else, return last item, insert value
int LeafNode::getMax_insert(int &value)
{
  int maxNum = 0;
  //if current[last] is max, replace with value, return current num
  if (values[leafSize - 1] >= value)
  { 
    maxNum = values[leafSize - 1];
    count--;
    insertInSortedList(value);
    return maxNum;
  }
  //if value is max in current, return value
  else
    return value;
}


