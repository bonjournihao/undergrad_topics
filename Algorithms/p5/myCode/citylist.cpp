#include <ostream>
#include "CityList.h"
using namespace std;

CityNode::~CityNode()
{
  delete next;
}// ~CityNode


CityList::~CityList()
{
  while (ptr)
  {
  	ListNode *ptr = head;
  	head = head->next;
  	delete ptr;
  } //while
} // ~CityList

int& CityList::getCount()
{
  return count;
}

CityList& CityList::operator += (const City &rhs)
{
  if (head = NULL)
  {
  	head = new CityNode(rhs, head);
  	tail = head;
  } // if empty list, create node

  else
  {
  	tail->next = new CityNode(rhs, NULL);
    tail = tail->next;
  } // add at the end

  count += 1;
  return *this;
}

CityList& CityList::operator -= (const City &rhs)
{
  ListNode *ptr = head, *prev = NULL;
  while (ptr && !(ptr->data).isEqual(rhs->data))
  {
  	prev = ptr;
  	ptr = ptr->next;
  } // ptr != NULL if found data

  if (ptr) // if found city
  {
    if (!prev) // if is at front of the list
      head = ptr->next;
    else // if in the middle
    {
      prev->next = ptr->next;
    } // else
    count -= 1;
  } //if
  return *this;
}//operator -=

const City CityList::operator [] (int index) const //for access
{
  ListNode *ptr = head;
  for (int i = 0; i < index; i++)
  {
  	ptr = ptr->next;
  }// for
  return ptr->data;
}

City& CityList::operator [] (int index) // for modification
{
  ListNode *ptr = head;
  for (int i = 0; i < index; i++)
  {
  	ptr = ptr->next;
  } // for
  return ptr->data;
}
