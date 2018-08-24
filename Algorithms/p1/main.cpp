#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include <string.h>

void run(Vector *vector)
{
  char airport1[85];
  char airport2[85];

  while (1)
  {
    printf("\nPlease enter two airport abbreviations (XXX XXX = done): ");
    scanf(" %s %s", airport1, airport2); //take user input

    if (strcmp(airport1, "XXX") == 0 && strcmp(airport2, "XXX") == 0)
    {
      break;
    }//if user inputs XXX XXX, exit

    int index1 = findAirport(vector, airport1); //get airport index
    int index2 = findAirport(vector, airport2); //get airport index

    if (index1 >= 0 && index2 >= 0) //if exist, prints out information
    {
      calcDistance(vector, index1, index2);
    } //if

    if (index1 < 0) 
    { 
      printf("%s is not a valid airport.\n", airport1);
    } //if not valid airport, prints error message
  
    if (index2 < 0) 
    {
      printf("%s is not a valid airport.\n", airport2);
    } //if not valid airport, prints error message
  }//while
}//interact with user

int main()
{
  Vector vector;
  storeCities(&vector); // store population info of cities
  readAirports(&vector); // store airport info of cities
  cleanCities(&vector); //clean unused Cities
  //printAll(&vector);
  run(&vector); // interact with user
  deallocate(&vector); //deallocate Cities and Vector
  return 0;
}//main function
