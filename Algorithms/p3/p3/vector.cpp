#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <string>
#include "vector.h"
using namespace std;


Vector::Vector():size(10), count(0)
{
  cityArray = new City[size];

}  // initialize()


Vector::~Vector()
{
  for (int i = 0; i < count; i++)
  {
    cityArray[i].deallocate();
  }// delete city class

  delete [] cityArray;
}  // delete Vector class


void Vector::calcDistance(int index1, int index2)const
{
  cityArray[index1].calcDistance(&cityArray[index2]);
  return;
}  // calcDistance()

void Vector::calcAirportTraffic(int index3)const
{
  int totalPass = 0;

  for (int i = 0; i < count; i++)
  {
    totalPass += cityArray[index3].calcAirportTraffic(&cityArray[i]);
  } // for

  cout << "Total passengers: " << totalPass << "\n";
  return;
}  // calcAirportTraffic()

void Vector::cleanCities()
{
  int i = 0, j = 0;

  for (i = 0; i < count; i++)
  {
    if (cityArray[i].hasAirport())
    {
      cityArray[j++] = cityArray[i];

      if (j - 1 != i)
      {
        cityArray[i].deallocate();
      } //if
    } //if
  } //if

  count = j;
}  // cleanCities())


int Vector::findAirport(const char *airport)const
{
  City city;
  
  city.setAirport(airport);

  for (int i = 0; i < count; i++)
  {
    if (cityArray[i].isEqual(&city))
    {
      return i;
    } //if
  } //if

  cout << airport << " is not a valid airport.\n";
  return -1;
}  // findAirport()


void Vector::readAirports()
{
  char line[1000];
  City city;
  ifstream myfile;
  myfile.open("airportLL.txt");
  char *locState = NULL;

  while (myfile.getline(line, 1000))
  {
    if (line[0] == '[')
    {
      city.readAirport(line, locState);
      
      for (int i = 0; i < count; i++)
        if (cityArray[i].isEqual(&city))
        {
          cityArray[i].copyLocation(&city);
          break;
        }  // if found a matching name
      
      city.deallocate();
    }  // if an airport line

    else //else
      if (line[0] != '\0')
      {
        if (locState != NULL)
          delete [] locState;
        locState = strdup(line);
      } // if
  }  // while

  if (locState)
    delete [] locState;
  myfile.close();
}  // readAirports()


void Vector::readCities()
{
  ifstream myfile;
  myfile.open("citypopulations.csv", ifstream::in);

  while(!myfile.eof())
  {
    if (size == count)
    {
      resize();
    } //if

    cityArray[count++].readCity(myfile);
  } // while more in file

  count--;
  myfile.close();
}  // readCities()


void Vector::resize()
{
  int i;
  City *temp = new City[2 * size];

  for (i = 0; i < size; i++)
  {
    temp[i] = cityArray[i];
  } //for

  size *= 2;
  delete [] cityArray;
  cityArray = temp;
}  // resize()

int Vector::getChoice()const
{
  while(1)
  {
    cout << "\nFlight Simulator Menu\n0. Done.\n";
    cout << "1. Determine distance and passengers between two airports.\n";
    cout << "2. Determine all traffic from one airport.\n\n";
    cout << "Your choice (0 - 2): ";
  
    string choiceStr;
    int choice;
    getline(cin, choiceStr);
    choice = atoi(choiceStr.c_str());

    if (choice == 0 && !(choiceStr.length() == 1 \
        && choiceStr.c_str()[0] == '0'))
    {
      continue;
    } // if
 
    if (choice == 0 || choice == 1 || choice == 2)
    {
      return choice;
    } // if

    else //else
    {
      cout << "Your choice must be between 0 and 2. Please try again." << endl;
    } // else
  }//while loop
}// getChoice()
