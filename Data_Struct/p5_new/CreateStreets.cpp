#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

enum {NORTH, EAST, SOUTH, WEST};
enum {NS, EW};

int readNames(char streetNames[550][25])
{
  int count = 0;
  ifstream inf("streetNames.txt");
  strcpy(streetNames[count++], "None");
  while(inf.getline(streetNames[count++], 25));
  return count;
}  // readNames()

void addEastWestStreet(short ***grid, int lastStarts[2], int lastEnds[2],
  int streetNum, int citySize)
{
  int starts[2], ends[2], searchCount, j, k, start, end;
  bool foundEmptyIntersection;

  for(searchCount = 0; searchCount < 5; searchCount++)
  {
    starts[EW] = lastStarts[EW] - rand() % 5 - 2;
    if(starts[EW] < 0)
      starts[EW] = 0;

    ends[EW] = lastStarts[EW] + rand() % 5 + 2;
    if(ends[EW] > citySize  - 1)
      ends[EW] = citySize  - 1;

    if(lastStarts[NS] !=  lastEnds[NS])
      starts[NS] = ends[NS] = lastStarts[NS] + rand() % abs(lastStarts[NS] - lastEnds[NS]);
    else
      starts[NS] = ends[NS] = lastStarts[NS];
    if(grid[starts[NS]][lastStarts[EW]][EAST] == 0 ||
      grid[starts[NS]][lastStarts[EW]][WEST] == 0 )
      break;
  } // for searchCount

  if(searchCount == 5) // failed to find an opening on last street
  {
    foundEmptyIntersection = false;
    for(j = 0; j < citySize / 2 && !foundEmptyIntersection; j++)
    {
      start = citySize / 2 - j;
      end = citySize / 2 + j;
      for(k = start; k <= end && !foundEmptyIntersection; k++)
      {
        if((grid[start][k][EAST] == 0 || grid[start][k][WEST] == 0)
          && (grid[start][k][NORTH] != 0 || grid[start][k][SOUTH] != 0))
        {
          foundEmptyIntersection = true;
          lastStarts[NS] = start;
          lastStarts[EW] = k;
        }
        else
          if((grid[end][k][EAST] == 0 || grid[end][k][WEST] == 0)
            && (grid[end][k][NORTH] != 0 || grid[end][k][SOUTH] != 0))
          {
            foundEmptyIntersection = true;
            lastStarts[NS] = end;
            lastStarts[EW] = k;
          }
          else
            if ((grid[k][start][EAST] == 0 || grid[k][start][WEST] == 0)
              && (grid[k][start][NORTH] != 0 || grid[k][start][SOUTH] != 0))
            {
              foundEmptyIntersection = true;
              lastStarts[NS] = k;
              lastStarts[EW] = start;
            }
            else
              if ((grid[k][end][EAST] == 0 || grid[k][end][WEST] == 0)
                && (grid[k][end][NORTH] != 0 || grid[k][end][SOUTH] != 0))
              {
                foundEmptyIntersection = true;
                lastStarts[NS] = k;
                lastStarts[EW] = end;
              }
        } // for k
    } // for j
    starts[EW] = lastStarts[EW] - rand() % 5 - 2;
    if(starts[EW] < 0)
      starts[EW] = 0;
    ends[EW] = lastStarts[EW] + rand() % 5 + 2;
    if(ends[EW] > citySize  - 1)
      ends[EW] = citySize  - 1;
    starts[NS] = ends[NS] = lastStarts[NS];
  } // if searchCount == 5

  for(j = lastStarts[EW] - 1; j >= starts[EW]; j--)
    if(grid[starts[NS]][j][EAST] == 0)
    {
      grid[starts[NS]][j][EAST] = streetNum;
      grid[starts[NS]][j + 1][WEST] = streetNum;
    }
    else
      break;

  starts[EW] = j + 1;
  for(j = lastStarts[EW] + 1; j <= ends[EW]; j++)
    if(grid[starts[NS]][j][WEST] == 0)
    {
      grid[starts[NS]][j][WEST] = streetNum;
      grid[starts[NS]][j - 1][EAST] = streetNum;
    }
    else
      break;

  lastStarts[NS] = lastEnds[NS] = starts[NS];
  lastStarts[EW] = starts[EW];
  lastEnds[EW] = j - 1;
} // addEastWestStreet()


