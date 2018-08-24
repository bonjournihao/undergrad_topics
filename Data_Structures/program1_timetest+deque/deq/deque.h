#ifndef DEQUE_H
#define	DEQUE_H

template <typename T>
class Deque {
public:
  Deque();
  //~Deque();
  T& operator[] (int index)const;
  T& operator[] (int index);
  void pop_back();
  void push_back(const T &value);
  void pop_front();
  void push_front(const T &value);
  void print()const; 
private:
	int front;
	int back; 
	int size;
	int capacity;
	T *array;
	void increaseCapacity();
	void decreaseCapacity();
	bool isEmpty();
	bool isFull();

}; // class Deque 

#include "deque.cpp"

#endif	// DEQUE_H

