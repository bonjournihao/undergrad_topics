#ifndef CITYLIST_H
#define CITYLIST_H
#include "city.h"
using namespace std;


class CityNode
{
  City singleCity;
  CityNode *next;
public:
  CityNode();
  CityNode(City c, CityNode *n): singleCity(c), next(n){}
  ~CityNode();
  friend class CityList;
}; // class CityNode

class CityList
{
  CityNode *head;
  CityNode *tail;
public:
  static int count = 0;
  CityList():head(NULL), tail(NULL), count(0){}
  ~CityList(); 
  static int getCount();
  CityList& operator += (const City &rhs);
  CityList& operator -= (const City &rhs);
  const City operator [] (int index) const;
  City& operator [] (int index);
}; // class CityList

#endif // CITYLIST_H