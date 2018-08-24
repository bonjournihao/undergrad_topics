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
  void calcDistance(int index1, int index2, \
                    int choice = 3, int *dis = 0, int *pass = 0)const;
  void calcAirportTraffic(int index3)const;
  void deallocate();
  void cleanCities();
  void readAirports();
  void readCities();
  static int getChoice();
  void runChoice1()const;
  void runChoice2()const;
}; //Vector class

#endif	// VECTOR_H 

