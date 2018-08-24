#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <cstring>
#include "vector.h"
#include "plane.h"
using namespace std;

void displayPlaneInformation(const Plane *planes, int planeNum);
void addPlaneInformation(Plane *planes, int planeNum);
void determineBestPlane(const Plane *planes, \
                        int planeNum, const Vector *cities);

void run(const Vector *cities, Plane *planes, int *planeNum)
{
  int choice = -1;
  
  while (choice != 0)
  {
    choice = Vector::getChoice();

    switch(choice)
    {
      case 1:
        cities->runChoice1();
        break;
      case 2:
        cities->runChoice2();
        break;

      case 3:
      {
        Plane::planeInfoHead();
        displayPlaneInformation(planes, *planeNum);
        break;
      } //choice 3

      case 4:
      {
        addPlaneInformation(planes, *planeNum);
        (*planeNum)++;
        break;
      } //choice == 4

      case 5:
        determineBestPlane(planes, *planeNum, cities);
    } //run choice
  } // get choice from input

  return;
}  // run())

void displayPlaneInformation(const Plane *planes, int planeNum)
{
  for (int i = 0; i < planeNum; i++)
  {
    cout << planes[i] << endl;
  } // for
} //displayPlaneInformation

void determineBestPlane(const Plane *planes,\
                        int planeNum, const Vector *cities)
{
  int minDis, minPass;
  int cost, trips;
  int bestPlaneIndex;

  planes = (Plane*) planes;
  bestPlaneIndex = Plane::getName(planes, planeNum, cities, \
  &minDis, &minPass, &cost);

  if (bestPlaneIndex == -1)
  {
    cout << "No planes available" << endl;
    return;
  } // if

  trips = ceil(double(minPass) / planes[bestPlaneIndex].getPassengers());
  cout << setw(11) << left << "Passengers" << setw(7) << left << "Miles";
  cout << setw(6) << left << "Trips" << setw(10) << left << "Name" \
  << "Cost" << endl;
  cout << setw(11) << left << minPass << setw(7) << left << minDis;
  cout << setw(6) << left << trips << setw(10) << left \
  << planes[bestPlaneIndex].getPlaneName();
  cout << left << "$" << left << cost << endl;
} //print best plane info

void addPlaneInformation(Plane *planes, const int planeNum)
{
  ofstream out("planes.dat", ios::out | ios::binary | ios::app);
  char name[12];
  int passengers, fuel, range, speed, price;
  cout << "Name: ";
  cin >> name;
  cout << "Passengers: ";
  cin >> passengers;
  cout << "Fuel capacity (in U.S. gallons): ";
  cin >> fuel;
  cout << "Range (in miles): ";
  cin >> range;
  cout << "Speed (in mph): ";
  cin >> speed;
  cout << "Price: ";
  cin >> price;
  cin.ignore();
  planes[planeNum].setValues(name, passengers, fuel, range, speed, price);
  out.write((char *) &planes[planeNum], sizeof(Plane));
  out.close();
} //addPlaneInformation

int main() 
{
  Vector cities;
  Plane planes[10];
  int planeNum = 0; //actual number + 1

  cities.readCities();
  cities.readAirports();
  cities.cleanCities();
  Plane::readPlanes(planes, &planeNum);

  run(&cities, planes, &planeNum);
  return 0;
} // main())

