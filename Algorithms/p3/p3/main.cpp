#include <iostream>
#include <cstdlib>
#include <cstring>
#include "vector.h"
using namespace std;


void run(const Vector *cities)
{
  char airport1[80], airport2[80], airport3[80];
  int index1, index2, index3, choice;

  while (1)
  {
   // choice = getChoice();

    if ((choice = cities->getChoice()) == 0)
      break;

    if (choice == 1)
    {
      cout << "\nPlease enter two airport abbreviations (XXX XXX): ";
      cin >> airport1 >> airport2;
      cin.ignore();
      index1 = cities->findAirport(airport1);
      index2 = cities->findAirport(airport2); 

      if (index1 >= 0 && index2 >= 0)
        cities->calcDistance(index1, index2);
    } // if choice == 1
    else //else choice == 2 
    {
      cout << "\nPlease enter an airport abbreviation (XXX): ";
      cin >> airport3;
      cin.ignore();
      index3 = cities->findAirport(airport3);

      if (index3 >= 0)
        cities->calcAirportTraffic(index3);
    }// else choice == 2
  }  // infinite while loop
 
  return;
}  // run())


int main() 
{
  Vector cities;
  cities.readCities();
  cities.readAirports();
  cities.cleanCities();

  run(&cities);
  return 0;
} // main())

