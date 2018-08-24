#ifndef CITY_H
#define CITY_H

typedef struct city
{
  double longitude;
  double latitude;
  char *name;
  char *state;
  char airport[4];
  double population;
} City; // City Struct

void initialize(City *city);
int readCityInfo(FILE *filePtr, City *city);
void parseLine(char *line, City *city);
bool isEqual(const City *city1, const City *city2);
void copyLocation(City *cityDes, const City *citySrc);
void copyCity(City *cityDes, City *citySrc);
int hasAirport(City *city);
void calcDistance(const City *city1, const City *city2, int* distance);
int calPass(City *city1, City *city2);
void deallocate(City *city);
void setAirport(City *city, const char *airport);
void readAirport(City *city, char *line);
void printPassAndDistance(City *city1, City *city2, int numPass, int distance);


#endif
