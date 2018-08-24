#ifndef DEQUE_H
#define DEQUE_H

#include <iostream>
#include <vector>

class Operation
{
  public:
  	char op[5];
  	int index;
  	int value
  	void readLines(char *line);
  static int count = 0;
  static Operation& readFile(char &filename, char *count);

};

template <class Object>
class Deque
{
  private:
  	vector<Object> ob;
  	int currentSize;
  	int front;
  	int back;

  public:
    Deque(int capacity);
    void initialize();
    void resize();
    bool isEmpty();
    bool isFull();
    void push_front(const Object &x);
    void push_back(const Object &x);
    void pop_front(const Object &x);
    void pop_back(const Object &x);
    Object& operator [](const Object &rhs); //set value
    Object operator [](const Object &rhs)const; //read only
  
};



#include "deque.cpp"
#endif