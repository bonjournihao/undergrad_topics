#include <stdio.h>
#include <stdlib.h>
#include "city.h"
#include <string.h>
#include <stdbool.h>
#include <math.h>

//initialize a City struct
void initialize(City *city)
{
  city->longitude = 0.0; 
  city->latitude = 0.0; 
  city->population = 0.0;
  city->name = NULL;
  city->state = NULL;
  return;
} //initialize

//read and store population information of a city from file pointer
int readCityInfo(FILE *filePtr, City *city)
{
  char line[100];
  
  if (feof(filePtr))
  {
    return 0; //indicates over
  } //if

  fgets(line, 100, filePtr);

  parseLine(line, city);
  return 1; //check if need to resize vector
} //readCityInfo()

//Assign the struct variables with corresponding values
void parseLine(char *line, City *city)
{
  int which = 0;
  char *word;
  word = strtok(line, ",\n");
 
  while (word != NULL)
  {
    switch(which)
    {
      case 0 :
        city->name = strdup(word);
        which = 1;
        break;
      
      case 1 :
        city->state = strdup(word);
        which = 2;
        break;
      
      case 2 :
      	city->population = double(atoi(word));
        break;
    } //switch

    word = strtok(NULL, ",\n");
  } //while
} //parseLine()

//test if two cities are equal in name or state
bool isEqual(const City *city1, const City *city2)
{
  if (city1 == NULL && city2 == NULL)
  {
    return true;
  } //if

  if (city1 == NULL || city2 == NULL)
  {
    return false;
  } //if

  bool re = false;

  if (city1->name != NULL && city2->name != NULL)
  {
  	re = strcmp(city1->name, city2->name) == 0;
  }//if

  if (strlen(city1->airport) != 0 && strlen(city2->airport) != 0)
  {
  	re = re || strcmp(city1->airport, city2->airport) == 0;
  }//if

  return re;
} // isEqual()

//copy citySrc to cityDes
void copyLocation(City* cityDes, const City* citySrc)
{
  cityDes->longitude = citySrc->longitude;
  cityDes->latitude = citySrc->latitude;
  strcpy(cityDes->airport, citySrc->airport);
  return;
} //copyLocation()

// copy all information of a city to another
void copyCity(City* cityDes, City* citySrc) 
{
  cityDes->longitude = citySrc->longitude;
  cityDes->latitude = citySrc->latitude;
  cityDes->population = citySrc->population;
  cityDes->name = strdup(citySrc->name);
  cityDes->state = strdup(citySrc->state);
  
  if (strlen(citySrc->airport) != 0) 
  {
    strcpy(cityDes->airport, citySrc->airport);
  }//if

  return;
} //copyCity()

//check if a city is useful; if not, return 0
int hasAirport(City* city) 
{
  if (strlen(city->airport) == 0)
  {
    return 0;
  } //if
  
  else //if there is airport, return 1
  {
    return 1;
  } //else
} // hasAirport()

//calculate travel distance between two cities
void calcDistance(const City *city1, const City *city2, int *distance)
{
  double distanceDouble;
  double la1, la2, lo1, lo2;
  const double R = 3963.0;
  la1 = (city1-> latitude) * M_PI / 180;
  la2 = (city2-> latitude) * M_PI / 180;
  lo1 = (city1-> longitude) * M_PI / 180;
  lo2 = (city2-> longitude) * M_PI / 180;
  distanceDouble = acos(sin(la1) * sin(la2) + cos(la1) \
* cos(la2) * cos(lo1 - lo2)) * R;
  *distance = (int)distanceDouble;
} //calcDistanceCity()

//calculate # of passengers between two cities
int calPass(City* city1, City* city2)
{
  int numPass;
  numPass = (city1->population) * (city2->population) / 250000000.0;
  return numPass;
} //calPass()

//deallocate memory for a City
void deallocate(City *city)
{
  if (city->name != NULL)
  {
    free(city->name);	
  }//if
  
  if (city->state != NULL)
  {
    free(city->state);	
  }//if
} //deallocate

//copy an airport name to a City struct variable
void setAirport(City *city, const char* airport)
{
  strcpy(city->airport, airport);
} //setAirport()

void printPassAndDistance(City *city1, City *city2, int numPass, int distance)
{
  if (distance < 100)
  {
    printf("0 passengers fly the %d miles from\n%s,%s to %s,%s\n",\
   distance, city1->name, city1->state, city2->name, city2->state);
  }//if
  
  else //print passenger number
  {
  printf("%d passengers fly the %d miles from\n%s,%s to %s,%s\n", numPass,\
   distance, city1->name, city1->state, city2->name, city2->state);
  }//else
}//printPassAndDistance



//read one-line-information and store it to City
void readAirport(City *city, char *line)
{
  char *token;
  int which = 0;
  char cityname[20];
  bool needSpace = false;
  token = strtok(line, "[] \n");
  
  while (token != NULL)
  {
    switch(which)
    {
      case 0 :
        strcpy(city->airport, token);
        which = 1;
        break;

      case 1 :
        sscanf(token, "%lf", &(city->latitude));
        which = 2;
        break;

      case 2 :
        sscanf(token, "%lf", &(city->longitude));
        which = 3;
        break;

      case 3 :

        if (needSpace)
        {
          strcat(cityname, " ");
        }//if needSpace has been changed to true
  
        strcat(cityname, token);
        needSpace = true;
        break;
    } // switch

    token = strtok(NULL, "[] \n");
  } // while

  city->name = strdup(cityname);
  return;
} // readAirport()
