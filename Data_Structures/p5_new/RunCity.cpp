#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iomanip>
#include "StreetSelecter.h"
#include "RunCity.h"
#include "CPUTimer.h"

using namespace std;



Block* readFile(ifstream &inf, int &citySize, int &blockCount, Trip *trips,
  int numTrips)
{
  Block *blocks;
  inf >> citySize;
  blocks = new Block[citySize * citySize];
  inf.ignore(1);
  blockCount = 0;
  inf >> blocks[blockCount].startCoordinates[NS]
    >> blocks[blockCount].startCoordinates[EW];

  while(blocks[blockCount].startCoordinates[NS] >= 0)
  {
    for(int i = 0; i < 4; i++)
      inf >> blocks[blockCount].startRoads[i];
    inf >> blocks[blockCount].endCoordinates[NS]
      >> blocks[blockCount].endCoordinates[EW];
    for(int i = 0; i < 4; i++)
      inf >> blocks[blockCount].endRoads[i];
    inf >> blocks[blockCount++].length;
    inf >> blocks[blockCount].startCoordinates[NS]
      >> blocks[blockCount].startCoordinates[EW];
  } // while more blocks

  for(int i = 0; i < numTrips; i++)
    inf >> trips[i].startCoordinates[NS] >> trips[i].startCoordinates[EW]
      >> trips[i].endCoordinates[NS] >> trips[i].endCoordinates[EW];

  return blocks;
} // readFile()

void writeSolution(char *filename, Trip widenedBlocks[100], int numWidened)
{
  int len = 0;
  char fn[80];
  strcpy(fn, filename);
  strtok(fn, ".");
  strcat(fn, ".ans");
  ofstream outf(fn);
  for(int i = 0; i < numWidened; i++)
  {
    outf << widenedBlocks[i].startCoordinates[NS] << ' '
      << widenedBlocks[i].startCoordinates[EW] << ' '
      << widenedBlocks[i].endCoordinates[NS] << ' '
      << widenedBlocks[i].endCoordinates[EW] << endl;
    len += abs(widenedBlocks[i].startCoordinates[0] - widenedBlocks[i].endCoordinates[0])
      + abs(widenedBlocks[i].startCoordinates[1] - widenedBlocks[i].endCoordinates[1]);
  }
  outf.close();
  if(len > 100)
    cout << "Total blocks exceeded 100: " << len << endl;
}

int main(int argc, char* argv[])
{
  Block *blocks;
  StreetSelecter *streetSelecter;
  int citySize, blockCount, numTrips, numWidened;
  Trip *trips, widenedBlocks[100];
  char filename[80], command[80];
  CPUTimer ct;

  if(argc != 2)
  {
    cout << "usage: RunCity.out CityFilename\n";
    return 1;
  }

  ifstream inf(argv[1]);
  strcpy(filename, argv[1]);
  strtok(filename, "-");
  strtok(NULL, "-");
  numTrips = atoi(strtok(NULL, "-"));
  trips = new Trip[numTrips];

  blocks = readFile(inf, citySize, blockCount, trips, numTrips);
  ct.reset();

  streetSelecter = new StreetSelecter(blocks, blockCount, citySize, numTrips);

  delete [] blocks;

  streetSelecter->select(trips, numTrips, widenedBlocks, numWidened); // fills widenedBlocks
  
  cout << "CPU Time: " << ct.cur_CPUTime() << endl;
  writeSolution(argv[1], widenedBlocks, numWidened);  // writes widenedBlocks to filename.ans
  sprintf(command,"./RunTrips.out %s", argv[1]);
  system(command);  // runs trips of file using original and widened streets
  return 0;
}