void addNorthSouthStreet(short ***grid, int lastStarts[2], int lastEnds[2],
  int streetNum, int citySize)
{
  int starts[2], ends[2], searchCount, j, k, start, end;
  bool foundEmptyIntersection;

  for(searchCount = 0; searchCount < 5; searchCount++)
  {
    starts[NS] = lastStarts[NS] - rand() % 5 - 2;
    if(starts[NS] < 0)
      starts[NS] = 0;

    ends[NS] = lastStarts[NS] + rand() % 5 + 2;
    if(ends[NS] > citySize  - 1)
      ends[NS] = citySize  - 1;

    if(lastStarts[EW] != lastEnds[EW])
      starts[EW] = ends[EW] = lastStarts[EW] + rand() % abs(lastStarts[EW] - lastEnds[EW]);
    else
      starts[EW] = ends[EW] = lastStarts[EW];
    if(grid[lastStarts[NS]][starts[EW]][NORTH] == 0 ||
      grid[lastStarts[NS]][starts[EW]][SOUTH] == 0 )
      break;
  } // for searchCount

  if(searchCount == 5) // failed to find an opening on last street
  {
    foundEmptyIntersection = false;
    for(j = 0; j < citySize / 2 && !foundEmptyIntersection; j++)
    {
      start = citySize / 2 - j;
      end = citySize / 2 + j;
      for(k = start; k <= end && !foundEmptyIntersection; k++)
      {
        if((grid[start][k][NORTH] == 0 || grid[start][k][SOUTH] == 0)
          && (grid[start][k][EAST] != 0 || grid[start][k][WEST] != 0))
        {
          foundEmptyIntersection = true;
          lastStarts[NS] = start;
          lastStarts[EW] = k;
        }
        else
          if((grid[end][k][NORTH] == 0 || grid[end][k][SOUTH] == 0)
            && (grid[end][k][EAST] != 0 || grid[end][k][WEST] != 0))
          {
            foundEmptyIntersection = true;
            lastStarts[NS] = end;
            lastStarts[EW] = k;
          }
          else
            if ((grid[k][start][NORTH] == 0 || grid[k][start][SOUTH] == 0)
              && (grid[k][start][EAST] != 0 || grid[k][start][WEST] != 0))
            {
              foundEmptyIntersection = true;
              lastStarts[NS] = k;
              lastStarts[EW] = start;
            }
            else
              if ((grid[k][end][NORTH] == 0 || grid[k][end][SOUTH] == 0)
                && (grid[k][end][EAST] != 0 || grid[k][end][WEST] != 0))
              {
                foundEmptyIntersection = true;
                lastStarts[NS] = k;
                lastStarts[EW] = end;
              }

        } // for k
    } // for j
    starts[NS] = lastStarts[NS] - rand() % 5 - 2;
    if(starts[NS] < 0)
      starts[NS] = 0;
    ends[NS] = lastStarts[NS] + rand() % 5 + 2;
    if(ends[NS] > citySize  - 1)
      ends[NS] = citySize  - 1;
    starts[EW] = ends[EW] = lastStarts[EW];
  } // if searchCount == 5

  for(j = lastStarts[NS] - 1; j >= starts[NS]; j--)
    if(grid[j][starts[EW]][SOUTH] == 0)
    {
      grid[j][starts[EW]][SOUTH] = streetNum;
      grid[j + 1][starts[EW]][NORTH] = streetNum;
    }
    else
      break;

  starts[NS] = j + 1;
  for(j = lastStarts[NS] + 1; j <= ends[NS]; j++)
    if(grid[j][starts[EW]][NORTH] == 0)
    {
      grid[j][starts[EW]][NORTH] = streetNum;
      grid[j - 1][starts[EW]][SOUTH] = streetNum;
    }
    else
      break;

  lastStarts[EW] = lastEnds[EW] = starts[EW];
  lastStarts[NS] = starts[NS];
  lastEnds[NS] = j - 1;
} // addNorthSouthStreet()


void fillGrid(int streetCount, short ***grid, int citySize)
{
  int lastStarts[2], lastEnds[2];
  lastStarts[EW] = rand() % citySize;
  lastEnds[EW] = lastStarts[EW] + rand() % 10 + 1;
  if(lastEnds[EW] > citySize  - 1)
    lastEnds[EW] = citySize  - 1;
  lastStarts[NS] = lastEnds[NS] = rand() % citySize;

  for(int i = 1; i <= streetCount; i++)
  {
    if(lastStarts[EW] == lastEnds[EW])
    {  // last street ran north to south so this one runs west to east.
      addEastWestStreet(grid, lastStarts, lastEnds, i, citySize);
    }
    else // last street ran east to west so this one runs north to south
    {
      addNorthSouthStreet(grid, lastStarts, lastEnds, i, citySize);
    }
  } // for each street
} // fillGrid()

