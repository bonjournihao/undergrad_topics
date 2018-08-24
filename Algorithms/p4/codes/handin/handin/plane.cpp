#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <climits>
#include <cctype>
#include <cstring>
#include "plane.h"
using namespace std;

void Plane::readPlanes(Plane *planes, int *planeNum)
{
  ifstream in;
  in.open("planes.dat", ios::in | ios::binary);

  if (!in.is_open())
    return;
  
  while (!in.eof())
  {
    planes[*planeNum].readPlane(&in);

    if (isInvalidLine(planes, *planeNum))
    	continue;
    (*planeNum)++;
  } // while

  in.close();
} // readPlanes()

bool Plane::isInvalidLine(const Plane *planes, int planeNum)
{
  if (!(strchr(planes[planeNum].name, '-') && \
      isdigit(planes[planeNum].name[0])))
    return true;
  return false;
} // invalidLine()

void Plane::readPlane(ifstream *in)
{
  (*in).read((char *)this, sizeof(Plane));
} // readPlane()

ostream& operator<<(ostream &os, const Plane &rhs)
{
  int remainder1000, remainder100, remainder10, remainder1;

  os << setw(12) << left << rhs.name << setw(6) << left << rhs.passengers;
  remainder100 = rhs.range % 1000;
  remainder10 = remainder100 % 100;
  remainder1 = remainder10 % 10;
  os << left << rhs.range / 1000 << "," << remainder100 / 100 \
  << remainder10 / 10 << left << setw(2) << remainder1;
  os << setw(6) << left << rhs.speed;
  remainder1000 = rhs.fuel % 10000;
  remainder100 = remainder1000 % 1000;
  remainder10 = remainder100 % 100;
  remainder1 = remainder10 % 10;

  if (rhs.fuel < 10000)
  	os << setw(2) << right << remainder1000 / 1000 << "," \
  	<< remainder100 / 100 << remainder10 / 10 << setw(2) << \
  	left << remainder1;

  else //else
    os << rhs.fuel / 10000 << remainder1000 / 1000 << "," \
    << remainder100 / 100 << remainder10 / 10 << setw(2) << remainder1;

  os << setw(6) << left << setprecision(3) << fixed << \
  double(rhs.range) / double(rhs.fuel);
  os << setw(5) << right << setprecision(2) << fixed << \
  Plane::PPG * (double(rhs.fuel) / double(rhs.range));
  os << setw(2) << right << "$";
  os << setw(5) << right << setprecision(1) << double(rhs.price) / 1000000.0;
  return os;
} // operator <<

Plane& Plane::operator=(const Plane &rhs)
{
  if (this == &rhs)
    return *this;

  strcpy(name, rhs.name);
  passengers = rhs.passengers;
  range = rhs.range;
  speed = rhs.speed;
  fuel = rhs.fuel;
  price = rhs.price;
  return *this;
} // operator=

void Plane::setValues(const char *nameVal, int passengersVal, int fuelVal, \
                      int rangeVal, int speedVal, int priceVal)
{
  strcpy(name, nameVal);
  passengers = passengersVal;
  fuel = fuelVal;
  range = rangeVal;
  speed = speedVal;
  price = priceVal;
} // setValues()

const char* Plane::getPlaneName()const
{
  return name;
} // getPlaneName()

int Plane::getPassengers()const
{
  return passengers;
} // getPassengers()

int Plane::getName(const Plane *planes, int planeNum, const Vector *cities, \
                   int *minDis, int *minPass, int *cost)
{
  int index1, index2, currentCost, lowCostPlaneIndex = 0, lowestCost = INT_MAX;
  char airport1[80], airport2[80];

  cout << "\nPlease enter two airport abbreviations (XXX XXX): ";
  cin >> airport1 >> airport2; //get two airports
  cin.ignore();
  index1 = cities->findAirport(airport1);
  index2 = cities->findAirport(airport2); 

  if (index1 >= 0 && index2 >= 0)
    cities->calcDistance(index1, index2, 4, minDis, minPass);

  if (*minPass == 0)
    return -1;

  for (int i = 0; i < planeNum; i++) //lowest cost plane
  {
    if (planes[i].range >= *minDis)
    {
      if ((currentCost = planes[i].getCost(*minDis, *minPass)) < lowestCost)
      {
        lowestCost = currentCost;
        lowCostPlaneIndex = i;
      } // if
    } // if
  } // for

  if (lowestCost == INT_MAX)
    return -1;

  *cost = lowestCost;
  return lowCostPlaneIndex;
} // getName()


int Plane::getCost(int dis, int pass)const
{
  int totalCost, trips;
  double fuelCost, attendant, pilot, maintenance;

  trips = ceil(double(pass) / passengers);
  fuelCost = PPG * (double(fuel) / range) * dis * trips;
  attendant = ceil(passengers / 100.0) * 30 * ceil(2.0 + dis / \
  double(speed)) * trips;
  pilot = 2 * 100 * ceil(2.0 + dis / double(speed)) * trips;
  maintenance = 0.000025 * price * (double(dis) / speed) * trips;

  totalCost = ceil(fuelCost + attendant + pilot + maintenance);
  return totalCost;
} // getCost()

void Plane::planeInfoHead()
{
  cout << "\nPlane Information" << endl;
  cout << setw(12) << left << "Name";
  cout << setw(6) << left << "Pass.";
  cout << setw(6) << left << "Range";
  cout << setw(6) << left << "Speed";
  cout << setw(7) << left << "Fuel";
  cout << setw(6) << left << "MPG";
  cout << setw(6) << left << "$/mi";
  cout << setw(12) << "Price * 10^6" << endl;
  return;
} // planeInfoHead()
