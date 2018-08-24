#ifndef _Deque_H
#define _Deque_H

template <class Object>
class Deque
{
	public:
		Deque(int defaultCapacity = 1000);
		~Deque();
		void print();
		void push_front(const Object &o);
		void push_back(const Object &o);
		Object pop_front();
		Object pop_back();

	private:
		int front;
		int back;
		int size;
		int capacity;
		Object *array;
		void increaseCapacity();
		void decreaseCapacity();
		bool isEmpty();
		bool isFull();
};
#endif