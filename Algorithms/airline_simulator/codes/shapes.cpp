#include <iostream>
#include <iomanip>
using namespace std;

class Shape
{
  virtual void write(ostream &os)const = 0;
  friend ostream& operator << (ostream &os, const Shape &rhs)
  {
    rhs.write(os);
    return os;
  } // operator <<
}; // class shape

class IntersectingLines:public Shape
{
  void write(ostream &os)const
  {
  	os << setw(5) << right << "/" << "\\" << endl;
  	os << setw(4) << right << "/" << setw(3) << right << "\\" << endl;
  	os << setw(3) << right << "/" << setw(5) << right << "\\" << endl;
  	os << setw(2) << right << "/" << setw(7) << right << "\\" << endl;
  	os << "/" << setw(9) << right << "\\" << endl;
  } // write()
}; // class intersection line

class BaseLine:public Shape
{
  void write(ostream &os)const
  {
  	for (int i = 0; i < 10; i++)
    {
  	  os << "-";
    } // for
    os << endl;
  } // write()
}; // class baseLine

class ParallelLines:public Shape
{
  void write(ostream &os)const
  {
  	for (int i = 0; i < 3; i++)
  	{
  	  os << "|" << setw(9) << right << "|" << endl;
  	} // for
  } // write()
}; // class Parallel

class BlankLines:public Shape
{
  void write(ostream &os)const
  {
    os << setw(10) << endl;
    os << setw(10) << endl;
  } // write()
}; // class blankLine

class Circle:public Shape
{
  void write(ostream &os)const
  {
  	os << setw(5) << right << "*" << setw(5) << endl;
    os << setw(3) << right << "*" << setw(4) << right << "*" << setw(3) << endl;
    os << setw(4) << right << "*" << setw(2) << right << "*" << setw(4) << endl;
  } // write(0)
}; // class Circle


int main()
{
  IntersectingLines a;
  BaseLine b;
  ParallelLines c;
  BlankLines d;
  Circle e;
  Shape *shapes[9];

  shapes[0] = &a;
  shapes[1] = &b;
  shapes[2] = &c;
  shapes[3] = &b;
  shapes[4] = &d;
  shapes[5] = &e;
  shapes[6] = &a;
  shapes[7] = &b;
  shapes[8] = &a;

  for (int i = 0; i < 9; i++)
  {
  	cout << *(shapes[i]);
  } // for
  return 0;
} // main()