void List::printAll()
{
  ListNode *ptr = head;
  for (ptr = head; ptr; ptr = ptr->next)
  {
  	cout << *(ptr->person);
  }
}

void insert(const Person *p)
{
  ListNode *ptr = head;
  ListNode *prev = Null;
  for (ptr = head; ptr&&strcmp(ptr->person->getName, p->getName); ptr = ptr->next)
  {
  	prev = ptr;
  }
  if (prev) // middle of list
  	prev->next = new ListNode(p, ptr);
  else // front of list
  	head = new ListNode(p, ptr);
}