// Author: Sean Davis
#include "deque.h"
#include "mynew.h"
#include <iostream>
using namespace std;

 
inline int positive_modulo(int i, int n) {
    return (i % n + n) % n;
} 

template <typename T>
Deque<T>::Deque() 
{
	capacity = 500;
	front = 0;
	back = -1;
	size = 0;
	array = new T[capacity];
} // deque()

/*
template <typename T>
Deque<T>::~Deque()
{
	if (array)
	{
		delete[] array;
	}
}
*/

template <typename T>
bool Deque<T>::isEmpty()
{
	return size == 0;
}

template <typename T>
bool Deque<T>::isFull()
{
	return size == capacity;
}

template <typename T>
T& Deque<T>::operator[] (int index)const
{
   return array[positive_modulo(front - 1 - index, capacity)];
  //return 0;  // added to allow error free compilation with empty class.
}  // operator[] read


template <typename T>
T& Deque<T>::operator[] (int index)
{
  //unsigned short num;  // added to allow error free compilation with empty class
  return array[positive_modulo(front - 1 - index, capacity)];
  //return num;  // added to allow error free compilation with empty class
} // operator[] write


template <typename T>
void Deque<T>::pop_back()
{
	if(isEmpty())
	{
		cout << "Deque is empty" << endl;
		return;
	}
	if(double(size) <= capacity * 0.5) {
		decreaseCapacity();
	}
	size--;
	++back;
}  // pop_back()


template <typename T>
void Deque<T>::push_back(const T &value)
{
	if (isFull())
	{
		increaseCapacity();
	}

	array[positive_modulo(back, capacity)] = value;
	back--;
	size++;
} // push_back()


template <typename T>
void Deque<T>::pop_front()
{
	if(isEmpty())
	{
		cout << "Deque is empty" << endl;
		return;
	}

	if(double(size) <= capacity * 0.5) {
		decreaseCapacity();
	}

	size--;
	front--;
} // pop_front()


template <typename T>
void Deque<T>::push_front(const T &value)
{
	if (isFull())
	{
		//cout << "deque is full" << endl;
		increaseCapacity();
	}
	//cout << "insert into" << front%capacity << endl;
	array[positive_modulo(front, capacity)] = value;
	front++;
	size++;
} // push_front()

template <typename T>
void Deque<T>::increaseCapacity()
{
	T *newArray = new T[capacity * 2];
	int i, j;
	for (i = back + 1,j=0; ; i++,j++) {
		//int pos = positive_modulo(i,capacity);
		//cout << "copy " << pos << "into new array" << j;
		newArray[j] = array[positive_modulo(i,capacity)];
		if(positive_modulo(i, capacity) == positive_modulo(front-1, capacity))
		{
			break;
		}
	}

	front = j + 1;
	back = -1;
	delete[] array;
	array = newArray;
	capacity *= 2;
}

template <typename T>
void Deque<T>::decreaseCapacity()
{
	T *newArray = new T[capacity / 2];
	int i, j;
	for (i = back + 1,j=0; ; i++,j++) {
		//int pos = positive_modulo(i,capacity);
		//cout << "copy " << pos << "into new array" << j;
		newArray[j] = array[positive_modulo(i,capacity)];
		if(positive_modulo(i, capacity) == positive_modulo(front-1, capacity))
		{
			break;
		}
	}

	front = j + 1;
	back = -1;
	delete[] array;
	array = newArray;
	capacity /= 2;
}

/*
template <typename T>
void Deque<T>::print() const
{
	for (int i = front -1;; i--) {
		if (positive_modulo(i, capacity) == positive_modulo(back, capacity))
		{
			break;
		}
		cout << array[i] << ",";
	}
	cout << endl;
}*/