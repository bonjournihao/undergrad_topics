#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <cctype>
#include <iomanip>
// #define DOS
#ifdef DOS
#include <conio.h>
#else
  #include <termio.h>

void set_keypress(void) {
  struct termios new_settings, stored_settings;
  tcgetattr(0,&stored_settings);
  new_settings = stored_settings;
  new_settings.c_lflag &= (~ICANON);
  new_settings.c_cc[VTIME] = 0;
  tcgetattr(0,&stored_settings);
  new_settings.c_cc[VMIN] = 1;
  tcsetattr(0,TCSANOW,&new_settings);
}

#endif

using namespace std;
enum {SOUTH, EAST};

short*** readFile(const char *filename, int &citySize)
{
  short ***grid;
  int startNS, startEW, endNS, endEW, dummy;
  ifstream inf(filename);
  inf >> citySize;
  grid = new short**[citySize];
  for(int i = 0; i < citySize; i++)
  {
    grid[i] = new short*[citySize];
    for(int j = 0; j < citySize; j++)
    {
      grid[i][j] = new short[2];
      for(int k = 0; k < 2; k++)
        grid[i][j][k] = 0;
    } // for j
  } // for i

  inf.ignore(1);
  inf >> startNS >> startEW;
  while(startNS >= 0)
  {
    for(int i = 0; i < 4; i++)
      inf >> dummy;
    inf >> endNS >> endEW;
    for(int i = 0; i < 5; i++)
      inf >> dummy;
    if(startNS == endNS)
      for(int i = startEW; i < endEW; i++)
        grid[startNS][i][EAST] = 1;
    else
      for(int i = startNS; i < endNS; i++)
        grid[i][startEW][SOUTH] = 1;
    inf >> startNS >> startEW;
  }

  return grid;
} // readFile()


void displayGridPart(short ***grid, int citySize, int originNS, int originEW)
{
  cout << '\n' << setw(3) << originEW << ' ';
  for(int i = 0; i < 35; i++)
    cout << (originEW + i) % 10 << ' ';
  cout << endl;

  for(int i = originNS; i < originNS + 10; i++)
  {
    cout << setw(3) << i << ' ';
    for(int j = originEW; j < originEW + 35; j++)
    {
      if(grid[i][j][EAST])
      {
        if(grid[i][j][SOUTH])
            cout << "+-";
        else
            cout << "--";
      }
      else
      {
        if(grid[i][j][SOUTH])
          cout << "| ";
        else
          cout << "  ";
      }
    } // for j
    cout << "\n    ";

    for(int j = originEW; j < originEW + 35; j++)
    {
      if(grid[i][j][SOUTH])
        cout << "| ";
      else
        cout << "  ";
    } // for j
    cout << endl;

  } // for i
  cout << "h=left, j=down, k=up, l=right, caps = paging, x = exit  ";
} // displayGridPart

void displayGrid(short ***grid, int citySize)
{
  int originNS = 0, originEW = 0, key;
#ifndef DOS
  set_keypress();
#endif

  do
  {
    displayGridPart(grid, citySize, originNS, originEW);
#ifdef DOS
    key = getch();
#else
    key = getchar();
#endif
    switch(key)
    {
      case 'h' : originEW = max(0, originEW - 1); break;
      case 'H' : originEW = max(0, originEW - 35); break;
      case 'j' : originNS = min(citySize - 10, originNS + 1); break;
      case 'J' : originNS = min(citySize - 10, originNS + 10); break;
      case 'k' : originNS = max(0, originNS - 1); break;
      case 'K' : originNS = max(0, originNS - 10); break;
      case 'l' : originEW = min(citySize - 35, originEW + 1); break;
      case 'L' : originEW = min(citySize - 35, originEW + 35); break;
    } // switch
  } while(key != 'x');
}  // displayGrid


int main(int argc, char* argv[])
{
  short ***grid;
  int citySize;

  if(argc != 2)
  {
    cout << "usage: ShowCity CityFilename\n";
    return 1;
  }

  grid = readFile(argv[1], citySize);
  displayGrid(grid, citySize);
  for(int i = 0; i < citySize; i++)
  {
    for(int j = 0; j < citySize; j++)
      delete [] grid[i][j];

    delete []  grid[i];
  }

  delete [] grid;
  return 0;
}  // main()
