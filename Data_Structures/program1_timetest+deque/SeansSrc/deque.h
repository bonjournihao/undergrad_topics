#ifndef DEQUE_H
#define	DEQUE_H

#define ROW_SIZE 1000
#define ROW_COUNT 5001

template <typename T>
class Deque {
  int rowCount;
  int zeroPos;
  T *rows[ROW_COUNT];
  int count;
public:
  Deque();
  T& operator[] (int index)const;
  T& operator[] (int index);
  void pop_back();
  void push_back(const T &value);
  void pop_front();
  void push_front(const T &value);
  void print();
  
}; // class Deque 

#include "deque.cpp"

#endif	// DEQUE_H

