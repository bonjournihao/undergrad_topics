#include "LinkedList.h"
#include "StackAr.h"
#include "CursorList.h"
#include "QueueAr.h"
#include "StackLi.h"
#include "vector.h"
#include "SkipList.h"
#include "CPUTimer.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;

vector <CursorNode <int> > cursorSpace(500000);

void getFilename (char *filename)
{
  cout << "Filename >> ";
  cin >> filename;
  return;
}

int getChoice()
{
  char choice[2];
  bool valid = false;

  while (!valid)
  {
    cout << "ADT Menu" << endl;
    cout << "0.Quit" << endl;
    cout << "1.LinkedList" << endl;
    cout << "2.CursorList" << endl;
    cout << "3.StackAr" << endl;
    cout << "4.StackLi" << endl;
    cout << "5.QueueAr" << endl;
    cout << "6. SkipList" << endl;
    cout << "Your choice >> ";
    cin >> choice;

    if (! (atoi(choice) >= 0 && atoi(choice) <= 6))
    {
      cout << "Your choice is not between 0 and 6." << endl;
      cout << "Please try again." << endl;
      continue;
    }

    valid = true;
  }
  return atoi(choice);
}// get the numeric choice from user

void RunList(char *filename)
{
  List<int> list;
  ListItr<int> listItr;

  ifstream inf(filename);
  char str[150];
  char c;
  int integer;

  inf.getline(str, 256);

  while (inf.get(c))
  {
  	if(c == 'i' || c == 'd')
  	{
  	  inf >> integer;
  	  if (c == 'i')
  	  {
  	  	list.insert(integer, list.zeroth());
  	  }
  	  else
  	  {
  	  	list.remove(integer);
  	  }
  	}
  }

  return;
}

void RunCursorList(char *filename)
{
  CursorList<int> list(cursorSpace);
  CursorListItr<int> listItr(list);

  ifstream inf(filename);
  char str[150];
  char c;
  int integer;

  inf.getline(str, 256);

  while (inf.get(c))
  {
  	if(c == 'i' || c == 'd')
  	{
  	  inf >> integer;
  	  if (c == 'i')
  	  {
  	  	list.insert(integer, list.zeroth());
  	  }
  	  else
  	  {
  	  	list.remove(integer);
  	  }
  	}
  }

  return;
}

void RunSkipList(char *filename)
{
  SkipList<int> list(0, 500000);
  ifstream inf(filename);
  char str[150];
  char c;
  int integer;

  inf.getline(str, 256);

  while (inf.get(c))
  {
  	if(c == 'i' || c == 'd')
  	{
  	  inf >> integer;
  	  if (c == 'i')
  	  {
  	  	list.insert(integer);
  	  }
  	  else
  	  {
  	  	list.deleteNode(integer);
  	  }
  	}
  }

  return;
}

void RunStackAr(char *filename)
{
  StackAr<int> array(500000);
  ifstream inf(filename);
  char str[150];
  char c;
  int integer;

  inf.getline(str, 256);

  while (inf.get(c))
  {
  	if(c == 'i' || c == 'd')
  	{
  	  inf >> integer;
  	  if (c == 'i')
  	  {
  	  	array.push(integer);
  	  }
  	  else
  	  {
  	  	array.pop();
  	  }
  	}
  }

  return;
}

void RunStackLi(char *filename)
{
  StackLi<int> list;
  ifstream inf(filename);
  char str[150];
  char c;
  int integer;

  inf.getline(str, 256);

  while (inf.get(c))
  {
  	if(c == 'i' || c == 'd')
  	{
  	  inf >> integer;
  	  if (c == 'i')
  	  {
  	  	list.push(integer);
  	  }
  	  else
  	  {
  	  	list.pop();
  	  }
  	}
  }

  return;
}

void RunQueueAr(char *filename)
{
  Queue<int> array(500000);
  ifstream inf(filename);
  char str[150];
  char c;
  int integer;

  inf.getline(str, 256);

  while (inf.get(c))
  {
  	if(c == 'i' || c == 'd')
  	{
  	  inf >> integer;
  	  if (c == 'i')
  	  {
  	  	array.enqueue(integer);
  	  }
  	  else
  	  {
  	  	array.dequeue();
  	  }
  	}
  }

  return;
}


int main()
{
  CPUTimer ct;
  int choice;
  char filename[10];

  getFilename(filename);
  do
  {
    choice = getChoice();

    ct.reset();
    switch (choice)
    {
      case 0: break;
      case 1: RunList(filename); break;
      case 2: RunCursorList(filename); break;
      case 3: RunStackAr(filename); break;
      case 4: RunStackLi(filename); break;
      case 5: RunQueueAr(filename); break;
      case 6: RunSkipList(filename); break;
    }
    cout << "CPU time: " << ct.cur_CPUTime() << endl;
  } while(choice > 0);
  return 0;
}