void writeBlocks(short ***grid, ofstream &outf, int citySize,
  vector <int> &streetNums)
{
  int i, j, k, direction;


  for(i = 0; i < citySize; i++)
    for(j = 0; j < citySize; )
      if(grid[i][j][EAST] != 0)
      {
        outf << i << ' ' << j << ' ';
        for(direction = NORTH; direction <= WEST; direction++)
          outf << streetNums[grid[i][j][direction]] << ' ';
        for(k = j + 1; k < citySize && grid[i][j][EAST] == grid[i][k][WEST]
          && grid[i][k][NORTH] == 0 && grid[i][k][SOUTH] == 0; k++);
        if(grid[i][j][EAST] != grid[i][k][WEST])
          k--;
        outf << i << ' ' << k << ' ';
        for(direction = NORTH; direction <= WEST; direction++)
          outf << streetNums[grid[i][k][direction]] << ' ';
        outf << k - j << endl;
        j = k;
      }
      else // not an intersection
        j++;

  for(j = 0; j < citySize; j++)
    for(i = 0; i < citySize; )
      if(grid[i][j][SOUTH] != 0)
      {
        outf << i << ' ' << j << ' ';
        for(direction = NORTH; direction <= WEST; direction++)
          outf << streetNums[grid[i][j][direction]] << ' ';
        for(k = i + 1; k < citySize && grid[i][j][SOUTH] == grid[k][j][NORTH]
          && grid[k][j][EAST] == 0 && grid[k][j][WEST] == 0; k++);
        if(grid[i][j][SOUTH] != grid[k][j][NORTH])
          k--;
        outf << k << ' ' << j << ' ';
        for(direction = NORTH; direction <= WEST; direction++)
          outf << streetNums[grid[k][j][direction]] << ' ';
        outf << k - i << endl;
        i = k;
      }
      else // not an intersection
        i++;

  outf << "-1 -1\n";

}  // writeBlocks()

void writeTrips(short ***grid, ofstream &outf, int citySize,
  vector <int> &streetNums, int numTrips)
{
  int startNS, startEW, endNS, endEW, total;
  for(int i = 0; i < numTrips; i++)
  {
    do
    {
      startNS = rand() % citySize;
      startEW = rand() % citySize;
    } while(grid[startNS][startEW][EAST] + grid[startNS][startEW][EAST] == 0
      || grid[startNS][startEW][NORTH] + grid[startNS][startEW][SOUTH] == 0);

     do
    {
      endNS = rand() % citySize;
      endEW = rand() % citySize;
      total = 0;
      for(int j = 0; j < 4; j++)
        total += grid[endNS][endEW][j];
    } while((startNS == endNS && startEW == endEW)
      ||  grid[endNS][endEW][EAST] + grid[endNS][endEW][EAST] == 0
      || grid[endNS][endEW][NORTH] + grid[endNS][endEW][SOUTH] == 0);

    outf << startNS << ' ' << startEW << ' ' << endNS << ' ' << endEW << endl;

  } // for t
} // writeTrips()

int main(int argc, char* argv[])
{
  int seed, streetCount, citySize, numTrips;
  short ***grid;

  char filename[80]; //, streetNames[550][25];
 // streetNameCount = readNames(streetNames);

  cout << "City size: ";
  cin >> citySize;
  cout << "Number of streets: ";
  cin >> streetCount;
  cout << "Number of trips: ";
  cin >> numTrips;
  cout << "Seed: ";
  cin >> seed;
  srand(seed);
  sprintf(filename, "City%d-%d-%d-%d.txt", citySize, streetCount, numTrips, seed);

  vector <int> streetNums;
  for(int i = 0; i <= streetCount; i++)
    streetNums.push_back(i);

  random_shuffle(streetNums.begin() + 1, streetNums.end());
  grid = new short**[citySize];
  for(int i = 0; i < citySize; i++)
  {
    grid[i] = new short*[citySize];
    for(int j= 0; j < citySize; j++)
    {
      grid[i][j] = new short[4];
      for(int k = 0; k < 4; k++)
        grid[i][j][k] = 0;
    } // for j
  } // for i

  fillGrid(streetCount, grid, citySize);
  ofstream outf(filename);
  outf << citySize << endl;
  writeBlocks(grid, outf, citySize, streetNums);
  writeTrips(grid, outf, citySize, streetNums, numTrips);

  for(int i = 0; i < citySize; i++)
  {
    for(int j= 0; j < citySize; j++)
      delete [] grid[i][j];
    delete [] grid[i];
  } // for i
  delete [] grid;
  return 0;
} // main()

