// Author: Sean Davis 

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <climits>
#include "vector.h"
#include "plane.h"
using namespace std;


void addPlaneInformation(Plane planes[], int *planeCount)
{
  planes[*planeCount].getPlane();
  ofstream outf("planes.dat", ios::app | ios::binary);
  outf.write((char*) &planes[(*planeCount)++], sizeof(Plane));
  outf.close();
}  // addPlaneInformation()


void calcDistance(const Vector *cities)
{
  char airport1[80], airport2[80];
  int index1, index2, distance, passengers;
  cout << "\nPlease enter two airport abbreviations (XXX XXX): ";
  cin >> airport1 >> airport2;
  index1 = cities->findAirport(airport1);
  index2 = cities->findAirport(airport2);

  if (index1 >= 0 && index2 >= 0)
    cities->calcDistance(index1, index2, &distance, &passengers, true);
}  // calcDistance())


void determineAirportTraffic(const Vector *cities)
{
  char airport[80];
  int index;
  cout << "\nPlease enter an airport abbreviation (XXX): ";
  cin >> airport;
  index = cities->findAirport(airport);

  if (index >= 0)
    cities->calcAirportTraffic(index);
}  // determinAirportTraffic()


void determineBestPlane(const Vector *cities, const Plane *planes, 
                        int planeCount)
{
  char airport1[80], airport2[80];
  int index1, index2, distance, passengers, minIndex = -1, minCost = INT_MAX, 
    minTrips, trips, cost;
  cout << "\nPlease enter two airport abbreviations (XXX XXX): ";
  cin >> airport1 >> airport2;
  index1 = cities->findAirport(airport1);
  index2 = cities->findAirport(airport2);

  if (index1 >= 0 && index2 >= 0)
  {
    cities->calcDistance(index1, index2, &distance, &passengers, false);
      
    for (int i = 0; i < planeCount; i++)
    {
       cost = planes[i].calcCost(distance, passengers, &trips);
       
       if (trips > 0 && cost < minCost)
       {
         minTrips = trips;
         minCost = cost;
         minIndex = i;
       }  // if lower cost
    }  // for each plane
    
    if (minIndex == -1)
      cout << "No planes available\n";
    else // else found a plane
    {
      cout << "Passengers Miles  Trips Name      Cost\n";
      cout << left << setw(11) << passengers << setw(7) << distance << setw(6) 
        << minTrips << setw(10) << planes[minIndex].getName() << '$' << minCost 
        << endl;
    } // else found a plane
  }   // if valid inputs  
}  //  determineBestPlane()


void displayPlaneInformation(const Plane *planes, int planeCount)
{
  cout << "\nPlane Information\n";
  cout << "Name        Pass. Range Speed Fuel   MPG   $/mi  Price * 10^6\n";
  
  for (int i = 0; i < planeCount; i++)
    cout << planes[i];

}  // displayPlaneInformation()


int getChoice()
{
  int choice;

  do
  {
    cout << "\nFlight Simulator Menu\n";
    cout << "0. Done.\n";
    cout << "1. Determine distance and passengers between two airports.\n";
    cout << "2. Determine all traffic from one airport.\n";
    cout << "3. Display planes information.\n";
    cout << "4. Add plane information.\n";
    cout << "5. Determine best plane between two airports.\n\n";

    cout << "Your choice (0 - 5): ";
    cin >> choice;
    cin.ignore(1000, '\n');
   
    if  (choice < 0 || choice > 5)
      cout << "Your choice must be between 0 and 5. Please try again.\n";
  } while(choice < 0 || choice > 5);
   
  return choice;
}  // getChoice()  


void readPlanes(Plane planes[], int *planeCount)
{
  ifstream inf("planes.dat", ios::binary);
  *planeCount = 0;
  
  if (! inf)
    return;

  while (inf.read((char*) &planes[(*planeCount)++], sizeof(Plane)));

  --(*planeCount);
  inf.close();
} // readPlanes()



void run(const Vector *cities, Plane *planes, int planeCount)
{
  int choice;
  
//  for(index1 = 0; index1 < cities->count; index1++)
//    for(index2 = index1 + 1; index2  < cities->count; index2++)
//      calcDistance(cities, index1, index2);

  do
  {
    choice = getChoice();
    
    switch(choice)
    {
      case 1 : calcDistance(cities); break;
      case 2 : determineAirportTraffic(cities); break;
      case 3 : displayPlaneInformation(planes, planeCount); break;
      case 4 : addPlaneInformation(planes, &planeCount); break;
      case 5 : determineBestPlane(cities, planes, planeCount); break;
    }  // switch
  } while(choice != 0);
}  // run())


int main(int argc, char** argv) 
{
  Vector cities;
  Plane planes[10];
  int planeCount = 0;
  cities.readCities();
  cities.readAirports();
  cities.cleanCities();
  readPlanes(planes, &planeCount);
 
//  srand(1);
//  for(int i = 0; i < cities.count; i++)
//    cout << "1\n" << cities.cityArray[i].airport << ' ' << cities.cityArray[rand() % cities.count].airport << endl;
//  printf("%s %s %lf %lf\n", cities.cityArray[i].name, cities.cityArray[i].airport, cities.cityArray[i].latitude, cities.cityArray[i].longitude);
  run(&cities, planes, planeCount);
  return 0;
} // main())

