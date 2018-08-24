
int main(int argc, char** argv)
{
  int opCount, index, retval;
  Operation *operations = readFile(argv[1], &opCount);
  currentRAM = 0;
  CPUTimer ct;
  Deque <unsigned short> *deque = new Deque<unsigned short>();
  for(int i = 0; i < opCount; i++)
  {
  switch(operations[i].op)
  {
    case 'F' : deque->push_front(operations[i].value); break;
    case 'B' : deque->push_back(operations[i].value); break;
    case 'f' : deque->pop_front(); break;
    case 'b' : deque->pop_back(); break;
    case 'A' : (*deque)[operations[i].index] = operations[i].value; break;
    case 'a' :
      index = operations[i].index;
      retval = (*deque)[index];

  if(retval != operations[i].value)
    cout << "Error: Operation #" << i << " index #" << index
    << " returned " << retval << " instead of " << operations[i].value
    << endl;
  break;
  default: cout << "Should never get here\n";
  } // switch
  // deque->print();
  } // for
  cout << "CPU Time: " << ct.cur_CPUTime() << " RAM: " << currentRAM << endl;
  return 0;
} // main()

/*
Operation& Operation::readFile(char &filename, char *count)
{
  ifstream inf(filename);
  inf >> count;
  char line[20];
  inf.getline(line, 20);
  for (; count > 0; count--)
  {
  	char i[5];
  	switch(i = )
  }
}
*/
void Deque::Deque()
{
  currentSize = 10;
  front = 0;
  back = -1;
  array.resize(10);
}

bool Deque::isEmpty()
{
  return array.capacity() == 0;
}

bool Deque::isFull()
{
  if (array.size == 0)
  	return false;

  return array.size == array.capacity;
}

void Deque::push_front(const Object &x)
{
  
}