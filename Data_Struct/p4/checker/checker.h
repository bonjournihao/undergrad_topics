#ifndef CHECKER_H
#define	CHECKER_H

#include "CheckerRunner.h"
#include "vector.h"
#include "BinaryHeap.h"
#include <cstring>

class String
{
public:
	char word[34];

	inline bool operator<(const String &rhs) const
	{
		return strcmp(word, rhs.word) < 0;
	}
	inline String operator=(const String &rhs)
	{
		strcpy(word,rhs.word);
		return *this;
	}
};

class Node{  
public:
  char letter;
  vector<Node*> children;
  bool independent;
  Node *parent;
  int childrenCount;
  Node();//construct root
  Node(const char &l, Node *nL);//construct children
  Node* hasChild(const char &l); //return ptr to child
  Node* addChild(const char &l); // return ptr to child
  void resizeChildren(int size);
  void setIndependent();
  void checkChild(int positionInWord, int remainingCount, int worDLength, const char *word, BinaryHeap<String> *wordHeap, int *count);
  bool isIndependent();
  void setParent(Node *p);
  char getLetter()const;
  void getWord(char *word, int length);
  friend class Trie;
}; //class Node


class Trie{
private:
  Node *root;
public:
  Trie();//new root
  //bool isEmpty();//return true if the trie is empty
  Node *getRoot();
  void insert(const char *word);//insert from words.txt into trie
  void find(const char *word, BinaryHeap<String> *wordHeap, int *count);
}; // class Trie


class Checker {
public:
  Checker(const DictionaryWord *words, int numWords);
  void findWord(const char *word, char matchingWords[100][MAX_LENGTH + 1],
    int *count);
private:
  Trie dictionary;
}; // class Checker





#endif	// CHECKER_H

