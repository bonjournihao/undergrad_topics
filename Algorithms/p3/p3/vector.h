#ifndef VECTOR_H
#define	VECTOR_H
#include "city.h"

class Vector
{
  City *cityArray;
  int size;
  int count;
  void resize();

public:
  Vector(); //default constructor
  ~Vector(); //destructor
  int findAirport(const char *airport)const;
  void calcDistance(int index1, int index2)const;
  void calcAirportTraffic(int index3)const;
  void deallocate();
  void cleanCities();
  void readAirports();
  void readCities();
  int getChoice()const;
}; //Vector class

#endif	// VECTOR_H 

