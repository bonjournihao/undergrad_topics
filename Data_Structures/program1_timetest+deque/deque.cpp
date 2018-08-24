#include "deque.h"

template <class Object>
Deque<Object>::Deque(int defaultCapacity)
{
	capacity = defaultCapacity;
	front = 0;
	end = -1;
	size = 0;
	array = new Object[capacity];
}

template <class Object>
Deque<Object>::~Deque()
{
	if (array)
	{
		delete[] array;
	}
}

template <class Object>
bool Deque<Object>::isEmpty()
{
	return size == 0;
}

template <class Object>
bool Deque<Object>::isFull()
{
	return size == capacity;
}

template <class Object>
void Deque<Object>::push_front(const Object& o)
{
	if (front > end)
	{
		increaseCapacity();
	}
	array[front%capacity] = o;
	front++;
	size++;
}

template <class Object>
void Deque<Object>::push_back(const Object& o)
{
	if (front > end)
	{
		increaseCapacity();
	}

	array[end%capacity] = o;
	end--;
	size++;
}

template <class Object>
Object Deque<Object>::pop_front()
{
	if(isEmpty())
	{
		cout << "Deque is empty" << endl;
	}
	else
	{
		size--;
		return array[(--front)%capacity];
	}
}

template <class Object>
Object Deque<Object>::pop_front()
{
	if(isEmpty())
	{
		cout << "Deque is empty" << endl;
	}
	else {
		size--;
		return array[(++back)%capacity];
	}
}

template <class Object>
void Deque<Object>::increaseCapacity()
{
	Object[] newArray = new Object[capacity * 2];
	for (int i = front,j=0; i%capacity <= end; i++,j++) {
		newArray[j] 
	}
}





