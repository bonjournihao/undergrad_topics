// Author: Sean Davis 

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <climits>
#include "citylist.h"
#include "plane.h"
using namespace std;


void calcAirportTraffic(const CityList &cities, int index)
{
  int total = 0;
  
  for (int i = 0; i < CityList::getCount(); i++)
    if (i != index)
      total += cities[index].showTraffic(cities[i]);
  
  cout << "Total passengers: " << total << endl;
}  // calcAirportTraffic()

void calcDistance(const CityList &cities, int index1, int index2, int *distance, \
                int *passengers, bool display)
{
  cities[index1].calcDistance(cities[index2], distance, passengers,
    display);
}  // calcDistance()

void cleanCities(CityList &cities)
{
  int i = 0;
  
  while (i < CityList::getCount())
  {
    if (!cities[i].hasAirport())
    {
      cities -= cities[i];
    }  // if city does not have an airport
    else // city has an airport
      i++;
  }  // while more in array
}  // cleanCities())

int findAirport(const CityList &cities, const char *airport)
{
  City city;
  
  city.setAirport(airport);
  
  for (int i = 0; i < CityList::getCount(); i++)
    if (cities[i] == city )
      return i;
  
  cout << airport << " is not a valid airport.\n";
  return -1;
}  // findAirport()

void readAirports(CityList &cities)
{
  char line[1000], state2[80];
  City city;
  ifstream inf("airportLL.txt");

  while (inf.getline(line, 1000))
  {
    if (isalpha(line[0]))
      strcpy(state2, strtok(line, "\n"));
    
    if (line[0] == '[')
    {
      city.readAirport(line, state2);
      
      for (int i = 0; i < CityList::getCount(); i++)
        if (cities[i] == city)
        {
          cities[i].copyLocation(city);
          break;
        }  // if found a matching name
      
      city.deallocate();
    }  // if an airport line
  }  // while
}  // readAirports()


void readCities(CityList &cities)
{
  City cityTemp;
  istream &is = inf;
  ifstream inf("citypopulations.csv");
  
  while(!inf.eof())
  {
    is >> cityTemp;
    cities += cityTemp;
  } // while more in file
  
  inf.close();
}  // readCities()


void addPlaneInformation(Plane planes[], int *(CityList::getCount()))
{
  planes[*(CityList::getCount())].getPlane();
  ofstream outf("planes.dat", ios::app | ios::binary);
  outf.write((char*) &planes[(*(CityList::getCount()))++], sizeof(Plane));
  outf.close();
}  // addPlaneInformation()


void calcDistance(const CityList  &cities)
{
  char airport1[80], airport2[80];
  int index1, index2, distance, passengers;
  cout << "\nPlease enter two airport abbreviations (XXX XXX): ";
  cin >> airport1 >> airport2;
  index1 = findAirport(cities, airport1);
  index2 = findAirport(cities, airport2);

  if (index1 >= 0 && index2 >= 0)
    calcDistance(cities, index1, index2, &distance, &passengers, true);
}  // calcDistance())


void determineAirportTraffic(const CityList  &cities)
{
  char airport[80];
  int index;
  cout << "\nPlease enter an airport abbreviation (XXX): ";
  cin >> airport;
  index = cities.findAirport(airport);

  if (index >= 0)
    calcAirportTraffic(cities, index);
}  // determinAirportTraffic()


void determineBestPlane(const CityList  &cities, const Plane *planes, 
                        int CityList::getCount())
{
  char airport1[80], airport2[80];
  int index1, index2, distance, passengers, minIndex = -1, minCost = INT_MAX, 
    minTrips, trips, cost;
  cout << "\nPlease enter two airport abbreviations (XXX XXX): ";
  cin >> airport1 >> airport2;
  index1 = findAirport(cities, airport1);
  index2 = findAirport(cities, airport2);

  if (index1 >= 0 && index2 >= 0)
  {
    calcDistance(cities, index1, index2, &distance, &passengers, false);
      
    for (int i = 0; i < CityList::getCount(); i++)
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


void displayPlaneInformation(const Plane *planes, int CityList::getCount())
{
  cout << "\nPlane Information\n";
  cout << "Name        Pass. Range Speed Fuel   MPG   $/mi  Price * 10^6\n";
  
  for (int i = 0; i < CityList::getCount(); i++)
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


void readPlanes(Plane planes[], int *(CityList::getCount()))
{
  ifstream inf("planes.dat", ios::binary);
  *CityList::getCount() = 0;
  
  if (! inf)
    return;

  while (inf.read((char*) &planes[(*(CityList::getCount()))++], sizeof(Plane)));

  --(*(CityList::getCount()));
  inf.close();
} // readPlanes()



void run(const CityList  &cities, Plane *planes, int CityList::getCount())
{
  int choice;
  
//  for(index1 = 0; index1 < cities->CityList::getCount(); index1++)
//    for(index2 = index1 + 1; index2  < cities->CityList::getCount(); index2++)
//      calcDistance(cities, index1, index2);

  do
  {
    choice = getChoice();
    
    switch(choice)
    {
      case 1 : calcDistance(cities); break;
      case 2 : determineAirportTraffic(cities); break;
      case 3 : displayPlaneInformation(planes, CityList::getCount()); break;
      case 4 : addPlaneInformation(planes, &CityList::getCount()); break;
      case 5 : determineBestPlane(cities, planes, CityList::getCount()); break;
    }  // switch
  } while(choice != 0);
}  // run())


int main(int argc, char** argv) 
{
  CityList  cities;
  Plane planes[10];

  readCities(cities);
  readAirports(cities);
  cleanCities(cities);
  readPlanes(planes, &CityList::getCount());
 
//  srand(1);
//  for(int i = 0; i < cities.CityList::getCount(); i++)
//    cout << "1\n" << cities.cities[i].airport << ' ' << cities.cities[rand() % cities.CityList::getCount()].airport << endl;
//  printf("%s %s %lf %lf\n", cities.cities[i].name, cities.cities[i].airport, cities.cities[i].latitude, cities.cities[i].longitude);
  run(cities, planes, CityList::getCount());
  return 0;
} // main())

