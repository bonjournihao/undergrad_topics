#ifndef VECTOR_H
#define VECTOR_H
#include "city.h"

typedef struct vector
{
  City* cityArray;
  int size;
  int count;
} Vector; //Vector struct

void initialize(Vector *vector, int size);
void resize(Vector *vector);
void deallocate(Vector *vector);
void storeCities(Vector *vector);
void readAirports(Vector *vector);
void cleanCities(Vector *vector);
int findAirport(const Vector *vector, const char* airport);
void calcDistance(const Vector *vector, int city1Index, int city2Index);

#endif
