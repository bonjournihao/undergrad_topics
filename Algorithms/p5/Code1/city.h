#ifndef CITY_H
#define	CITY_H
// Author: Sean Davis
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
  void readCity(istream &inf);
  bool isEqual(const City *city2) const;

public:
  City();
  City(const City &city);
  ~City();
  void calcDistance(const City *city2, int *distance, int *passengers, 
                    bool display) const;
  void copyLocation(const City *srcCity);
  void deallocate();
  bool hasAirport();

  void readAirport(char *line, const char *state2);
  void setAirport(const char *airport);
  int showTraffic(const City &destCity) const;
  City& operator= (const City& rhs);
  bool operator== (const City& rhs) const;
  friend istream& operator>>(istream &in, City &city);

};  // class City

#endif	// CITY_H 

