#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <cstring>
#include "vector.h"
#include "plane.h"
using namespace std;

void displayPlaneInformation(Plane *planes, int planeNum);
void addPlaneInformation(Plane *planes, int planeNum);
void determineBestPlane(Plane *planes, int planeNum, const Vector *cities);

void run(const Vector *cities, Plane *planes, int *planeNum)
{
  int choice;

  while (1)
  {
    if ((choice = Vector::getChoice()) == 0)
      break;

    else if (choice == 1) // else
      cities->runChoice1();

    else if (choice == 2) // else
      cities->runChoice2();

    else if (choice == 3) // else
    {
      Plane::planeInfoHead();
      displayPlaneInformation(planes, *planeNum);
    } // choice == 3

    else if (choice == 4)// else
    {
      addPlaneInformation(planes, *planeNum);
      (*planeNum)++;
    } //choice == 4

    else if (choice == 5) //else
      determineBestPlane(planes, *planeNum, cities);
  }  // infinite while loop

  return;
}  // run())

void displayPlaneInformation(Plane *planes, int planeNum)
{
  for (int i = 0; i < planeNum; i++)
  {
    cout << planes[i] << endl;
  } // for
} //displayPlaneInformation

void determineBestPlane(Plane *planes, int planeNum, const Vector *cities)
{
  int minDis, minPass;
  int cost, trips;
  int bestPlaneIndex;

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

void addPlaneInformation(Plane *planes, int planeNum)
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

