#ifndef RUNCITY_H
  #define RUNCITY_H

enum {NORTH, EAST, SOUTH, WEST};
enum {NS, EW};

class Block
{
public:
  short startCoordinates[2]; // NS first, EW second
  short startRoads[4];
  short endCoordinates[2]; // NS first, EW second
  short endRoads[4];
  short length;
}; // Block class

class Trip
{
public:
  short startCoordinates[2]; // NS first, EW second
  short endCoordinates[2];  // NS first, EW second
}; // Trip class

#endif
