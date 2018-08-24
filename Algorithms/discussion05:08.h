class ListNode()
{
  Person *person;
  ListNode *next;
  ListNode(Person *p, ListNode *n):next(n)
  {
  	person = new Person(*p);
  }
}// class ListNode

class List
{
  ListNode *head;
 public:
  void insert(Person *p); //insert name in sorted Person list
  void _delete(Person *p);
  int find(Person *p);
  void destroy();
  bool isEmpty(){return head == NULL; }
  void printAll(); //print all the contents of Person list
}