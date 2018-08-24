#ifndef CITY_H
#define	CITY_H
#include <iostream>
#include <fstream>
using namespace std;

class City
{
  double longitude;
  double latitude;
  char *name;
  char *state;
  char airport[4];
  int population;

public:
  City();
  ~City();
  City& operator=(const City &rhs);
  void deallocate();
  void calcDistance(const City *city2, int choice = 3, \
                    int *dis = 0, int *pass = 0)const;
  int calcAirportTraffic(const City *city)const;
  void copyLocation(const City *srcCity);
  bool hasAirport();
  bool isEqual(const City *city2)const;
  void readAirport(char *line, char *state);
  void readCity(ifstream &myfile); 
  void setAirport(const char *airport);

}; //City class

#endif	// CITY_H 
