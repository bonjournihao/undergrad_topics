#ifndef AIRPORT_H
#define AIRPORT_H
#include "city.h"
#include "list.h"
using namespace std;

class Airport:public City
{
  // double longitude;
  // double latitude;
  // char *name;
  // char *state;
  // char airport[4];
  // int population;
protected:
  List<Flight> flights;
public:
  void readInAirlines(List<Airport> &cities, char *airCompany, char *arrivCity);
  friend ostream& operator << (ostream &os, const Airport &rhs);
  void findRoute(Airport depAirport, Airport arrivAirport, List<Airport> cities, List<Flight> flights);
};// class Airport

class Flight
{
public:
  Flight();
  friend class Airport;
  char airline[10];
  char desAirport[4];
  bool operator < (const Flight &rhs)const;
  friend ostream& operator << (ostream &os, const Flight &rhs);
}; // class Flight

#endif // AIRPORT_H