#include <iostream>
#include <fstream>
#include "plane.h"
using namespace std;

int main()
{
  Plane planes[10];
  int i = 0;
  ifstream ifn("planes.dat", ios::binary);
  //ifn.open("planes.dat", ios::in | ios::binary );
  while(!ifn.eof())
  {
    planes[i++].read(&ifn);
    //ifn.read((char*)(&(planes[i++])), sizeof(Plane));
    cout << planes[i - 1].name << endl;
    cout << planes[i - 1].passengers << endl;
    cout << planes[i - 1].range << endl;
  }
  return 0;
}


