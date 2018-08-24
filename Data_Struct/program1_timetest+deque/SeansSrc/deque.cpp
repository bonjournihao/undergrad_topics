// Author: Sean Davis

#include <iostream>
using namespace std;

template <typename T>
Deque<T>::Deque() : rowCount(1), zeroPos(ROW_SIZE/2), count(0)  
{
  rows[0] = new T[ROW_SIZE];

} // deque()


template <typename T>
T& Deque<T>::operator[] (int index)const
{
  
  int row = (index + zeroPos) / ROW_SIZE;
  int pos = (index + zeroPos) % ROW_SIZE;
  return rows[row][pos];
}  // operator[] read

template <typename T>
T& Deque<T>::operator[] (int index)
{
  int row = (index + zeroPos) / ROW_SIZE;
  int pos = (index + zeroPos) % ROW_SIZE;
  return rows[row][pos];
} // operator[] write

template <typename T>
void Deque<T>::pop_back()
{
  int pos = (count + zeroPos - 1) % ROW_SIZE;

  if(pos == 0)
  {
    delete [] rows[rowCount - 1];
    rowCount--;
  }  // if zeroPos at end of row
  
  count--;
}  // pop_back()

template <typename T>
void Deque<T>::push_back(const T &value)
{
  int row = (count + zeroPos) / ROW_SIZE;
  int pos = (count + zeroPos) % ROW_SIZE;
  
  if(pos == 0)
    rows[rowCount++] = new T[ROW_SIZE];
  
  rows[row][pos] = value;
  
  if(++count == 1)
    zeroPos = pos;
} // push_back()


template <typename T>
void Deque<T>::pop_front()
{
  if(zeroPos == ROW_SIZE - 1)
  {
    delete [] rows[0];
    rowCount--;
    
    for(int i = 0; i < rowCount; i++)
      rows[i] = rows[i + 1];
    
    zeroPos = 0;
  }  // if zeroPos at end of row
  else
    zeroPos++;
  
  count--;
} // pop_front()


template <typename T>
void Deque<T>::push_front(const T &value)
{
  if(zeroPos == 0)
  {
    for(int i = rowCount++; i > 0; i--)
      rows[i] = rows[i - 1];
    
    rows[0] = new T[ROW_SIZE];
    zeroPos = ROW_SIZE - 1;
    rows[0][ROW_SIZE - 1] = value;
  }  
  else
    rows[0][--zeroPos] = value; 
  
  count++;
} // push_front()


template <typename T>
void Deque<T>::print()
{
  for(int i = 0; i < count; i++)
    cout << i << ": " << (*this)[i] << endl;
    
  cout << endl;
}

