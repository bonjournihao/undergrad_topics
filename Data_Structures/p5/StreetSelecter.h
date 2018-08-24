#ifndef StreetSelecterH
#define StreetSelecterH

#include "RunCity.h"
class vertex;
class shortTrip;
class graph;

class StreetSelecter
{
public:
	graph *g;
	StreetSelecter(Block *blocks, int blockCount, int citySize, int numTrips);
	void select(Trip *trips, int numTrips, Trip widenedBlocks[100], int &numWidened); 
    // fills widenedBlocks and sets numWidened to the number of entries
}; // StreetSelecter class
#endif

class graph
{
public:
	vertex *vertexList;
	int vertexListCount;
	graph():vertexList(NULL),vertexListCount(0){}

	void createAdjList(Block *blocks, int blockCount, int citySize);
	//find the shortest path from source to dest; use dijkstra's algorithm
	//!!!keep track of the short trips(use a map: count, length);
	bool hasUnknown();
	vertex* smallestUnknown();
	vertex *findVertex(vertex *rhs);
	int distBetween(vertex *v1, vertex *v2);
	void findPath(vertex *source, vertex *dest, shortTrip map[10005], int &shortTripCount);
	void findShortTrips(vertex *dest, shortTrip map[10005], int &shortTripCount);
	void widen(shortTrip map[10005], int &shortTripCount, Trip widenedBlocks[100], int &numWidened);
	void reset();
friend class StreetSelecter;
};

class vertex
{
public:
	short x;//coordinate northToSouth
	short y;//coordinate eastToWest

	vertex *adjList[4];
	int adjListCount;

	bool known;
	int dist;
	vertex *prev;

	vertex():x(0),y(0),adjListCount(0),known(false),dist(1000000),prev(NULL)
	{
		for (int i = 0; i<4;i++)
			adjList[i] = NULL;
	}
	vertex(short a, short b):x(a),y(b),adjListCount(0),known(false),dist(1000000),prev(NULL){}

	inline bool operator==(vertex &rhs);
	inline void operator=(vertex &rhs);

friend class graph;
};

class shortTrip
{
public:
	Trip trip;
	short length;
	int count;
	bool isEqual(shortTrip &rhs);
	//void copy(shortTrip &rhs);
};
