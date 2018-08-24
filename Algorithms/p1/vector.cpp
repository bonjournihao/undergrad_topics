#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "vector.h"
#include "city.h"

//initialize a Vector stuct that is passed in
void initialize(Vector *vector, int size)
{
  int i;
  vector->size = size;
  
  if (size == 10)
  {
    vector->count = 0;
    vector->cityArray = (City*)malloc((vector->size) * sizeof(City));
  } //if
  
  for (i = vector->count; i < size; i++)
  {
    initialize(vector->cityArray + i);
  } //for

  return;

} //initialize()

//resize the cityArray when the count exceeds array size; double the size
void resize(Vector *vector) 
{
  int i;
  City* doubeSizeCityArray = (City*)malloc((vector->size * 2) * sizeof(City));
 
  for (i = 0; i < vector->count; i++)
  {
    copyCity(doubeSizeCityArray + i, vector->cityArray + i);
  } // for

  for (i = 0; i < vector->count; i++)
  {
    deallocate(vector->cityArray + i);
  } //for

  if (vector->cityArray != NULL)
  {
    free(vector->cityArray);
  }//if

  vector->cityArray = doubeSizeCityArray;
  vector->size *= 2;
  initialize(vector, vector->size);
  
  return;
} //resize()

//deallocate cityArray after we create a new double-sized one
void deallocate(Vector *vector)
{
  int i;
 
  for (i = 0; i < vector->count; i++)
  {
    deallocate(&(vector->cityArray[i]));
  } //for
  
  if (vector->cityArray != NULL)
  {
    free(vector->cityArray);
  }//if

  return;
} //deallocate()

//store city population information into each City in cityArray
void storeCities(Vector *vector)
{
  initialize(vector, 10);
  FILE* filePtr;

  //open first file
  filePtr = fopen("citypopulations.csv", "r");
  
  while (readCityInfo(filePtr, vector->cityArray + vector->count))
  {
    vector->count += 1;

    if (vector->count == vector->size)
    {
      resize(vector);
    } //if
  } //while

  return;
} //storeCities()

//open second file; match and store airport information
void readAirports(Vector *vector)
{
  FILE *cityAirportFilePtr;
  char line[100];
  City currCity;
  int i = 0;
  cityAirportFilePtr = fopen("airportLL.txt", "r");
  initialize(&currCity);
 
  while (fgets(line, 100, cityAirportFilePtr))
  {
    if (strcmp(line, "\n") == 0)
    {
      continue;
    } //if
    
    if (line[0] == '[') //if the line has airport info
    {
      char *p = strstr(line, ",");
      *p = '\0';    
      readAirport(&currCity, line);
 
      for (i = 0; i < vector->count; i++)
      {
        if (isEqual(vector->cityArray + i, &currCity)) 
        {
          copyLocation(vector->cityArray + i, &currCity);
        } //if
      } //for
    } //else if 
  } //while

  deallocate(&currCity);
  return;
} //readAirports()



//clean useless cities
void cleanCities(Vector *vector) 
{
  int i, j;
  j = 0;
  City* cityArray = (City*)malloc((vector->size) * sizeof(City));
 
  for (i = 0; i < vector->count; i++)
  {
    if (hasAirport(vector->cityArray + i))
    {
      cityArray[j] = vector->cityArray[i];
      j++;
    } //if
    
    else //deallocate the old cityarray
    {
      deallocate(vector->cityArray + i);
    }//else if
  }// for

  free(vector->cityArray);
  vector->cityArray = cityArray;
  vector->count = j;
  return;
} //cleanCities()

//find the airport that user inputted exists; if not, return -1
int findAirport(const Vector *vector, const char* airportName)
{
  if (strlen(airportName) > 4)  
  {
    return -1;
  } //if

  City city;
  int i;

  initialize(&city);
  setAirport(&city, airportName);

  for (i = 0; i < vector->count; i++) 
  {
    if (isEqual(&city, vector->cityArray + i)) 
  {
      deallocate(&city);
      return i;
    } //if
  } //for

  deallocate(&city);
  return -1;
} //findAirport()

//calculates the distance between the two airports
//also calculates the # of passengers traveling between the two cities
//prints out the inforamtion
void calcDistance(const Vector *vector, int city1Index, int city2Index)
{
  City localCity1;
  City localCity2;
  initialize(&localCity1);
  initialize(&localCity2);
  copyCity(&localCity1, vector->cityArray + city1Index);
  copyCity(&localCity2, vector->cityArray + city2Index);
  int numPass = calPass(&localCity1, &localCity2);
  int distance;
  calcDistance(&localCity1, &localCity2, &distance);
  printPassAndDistance(&localCity1, &localCity2, numPass, distance);
  deallocate(&localCity1);
  deallocate(&localCity2);
  return;
} // calcDistance()

