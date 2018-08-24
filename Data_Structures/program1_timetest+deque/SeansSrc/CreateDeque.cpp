// Author: Sean Davis 
// Created on September 22, 2015, 2:12 PM
#include <deque>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <climits>
using namespace std;

typedef struct
{
  char op;
  unsigned short value;
  int index;
} Operation;


int fillDeque(deque <unsigned short> &q, int maximum, Operation operations[])
{
  int num, value, opCount = 0;
  
  while(q.size() < maximum)
  {
    num = rand() % 100;
    
    if(num < 35)
    {
      operations[opCount].op = 'F';
      operations[opCount].value = rand() % USHRT_MAX;
      q.push_front(operations[opCount].value);
    }  // if num < 35
    else
      if(num < 70)
      {
        operations[opCount].op = 'B';
        operations[opCount].value = rand()  % USHRT_MAX;
        q.push_back(operations[opCount].value);
      }  // if num < 70  
      else  // num >= 70
        if(!q.empty())
        {
          if(num < 80)
          {
            operations[opCount].op = 'A';
            operations[opCount].value = rand() % USHRT_MAX;
            operations[opCount].index = rand() % q.size();
            q[operations[opCount].index] = operations[opCount].value;
          }  // if num < 80
          else
            if(num < 90)
            {
              operations[opCount].op = 'a';
              operations[opCount].index = rand() % q.size();
              operations[opCount].value = q[operations[opCount].index];
            }  // if num < 90  
            else
              if(num < 95)
              {
                operations[opCount].op = 'f';
                q.pop_front();
              }  // if num < 95
              else  // num < 100
                {
                  operations[opCount].op = 'b';
                  q.pop_back();
                }  // if num < 95
          }  // if q is not empty
        else // Trying to access an empty queue so ignore
          opCount--;
          
    opCount++;
  }  // deque not full.
  
  return opCount;
} // fillDeque()

int emptyDeque(deque <unsigned short> &q, int final, Operation operations[], int opCount)
{
  int num, value;
  
  while(q.size() > final)
  {
    num = rand() % 100;
    
    if(num < 5)
    {
      operations[opCount].op = 'F';
      operations[opCount].value = rand() % USHRT_MAX;
      q.push_front(operations[opCount].value);
    }  // if num < 5
    else
      if(num < 10)
      {
        operations[opCount].op = 'B';
        operations[opCount].value = rand()  % USHRT_MAX;
        q.push_back(operations[opCount].value);
      }  // if num < 10  
      else  // num >= 10
        if(num < 20)
        {
          operations[opCount].op = 'A';
          operations[opCount].value = rand()  % USHRT_MAX;
          operations[opCount].index = rand() % q.size();
          q[operations[opCount].index] = operations[opCount].value;
        }  // if num < 20
        else
          if(num < 30)
          {
            operations[opCount].op = 'a';
            operations[opCount].index = rand() % q.size();
            operations[opCount].value = q[operations[opCount].index];
          }  // if num < 30  
          else
            if(num < 65)
            {
              operations[opCount].op = 'f';
              q.pop_front();
            }  // if num < 65
            else  // num < 100
            {
              operations[opCount].op = 'b';
              q.pop_back();
            }  // if num < 95
                     
    opCount++;
  }  // deque.size() > final.
  
  return opCount;
} // emptyDeque()

void writeFile(const char filename[], Operation operations[], int opCount)
{
  ofstream outf(filename);
  outf << opCount << endl;
  
  for(int i = 0; i < opCount; i++)
  {
    outf << operations[i].op;
    
    if(operations[i].op != 'b' && operations[i].op != 'f')
      outf << operations[i].value;
    
    if(operations[i].op == 'a' || operations[i].op == 'A')
      outf << ' ' << operations[i].index;
    
    outf << endl;
  }  // for each operation
  
  outf.close();
} // writeFile()

int main(int argc, char** argv) 
{
  char filename[80];
  int maximum, final, seed, opCount = 0;
  Operation *operations = new Operation[30000000];
  cout << "Maximum number of elements (10 - 5000000): ";
  cin >> maximum;
  cout << "Seed (any integer): ";
  cin >> seed;
  srand(seed);
  final = maximum / 2 - (rand() % (maximum / 4));
  deque<unsigned short> q;
  opCount = fillDeque(q, maximum, operations);
  opCount = emptyDeque(q, final, operations, opCount);
  sprintf(filename, "deque-%d-%d-%d.txt", maximum, final, seed);
  writeFile(filename, operations, opCount);
  return 0;
} // main())

