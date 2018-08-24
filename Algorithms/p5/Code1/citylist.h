#ifndef CITYLIST_H
#define	CITYLIST_H
#include "citynode.h"
#include "city.h"
// Author Sean Davis

class CityList
{
  CityNode *head;
  CityNode *tail;
  static int count;

  public:
    CityList();
    ~CityList();
    static int getCount();
    CityList& operator+=(City &city);
    CityList& operator-=(City &city);
    const City& operator[](int index)const;
    City& operator[](int index);
};

#endif