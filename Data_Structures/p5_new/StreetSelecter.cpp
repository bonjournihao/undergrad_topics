#include <cstdlib>
#include <iostream>
using namespace std;
#include "StreetSelecter.h"
#include "AvlTree.h"


StreetSelecter::StreetSelecter(Block *blocks, int blockCount, int citySize,
  int numTrips)
{
	g = new graph();//create a graph
	g->createAdjList(blocks, blockCount, citySize);
	//for (int i = 0; i < g->vertexListCount; i++)
	//{
	//	cout << i <<"th vertex: " << g->vertexList[i]->x << "," << g->vertexList[i]->y << endl;
	//	cout << " has" << g->vertexList[i]->adjListCount << " adjacent vertices" << endl;
	//}
} // StreetSelecter()

void StreetSelecter::select(Trip *trips, int numTrips, Trip widenedBlocks[100], int &numWidened)
{
	//1. call findPath for every trip
	//2. compare the shortTrip map to decide which blocks to widen;

	shortTrip map[10005];//store all short paths' information
	int shortTripCount = 0;
	for (int i = 0; i < numTrips; i++)
	{
		vertex *tempStart = g->findVertex(trips[i].startCoordinates[0], trips[i].startCoordinates[1]);
		vertex *tempEnd = g->findVertex(trips[i].endCoordinates[0], trips[i].endCoordinates[1]);
		//find shortest path from start to end
		//store frequency and weight of trips into map
		g->findPath(tempStart, tempEnd, map, shortTripCount);
		g->reset();
	} 


	/*
	cout << endl << endl;
	cout << "In shortTripMap: " << endl;
	for(int i = 0; i < shortTripCount; i++)
	{
		cout << map[i].trip.startCoordinates[0] << "," << map[i].trip.startCoordinates[1] \
		<< " to " << map[i].trip.endCoordinates[0] << "," << map[i].trip.endCoordinates[1]<<endl;
		cout << "count: " << map[i].count << endl;
	}
	*/



	g->widen(map, shortTripCount, widenedBlocks, numWidened);//choose blocks to widen


	/*
	for (int i = 0; i < numWidened; i++)
		cout << "widen blocks:" << widenedBlocks[i].startCoordinates[0] << "," << widenedBlocks[i].startCoordinates[1] \
		<< " to " << widenedBlocks[i].endCoordinates[0] << "," << widenedBlocks[i].endCoordinates[1]<<endl;
		*/
} // select()

vertex* graph::findVertex(short x, short y) {
	for(int i = 0; i < vertexListCount; i++) {
		if(vertexList[i]->x == x && vertexList[i]->y==y) {
			return vertexList[i];
		}
	}
	return NULL;
}

void graph::createAdjList(Block *blocks, int blockCount, int citySize)
{
	vertexList = new vertex*[citySize*citySize];//vertexNum == citySize^2

	//insert each block into list(using 2 vertices)
	for (int i = 0; i < blockCount; i++)
	{
		//create temporary vertex to be inserted int adjList
		vertex *temp1 = findVertex(blocks[i].startCoordinates[0], blocks[i].startCoordinates[1]);
		vertex *temp2 = findVertex(blocks[i].endCoordinates[0], blocks[i].endCoordinates[1]);
		if (temp1==NULL) {
			temp1 = new vertex(blocks[i].startCoordinates[0], blocks[i].startCoordinates[1]);
			vertexList[vertexListCount++] = temp1;
		}

		if(temp2 == NULL) {
			temp2 = new vertex(blocks[i].endCoordinates[0], blocks[i].endCoordinates[1]);
			vertexList[vertexListCount++] = temp2;
		}

		temp1->addAdj(temp2);
		temp2->addAdj(temp1);

	}//end of outer for
}//createAdjList

bool graph::hasUnknown()
{
	for (int i = 0; i < vertexListCount; i++)
	{
		if (vertexList[i]->known == false)
			return true;
	}
	return false;
}


void graph::reset() {
	for (int i = 0; i < vertexListCount; i++)
	{
		vertexList[i]->known = false;
		vertexList[i]->prev = NULL;
		vertexList[i]->dist = 1000000;
	}
}

vertex* graph::smallestUnknown()
{
	vertex* v = new vertex();
	for (int i = 0; i < vertexListCount; i++)
	{
		if (!vertexList[i]->known && vertexList[i]->dist < v->dist)
			v = vertexList[i];
	}
	return v;
}

int graph::distBetween(vertex v1, vertex v2)
{
	return abs(v1.x - v2.x) + abs(v1.y - v2.y);
}

