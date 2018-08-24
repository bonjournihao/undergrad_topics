// Author: Sean Davis 
// Created on September 22, 2015, 9:48 AM

#include <fstream>
#include <iostream>
#include "deque.h"
#include "mynew.h"
#include "CPUTimer.h"
using namespace std;

extern int currentRAM;

typedef struct
{
  char op;
  unsigned short value;
  int index;
} Operation;


Operation* readFile(const char *filename, int *opCount)
{
  initializeNew();
  ifstream inf(filename);
  inf >> *opCount;
  inf.ignore(10,'\n');
  Operation *operations = new Operation[*opCount];
  
  for(int i = 0; i < *opCount; i++)
  {
    inf >> operations[i].op;
    
    if(operations[i].op != 'b' && operations[i].op != 'f')
      inf >> operations[i].value;
    
    if(operations[i].op == 'A' || operations[i].op == 'a')
      inf >> operations[i].index;
    
    inf.ignore(10, '\n');
  } // for each operation
  return operations;
} // readFile()


int main(int argc, char** argv) 
{
  int opCount, index, retval;
  Operation *operations = readFile(argv[1], &opCount);
  currentRAM = 0;
  CPUTimer ct;
  Deque <unsigned short> *deque = new Deque<unsigned short>();

  for(int i = 0; i < opCount; i++)
  {
    switch(operations[i].op)
    {
      case 'F' : deque->push_front(operations[i].value); break;
      case 'B' : deque->push_back(operations[i].value); break;
      case 'f' : deque->pop_front(); break;
      case 'b' : deque->pop_back(); break;
      case 'A' : (*deque)[operations[i].index] = operations[i].value; break;
      case 'a' : 
        index = operations[i].index;
        retval = (*((const Deque<unsigned short> *)deque))[index];
      
        if(retval != operations[i].value)
          cout << "Error: Operation #" << i << " index #" << index 
            << " returned " << retval << " instead of " << operations[i].value
            << endl;

        break;
      default: cout << "Should never get here\n";
    }  // switch
    
   // deque->print();
  }  // for
  cout << "CPU Time: " << ct.cur_CPUTime() << " RAM: " << currentRAM << endl;
  return 0;
} // main())

