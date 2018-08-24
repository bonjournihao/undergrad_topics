A. Lists:
	1.Array
	2.Linked lists
	3.Cursor lists
	4.Skip lists
B. Stack & queue


1.Array 	[1][2][3][4]

			Insert	find	delete
Unsorted	O(1)	O(N)	O(N)
Sorted		O(N)	O(log(N)) O(N)

2.Linked lists  head->[1][]->[2][]->[3][]

			O(1)	O(N)	O(N)

3.Cursor lists:more than one linked list from cursorSpce 

 1   2   3   4   5   6
[?] [?] [?] [?] [?] [?]
[2] [3] [4] [5] [6] [0]

freelist: free-to-use indices

 1   2   3   4   5   6
[A] [?] [?] [?] [?] [?]   [A][ ]          freelist:2
[0] [3] [4] [5] [6] [0]

 1   2   3   4   5   6
[A] [B] [?] [?] [?] [?]   [A][ ]->[B][0]  freelist:3
[2] [0] [4] [5] [6] [0]

 1   2   3   4   5   6
[A] [B] [C] [?] [?] [?]   [A][ ]->[C][0]  freelist:4
[3] [0] [0] [5] [6] [0]


4.Skip lists : O(log(N))    

probability worst case: 0.5

Max_height = log(Capacity)

P(height = h) = {1/2^h   if h < max_height
				{1/2^h-1 if h = max_height


Stack: push pop top (recursion)

 |3|
 |2|
 |1|
 ---
Queue: push pop 
   _______
 -> 1 2 3 ->
   -------





