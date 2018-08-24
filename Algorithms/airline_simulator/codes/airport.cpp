#include <iostream>
#include <cstdlib>
#include "list.h"
using namespace std;

bool Flight::operator < (const Flight &rhs)const
{
  return !strcmp(airline, rhs.airline);
} // operator <

void Airport::readInAirlines(List<Airport> &cities, char *airCompany, char *arrivCity)
{
  Flight localFlight;

  for (int i = 0; i < cities.getCount; i++)
  {
  	if (strcmp(airport, cities[i].airport) = 0)
  	{
      strcpy(localFlight.airline, airCompany);
      strcpy(localFlight.desAirport, arrivCity);
  	  cities[i].flights += localFlight;
  	} // if airport found, add 1 flight info to its flights 
  } // find airport in citylist
} // readInAirlines

ostream& Airport::operator << (ostream &os, const Airport &rhs)
{
  os << rhs.airport << ":";
  os << rhs.flights << endl;
  return os;
} // operator <<

ostream& Flight::operator << (ostream &os, const Flight &rhs)
{
  os << setw(1);
  os << setw(2) << rhs.airline;
  os << "-";
  os << rhs.desAirport;

  return os;
} // operator <<

/*
void Flight::setDistance()
{
  Airport arrivAirport;
  strcpy(arrivAirport.airport, desAirport);
  for 
  distance = acos(
    sin(latitude * M_PI / 180) * sin(city2->latitude * M_PI / 180) 
    + cos(latitude * M_PI / 180) * cos(city2->latitude * M_PI / 180)
    * cos((longitude - city2->longitude) * M_PI / 180)) * 3963;
}
*/

List<Flight>& Airport::findRoute(Airport depAirport, Airport arrivAirport, List<Airport> cities, List<Flight> flightRoute)
{
  // haven't pass in airline
  List<Flight> shortestRoute;
  if (depAirport == arrivAirport)
  	return flightRoute;

  for (int i = 0; i < cities.getCount(); i++)
  {
  	for (int j = 0; i < cities.getCount(); j++)
  	{
  	  if (cities[i] == depAirport && cities[j] == arrivCity)
      {
      	for (int k = 0; k < cities[i].flights.getCount(); k++)
      	{
      	  //the line below will not work!!
  	      Route = findRoute(cities[i].flights[k].desAirport, cities[j], cities, flightRoute);
  	      //need operator <, not even on either side
  	      // fixme!!
  	      if (Route < shortestRoute)
  	      	shortestRoute = Route;
  	        flightRoute += route;
  	    }
  	  }
  	}
  }
}