//dijkstra's
void graph::findPath(vertex *source, vertex *dest, shortTrip map[10005], int &shortTripCount)
{
	source->dist = 0;
	/*
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
		v->known = true;
		for (int i = 0; i < v->adjListCount; i++)
		{
			if (v->adjList[i]->known)// if an adjacent vertex is true, skip
				continue;

			int tempDist = distBetween(*v, *(v->adjList[i])) + v->dist;
			if (v->adjList[i]->dist > tempDist)
			{
				v->adjList[i]->dist = tempDist;
				v->adjList[i]->prev = v;
			}//end of if
		}//end of for
	}//end of while
	*/

	//min heap based on dist
	//insert all, delete min, update min's adjacent vertex's dist and prev; if known, skip
	//re-insert into heap
	//if min == dest, return

	AvlTree *minTree = NULL;

	int UnknownCount = vertexListCount - 1;
	vertex *knownVertex = source;

	for (int i = 0; i < vertexListCount; i++)
	{
		cout << "inserting into avl tree" << endl;

		minTree->insert(vertexList[i]);
	}

	cout << "finished insertion" << endl;
	//mark one vertex unknown each time (not necessary though)
	while(UnknownCount >= 0)
	{
		//if the deleted vertex from heap is dest, return
		if (knownVertex == dest)
		{
			findShortTrips(dest, map, shortTripCount);
			break;
		}

		/*

		//insert all unknown vertices into heap
		for (int i = 0; i < vertexListCount; i++)
		{
			if (vertexList[i]->known)
				continue;
			//cout << "inserting " << vertexList[i]->x << "," << vertexList[i]->y << " into heap: " << i << endl;
			minHeap.insert(vertexList[i]);
		}
		*/

		//return min vertex and mark known

		knownVertex = minTree->findMin();
		minTree->remove(minTree->findMin());
		//cout << knownVertex->x << "," << knownVertex->y << " marked true" << endl;
		knownVertex->known = true;

		//update adjacent vertices
		for (int i = 0; i < knownVertex->adjListCount; i++)
		{
			if (knownVertex->adjList[i]->known == true)
				continue;
			int dist = knownVertex->dist + distBetween(*knownVertex, *(knownVertex->adjList[i]));
			if ( dist < knownVertex->adjList[i]->dist)
			{
				knownVertex->adjList[i]->dist = dist;
				knownVertex->adjList[i]->prev = knownVertex;
			}
		}//end of for
		UnknownCount--;
	}//end of while
}

void graph::findShortTrips(vertex *dest, shortTrip map[10005], int &shortTripCount)
{
	bool exist = false;
	vertex *ptr = dest;

	//cout << endl;
	//cout << "result from dijkstra's" << endl;
	while (ptr->prev)
	{
		//if there exists a trip or there are no trips at all, increment shortTripCount
		//otherwise, increment the the count in trips
		//cout << ptr->x << "," << ptr->y << " to " << ptr->prev->x << "," << ptr->prev->y << endl;

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
				/*int j = i;
				//sort the new map
				for (; j > 0; j--)
				{
					if (map[j-1].count < map[i].count)
						map[j] = map[j-1];
					else
						break;
				}
				map[j] = tempShortTrip;
				*/
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



//decide which streets to widen based on frequency and weight
void graph::widen(shortTrip map[10005], int &shortTripCount, Trip widenedBlocks[100], int &numWidened)
{

	//insertion sort
	shortTrip sortedMap[10005];
	sortedMap[0] = map[0];
	for (int i = 1; i < shortTripCount;i++)
	{
		shortTrip newItem = map[i];
		int j;
		for (j = i - 1; j >= 0; j--)
		{
			if (sortedMap[j].count < newItem.count)
				sortedMap[j+1] = sortedMap[j];
			else
			{
				sortedMap[j+1] = newItem;
				break;
			}
		}
		if (j < 0)
			sortedMap[0] = newItem;
	}

	numWidened = 0;
	int remaining = 100;//100 units allowed to be widened

	for (int i = 0; i < shortTripCount; i++)
	{
		if (remaining == 0)
			break;
		if (100-sortedMap[i].length < 0)
			continue;
		else
		{
			remaining = 100 - sortedMap[i].length;
			widenedBlocks[numWidened] = sortedMap[i].trip;
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
	known = rhs.known;
	known = rhs.prev;
}

void vertex::addAdj(vertex * v){
	adjList[adjListCount++] = v;
}
