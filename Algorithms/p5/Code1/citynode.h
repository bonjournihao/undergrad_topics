#ifndef CITYNODE_H
#define	CITYNODE_H
#include "city.h"
// Author Sean Davis

class CityNode
{
  friend class CityList;
  private:
    City city;
    CityNode* next;
  public:
    CityNode(const City &city);
    ~CityNode();
};

#endif