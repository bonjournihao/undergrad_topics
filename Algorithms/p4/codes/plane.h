#ifndef PLANE_H
#define PLANE_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include "city.h"
#include "vector.h"
using namespace std;

class Plane
{
  char name[12];
  int passengers;
  int range; // in miles
  int speed; // in miles per hour
  int fuel; // in U.S. gallons
  int price; // in U.S. dollars

public:
  Plane() {}
  void readPlane(ifstream *in);
  int getCost(int dis, int pass)const;
  void setValues(const char *nameVal, int passengersVal, \
                 int fuelVal, int rangeVal, int speedVal, int priceVal);
  const char* getPlaneName()const;
  int getPassengers()const;
  static const double PPG = 3.39; 
  static void readPlanes(Plane *planes, int *planeNum);
  static bool isInvalidLine(const Plane *planes, int planeNum);
  static int getName(const Plane *planes, int planeNum, const Vector *cities, \
                     int *minDis, int *minPass, int *cost);
  static void planeInfoHead();

  friend ostream& operator<<(ostream &os, const Plane &rhs);
  Plane& operator=(const Plane &rhs);
}; //place class

#endif
