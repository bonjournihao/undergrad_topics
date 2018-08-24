#ifndef LIST_H
  #define LIST_H

#include "airport.h"

template <typename T>
class List;

template <typename T>
ostream& List<T>::operator << (ostream &os, const List<T> &rhs);

template <typename T>
class ListNode
{
  T data;
  ListNode *next;
  ListNode(const T &o, ListNode<T> *node);
  friend class List<T>;
  friend ostream& operator << <T>(ostream &os, const List<T> &rhs);
};  // class CityNode


template <typename T>
class List
{
  ListNode<T> *head;
  int count;
public:
  List();
  List(const List<T>& orig);
  ~List();
  int getCount();
  List<T>&  operator += (const T &rhs);
  List<T>& operator -= (const T &rhs);
  const T& operator [] (int index) const;
  T& operator [] (int index);
  friend ostream& operator << <T>(ostream &os, const List<T> &rhs);

}; // class List 

#include "list.cpp"
#endif  // LIST_H 

