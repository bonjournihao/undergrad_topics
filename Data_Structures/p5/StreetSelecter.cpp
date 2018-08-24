#include <cstdlib>
#include <iostream>
using namespace std;

#include "StreetSelecter.h"


StreetSelecter::StreetSelecter(Block *blocks, int blockCount, int citySize,
  int numTrips)
{
	g = new graph();//create a graph
	g->createAdjList(blocks, blockCount, citySize);
	cout << "total count " << g->vertexListCount << endl;
	for (int i = 0 ; i < g->vertexListCount; i++)
	{
		cout << i << "st vertex:  ";
		cout << g->vertexList[i].x << "," << g->vertexList->y << endl;
	}
} // StreetSelecter()

void StreetSelecter::select(Trip *trips, int numTrips, Trip widenedBlocks[100], int &numWidened)
{
	//1. call findPath for every trip
	//2. compare the shortTrip map to decide which blocks to widen;

	shortTrip map[10005];//store all short paths' information
	int shortTripCount = 0;
	for (int i = 0; i < numTrips; i++)
	{
		vertex *tempStart = new vertex(trips[i].startCoordinates[0], trips[i].startCoordinates[1]);
		vertex *tempEnd = new vertex(trips[i].endCoordinates[0], trips[i].endCoordinates[1]);
		//find shortest path from start to end
		//store frequency and weight of trips into map
		g->findPath(tempStart, tempEnd, map, shortTripCount);
		g->reset();
	}

	g->widen(map, shortTripCount, widenedBlocks, numWidened);//choose blocks to widen
} // select()


void graph::createAdjList(Block *blocks, int blockCount, int citySize)
{
	vertexList = new vertex[citySize*citySize];//vertexNum == citySize^2

	cout << "original count:" << vertexListCount << endl;
	//insert each block into list(using 2 vertices)
	for (int i = 0; i < blockCount; i++)
	{
		//create temporary vertex to be inserted int adjList
		vertex *temp1 = new vertex(blocks[i].startCoordinates[0], blocks[i].startCoordinates[1]);
		vertex *temp2 = new vertex(blocks[i].endCoordinates[0], blocks[i].endCoordinates[1]);
		bool temp1Exist = false;
		bool temp2Exist = false;
		cout << "reading block: " << temp1->x << "," << temp1->y << "  ";
		cout << temp2->x << "," << temp2->y << endl;
		for (int j = 0; j < vertexListCount; j++)
		{
			//if existing vertex matches with temp1
			//append
			if (vertexList[j] == *temp1)
			{
				temp1Exist = true;
				int count = vertexList[j].adjListCount;
				vertexList[j].adjList[count] = temp2;
				vertexList[j].adjListCount++;

			}//end of if

			//if existing vertex matches with temp2
			//append
			if (vertexList[j] == *temp2)
			{
				temp2Exist = true;
				int count = vertexList[j].adjListCount;
				vertexList[j].adjList[count] = temp1;
				vertexList[j].adjListCount++;
			}// end of if

			//break, search is done
			if (temp1Exist == true && temp2Exist == true)
				break;
		}//end of inner for

		if (temp1Exist == false)
		{
			vertexList[vertexListCount] = *temp1;
			cout << temp1->x << "," << temp1->y << " is to be inserted" << endl;

			int count = vertexList[vertexListCount].adjListCount;
			vertexList[vertexListCount].adjList[count] = temp2;
			vertexList[vertexListCount].adjListCount++;
			vertexListCount++;
		}// end of if

		if (temp2Exist == false)
		{
			vertexList[vertexListCount] = *temp2;
			cout << temp2->x << "," << temp2->y << " is to be inserted" << endl;
			int count = vertexList[vertexListCount].adjListCount;
			vertexList[vertexListCount].adjList[count] = temp1;
			vertexList[vertexListCount].adjListCount++;
			vertexListCount++;
		}// end of if
		cout << "vertex count : " << vertexListCount << endl;
	}//end of outer for
}//createAdjList

bool graph::hasUnknown()
{
	for (int i = 0; i < vertexListCount; i++)
	{
		if (vertexList[i].known == false)
			return true;
	}
	return false;
}

vertex* graph::smallestUnknown()
{
	vertex* v = new vertex();
	for (int i = 0; i < vertexListCount; i++)
	{
		if (vertexList[i].known)
			continue;
		if (vertexList[i].dist < v->dist)
			v = &vertexList[i];
	}
	return v;
}

int graph::distBetween(vertex *v1, vertex *v2)
{
	return abs(v1->x - v2->x) + abs(v1->y - v2->y);
}

vertex *graph::findVertex(vertex *rhs)
{
	for (int i = 0; i < vertexListCount; i++)
	{
		if (vertexList[i] == *rhs)
			return &vertexList[i];
	}
	return NULL;
}

