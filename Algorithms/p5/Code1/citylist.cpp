#include "citylist.h"
#include "citynode.h"
#include "city.h"
#include <iostream>
#include <stdexcept>

int CityList::count = 0;
CityList::CityList()
{
  head = NULL;
  tail = NULL;	
}

CityList::~CityList()
{
  CityNode* cur = head;
  CityNode* next;
  while(cur != NULL)
  {
  	next = cur->next;
  	delete cur;
  	cur = next;
  }
  count = 0;
  head = NULL;
  tail = NULL;
}

int CityList::getCount()
{
  return count;
}

CityList& CityList::operator+=(City &city)
{
  if (head == NULL)
  {
  	head = new CityNode(city);
  	tail = head; 
  }
  else
  {
    tail->next = new CityNode(city);
    tail = tail->next;
  }
  count++;
  return *this;
}

CityList& CityList::operator-=(City &city)
{
  CityNode *cur = head;
  if (cur != NULL && cur->city == city)
  {
  	head = cur->next;
  	delete cur;
    count--;
  }
  else
  {
  	while(cur->next != NULL)
  	{
	  if(cur->next->city == city)
	  {
	  	CityNode *tmp = cur->next;
	  	if (tmp == tail) {
	  	  tail = cur;
	  	}
        cur->next = cur->next->next;
        delete tmp;
        count--;
        break;
	  }
	  cur = cur->next;
  	}
  }
  return *this;
}

const City& CityList::operator[](int index)const
{
  if (index < 0 || index >= count)
  {
    throw std::out_of_range("out of range");
  }
  CityNode *cur = head;
  while(cur != NULL)
  {
    if (index-- == 0)
      break;
    cur = cur->next;
  }
  return cur->city;
}

City& CityList::operator[](int index)
{
  if (index < 0 || index >= count)
  {
  	throw std::out_of_range("out of range");
  }
  CityNode *cur = head;
  while(cur != NULL)
  {
  	if (index-- == 0)
  	  break;
  	cur = cur->next;
  }
  return cur->city;
}