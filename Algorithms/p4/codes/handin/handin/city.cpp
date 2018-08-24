#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include "city.h"
using namespace std;

City::City():name(NULL), state(NULL)
{
  airport[0] = '\0';  // sentinel value
}  // default constructor()


City::~City()
{
  if (name)
  {
    delete [] name;
    name = NULL;
  } //if

  if (state)
  {
    delete [] state;
    state = NULL;
  } //if
}  // destructor

City& City::operator=(const City &rhs)
{
  if (this == &rhs)
    return *this;
 
  if (name)
    delete name;
  
  if (state)
    delete state;
  population = rhs.population;
  
  if (rhs.name)
  {
    name = new char[strlen(rhs.name) + 1];
    strcpy(name, rhs.name);
  } //copy name
 
  if (rhs.state)
  {
    state = new char[strlen(rhs.state) + 1];
    strcpy(state, rhs.state);
  } //copy state
 
  if (rhs.airport)
  {
    strcpy(airport, rhs.airport);
  } //if

  longitude = rhs.longitude;
  latitude = rhs.latitude;
  return *this;
}// overloaded operator()


void City::deallocate()
{
  if (name)
  {
    delete [] name;
    name = NULL;
  } //if

  if (state)
  {
    delete [] state;
    state = NULL;
  } //if
}  // deallocate()


void City::calcDistance(const City *city2, int choice, int *dis, int *pass)const
{
  int distance, passengers;
  
  passengers = (double) population * city2->population / 2500000000.0;
  distance = acos(
    sin(latitude * M_PI / 180) * sin(city2->latitude * M_PI / 180) 
    + cos(latitude * M_PI / 180) * cos(city2->latitude * M_PI / 180)
    * cos((longitude - city2->longitude) * M_PI / 180)) * 3963;

  if (distance < 100)
    passengers = 0;
  
  if (choice == 4)
  {
    *dis = distance;
    *pass = passengers;
    return;
  }//if

  cout << passengers << " passengers fly the " << distance << " miles from\n";
  cout << name << ", " << state << " to ";
  cout << city2->name << ", " << city2->state << ".\n";
  return;
}  // calcDistance())

int City::calcAirportTraffic(const City *city)const
{
  if (isEqual(city))
  {
    return 0;
  } // if

  int distance, passengers;
  
  passengers = (double) population * city->population / 2500000000.0;
  distance = acos(
    sin(latitude * M_PI / 180) * sin(city->latitude * M_PI / 180) 
    + cos(latitude * M_PI / 180) * cos(city->latitude * M_PI / 180)
    * cos((longitude - city ->longitude) * M_PI / 180)) * 3963;
  
  if (distance < 100)
  {
    cout << city->name << ", " << city->state << ": " << 0 << "\n";
    return 0;
  } //if

  cout << city->name << ", " << city->state << ": " << passengers << "\n";
  return passengers;
}  // calcAirportTraffic()

void City::copyLocation(const City *srcCity)
{
  strcpy(airport, srcCity->airport);
  latitude = srcCity->latitude;
  longitude = srcCity->longitude;
}  // copyLocation()


bool City::hasAirport()
{
  return airport[0] != '\0';
}  // hasAirport()


bool City::isEqual(const City *city2)const
{
  if (name && city2->name && state && city2->state)
    return strcmp(name, city2->name) == 0 && strcmp(state, city2->state) == 0;
  
  if (airport[0] && city2->airport[0])
    return strcmp(airport, city2->airport) == 0;
  
  return false;
}  // isName()


void City::readCity(ifstream &myfile)
{
  char line[1000], *ptr;
  
  if (!(myfile.getline(line, 100)) || !strstr(line, ","))
    return;

  strcat(line, "\n");
  ptr = strtok(line, ",");
  
  if (ptr)
  {  
    name = new char[strlen(ptr) + 1];
    strcpy(name, ptr);
    ptr = strtok(NULL, ",");
    state = new char[strlen(ptr) + 1];
    strcpy(state, ptr);
    population = atoi(strtok(NULL, ",\n"));
  } // if something on line
}  // readCity()

void City::readAirport(char *line, char *airportState)
{
  char *ptr;
  
  strtok(line, "] ");
  strcpy(airport, &line[1]);
  latitude = atof(strtok(NULL, " "));
  longitude = atof(strtok(NULL, " "));
  ptr = strtok(NULL, ",") + 1;
  name = new char[strlen(ptr) + 1];
  strcpy(name, ptr);
  state = strdup(airportState);
}  // readAirport

void City::setAirport(const char *airport2)
{
  strcpy(airport, airport2);
}  // setAirport()