//dijkstra's
void graph::findPath(vertex *source, vertex *dest, shortTrip map[10005], int &shortTripCount)
{
	findVertex(source)->dist = 0;

	while (true)
	{
		if (dest->known == true)
		{
			findShortTrips(dest, map, shortTripCount);
			return;
		}//end of if

		if (!hasUnknown())
			break;//done with the whole list

		vertex *v = smallestUnknown();//O(N)
		cout << "v's dist to source: " << v->dist << endl;
		v->known = true;
		for (int i = 0; i < v->adjListCount; i++)
		{
			if (v->adjList[i]->known)// if an adjacent vertex is true, skip
				continue;

			int tempDist = distBetween(v, v->adjList[i]) + v->dist;
			if (v->adjList[i]->dist > tempDist)
			{
				v->adjList[i]->dist = tempDist;
				v->adjList[i]->prev = v;
			}//end of if
		}//end of for
	}//end of while
}

void graph::findShortTrips(vertex *dest, shortTrip map[10005], int &shortTripCount)
{
	bool exist = false;
	vertex *ptr = dest;
	while (ptr->prev)
	{
		//if there exists a trip or there are no trips at all, increment shortTripCount
		//otherwise, increment the the count in trips
		shortTrip tempShortTrip;//to insert
		tempShortTrip.trip.startCoordinates[0] = ptr->x;
		tempShortTrip.trip.startCoordinates[1] = ptr->y;
		tempShortTrip.trip.endCoordinates[0] = ptr->prev->x;
		tempShortTrip.trip.endCoordinates[1] = ptr->prev->y;
		for (int i = 0; i < shortTripCount; i++)
		{
			// if there is an existing shortTrip in map
			//increment trip count
			if (map[i].isEqual(tempShortTrip))
			{
				map[i].count++;
				int j = i;
				//sort the new map
				for (; j > 0; j--)
				{
					if (map[j-1].count < map[i].count)
						map[j] = map[j-1];
					else
						break;
				}
				map[j] = tempShortTrip;
				exist = true;
				break;
			}
		}
		if (!exist)
		{
			tempShortTrip.length = abs(ptr->x - ptr->prev->x) + \
				abs(ptr->y - ptr->prev->y);
			tempShortTrip.count = 1;
			map[shortTripCount] = tempShortTrip;
			shortTripCount++;
		}
		ptr = ptr->prev;
	}
}

bool shortTrip::isEqual(shortTrip &rhs)
{
	bool equal = false;
	if (trip.startCoordinates[0] == rhs.trip.startCoordinates[0] && \
		trip.startCoordinates[1] == rhs.trip.startCoordinates[1] && \
		trip.endCoordinates[0] == rhs.trip.endCoordinates[0] && \
		trip.endCoordinates[1] == rhs.trip.endCoordinates[1])
	{
		equal = true;
	}
	if (trip.startCoordinates[0] == rhs.trip.endCoordinates[0] && \
		trip.startCoordinates[1] == rhs.trip.endCoordinates[1] && \
		trip.endCoordinates[0] == rhs.trip.startCoordinates[0] && \
		trip.endCoordinates[1] == rhs.trip.startCoordinates[1])
	{
		equal = true;
	}
	return equal;
}

/*
void shortTrip::copy(shortTrip &rhs)
{
	trip.startCoordinates[0] = rhs.startCoordinates[0];
	trip.startCoordinates[1] = rhs.startCoordinates[1];
	trip.endCoordinates[0] = rhs.endCoordinates[0];
	trip.endCoordinates[1] = rhs.endCoordinates[1];
	length = abs(trip.startCoordinates[0] - trip.endCoordinates[0])+ \
			abs(trip.startCoordinates[1] - trip.endCoordinates[1]);
	count = 1;
}
*/


//decide which streets to widen based on frequency and weight
void graph::widen(shortTrip map[10005], int &shortTripCount, Trip widenedBlocks[100], int &numWidened)
{
	numWidened = 0;
	int remaining = 100;//100 units allowed to be widened

	for (int i = 0; i < shortTripCount; i++)
	{
		if (remaining == 0)
			break;
		if (100-map[i].length < 0)
			continue;
		else
		{
			remaining = 100 - map[i].length;
			widenedBlocks[numWidened] = map[i].trip;
			numWidened++;
		}
	}
}


inline bool vertex::operator==(vertex &rhs)
{
	return (x==rhs.x && y==rhs.y);
}

inline void vertex::operator=(vertex &rhs)
{
	x = rhs.x;
	y = rhs.y;
}

void graph::reset()
{
	for (int i = 0; i < vertexListCount; i++)
	{
		vertexList[i].dist = 1000000;
		vertexList[i].prev = NULL;
		vertexList[i].known = false;
	}
}