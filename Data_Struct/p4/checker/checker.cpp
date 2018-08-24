#include "CheckerRunner.h"
#include "checker.h"
#include "BinaryHeap.h"
#include "vector.h"
#include <cstring>
#include <stdio.h>
#include <iostream>

using namespace std;


Checker::Checker(const DictionaryWord *words, int numWords) //insert into checker
{
	for (int i = 0; i < numWords; i++)
	{
		dictionary.insert(words[i].word);
	}
} // Checker()

void Checker::findWord(const char *word, char matchingWords[100][MAX_LENGTH + 1],
  int *count)
{
	BinaryHeap<String> wordHeap;
	*count = 0;
	//cout << endl << "finding: " << word << endl;
	dictionary.find(word, &wordHeap, count);
	for (int i = 0; i < *count; i++)
	{
		strcpy(matchingWords[i],wordHeap.deleteMin().word);
	}
	//for (int i = 0; i < *count; i++)
		//cout << "found: " << matchingWords[i] << endl;
 	//cout << "count is: " << *count << endl;
}




Trie::Trie()
{
	root = new Node();
}

/*
bool Trie::isEmpty()
{
	return !(root->hasChild()); 
}
*/

Node* Trie::getRoot()
{
	return root;
}

void Trie::insert(const char *word)
{
	Node *nodePtr = root;
	Node *childPtr = NULL;
	//cout << "root has " << root->childrenCount << " children" << endl;
	for (unsigned int i = 0; i < strlen(word); i++)
	{
		//cout << "inserting " << word[i] << " into tree" << endl;
		//cout << "current node to be checked: "<< nodePtr->getLetter() << endl;
		childPtr = nodePtr->hasChild(word[i]);
		if(!childPtr)
		{
			if (nodePtr->children.size() <= nodePtr->childrenCount * 2)
			{
				if (nodePtr->children.size() == 0)
					nodePtr->children.resize(1);
				else
					nodePtr->children.resize(nodePtr->children.size() * 2);
			}
			//cout << word[i] << " doesn't exist" << endl;
			childPtr = nodePtr->addChild(word[i]);
			//cout << "adding: " << childPtr->getLetter() << endl;
			childPtr->setParent(nodePtr);
			//cout << childPtr->getLetter() << " 's parent is " << childPtr->parent->getLetter()<< endl;
			nodePtr = childPtr;
			//cout << "nodePtr now pointing at: " << nodePtr->getLetter() <<endl;
		}
		else
		{
			nodePtr = childPtr;
			//cout << word[i] << " already exists" << endl;
		}
	}
	//add the termination '\0' == make the last node indepedent
	nodePtr->setIndependent();
}

//find matching word(s), and count; letters can only differ once
//ptr = root; while ptr != NULL if match with one children, ptr = ptr->children[*].
//regular case: when loop ends, if ptr->isIndep., match words; record count, word and return
//irregular case: if word[i] != any of ptr->children, mark errorCount = 1, continue finding. Repeat the previous step, 
//when loop ends, if ptr(s)->isIndep., record count & words, and return 
//if errorCount > 1, return;

void Trie::find(const char *word, BinaryHeap<String> *wordHeap, int *count)
{
/*
	Node *ptr = getRoot();
	Node *childPtr;

	vector<Node*> subtreePtr;
	Node *childPtr2;
	bool hasError = false;
	int remainingChance;

	int SubtreeCount;
	//initialize count to 0
	*count = 0;

	//the following 2 for loops could be better if replaced by a recursive call
	//because the program only allows one error in letter so 2 for loops work fine
	for (unsigned int m = 0; m < strlen(word); m++)
	{
		childPtr = ptr->hasChild(word[m]);
		cout << "looking for children: " << word[m] << endl;
		//if there is a matching letter, ptr = childPtr
		if (childPtr)
		{	
			ptr = childPtr;
		}

		//if error occurs, allow one more chance
		else
		{
			cout << "there's no " << word[m] << "\nlooking at next level" << endl;
			hasError = true;
			m++;//next letter
			//start from the next level
			SubtreeCount = ptr->childrenCount;
			remainingChance = SubtreeCount;
			subtreePtr = ptr->children;
			cout << endl;
			while (m < strlen(word))
			{	
				cout << "remaining valid subtrees:" << remainingChance << endl;
				for(int j = 0; j < SubtreeCount; j++)
				{
					//skip NULL subtree[j]
					if (!subtreePtr[j])
						continue;
					cout << "remain: " << subtreePtr[j]->getLetter() << endl;	
					childPtr2 = subtreePtr[j]->hasChild(word[m]);

					//if a child have the next letter, set subtreePtr to it 
					if (childPtr2)
						subtreePtr[j] = childPtr2;

					//if not set ptr to NULL
					else
					{
						subtreePtr[j] = NULL;
						remainingChance--;
					}

					//no more valid letter, return
					if (remainingChance == 0)
						return;
				}//end of for 
				m++;
			}//end of while 
		}//end of else
		if (hasError)
			break;
	}//end of for 

	//irregular case: count >= 1, word # >= 1
	if (hasError)
	{
		int i = 0;
		char wordToCopy[34];
		for (int j = 0; j < SubtreeCount; j++)
		{
			if (subtreePtr[j] == NULL)
				continue;
			if (subtreePtr[j]->isIndependent())
			{
				subtreePtr[j]->getWord(wordToCopy, strlen(word));
				strcpy(matchingWords[i], wordToCopy); 
				(*count)++;
				i++;
			}//end of if
		}//end of for 
		return;
	}
	//regular case:  count = 1, word # = 1
	if (ptr->isIndependent())
	{
		char wordToCopy[34];
		*count = 1;

		ptr->getWord(wordToCopy, strlen(word));
		strcpy(matchingWords[0], wordToCopy);
		return;
	}//end of if
*/

//method 2
/*
Node *ptr = getRoot();

vector<Node*> subtreePtr = ptr->children;
int subtreeCount = ptr->childrenCount;

int remainingCount[subtreeCount];

// count array initialized to 2; when 0, bad word
for (int i = 0; i < subtreeCount; i++)
	remainingCount[i] = 2;
//go through word
for (unsigned int m = 0; m < strlen(word); m++)
{	
	cout << "checking letter: " << word[m] << endl; 
	for (int i = 0; i < subtreeCount; i++)
	{
		//if subtree NULL, skip
		if (!subtreePtr[i])
			continue;
		//if misspelled
		if (subtreePtr[i]->getLetter() != word[m])
		{
			//cout << subtreePtr[i]->getLetter() << " isn't valid" << endl;
			//cout << "remaining count: " << remainingCount[i] - 1 << endl;a
			m++;
			remainingCount[i]--;
			while (m < strlen(word))
			{
				if (!subtreePtr[i]->hasChild(word[m]))
					subtreePtr[i] = NULL;
				else
					subtreePtr[i] = subtreePtr[i]->hasChild(word[m]);
				m++;
			}
			else
				remainingCount[i] = 0;
		}
		else
		{
			subtreePtr[i] = subtreePtr[i]->hasChlid(word[m])
	}//end of for
}// end of for

for (int i = 0; i < subtreeCount; i++)
{
	if (remainingCount[i] == 2)
	{													if (subtreePtr[i]->isIndependent())
		{
			char wordToCopy[34];
			*count = 1;

			subtreePtr[i]->getWord(wordToCopy, strlen(word));
			strcpy(matchingWords[0], wordToCopy);
			return;
		}//end of if
	}//end of id
	else
	{
		int j = 0;
		char wordToCopy[34];
		for (int k = 0; k < subtreeCount; k++)
		{
			if (subtreePtr[k] == NULL)
				continue;
			if (subtreePtr[k]->isIndependent())
			{
				subtreePtr[k]->getWord(wordToCopy, strlen(word));
				strcpy(matchingWords[j], wordToCopy);
				(*count)++;
				j++;
			}// end of if
		}// end of for
	}//end of else
}//end of for
*/

//method 3
//no error case
Node *ptr = root;
bool hasError = false;
for (unsigned int m = 0; m < strlen(word); m++)
{
	if (ptr->hasChild(word[m]))
		ptr = ptr->hasChild(word[m]);
	else
	{	
		hasError = true;
		break;
	}
}

if (!ptr->isIndependent() && !hasError) {
	hasError = true;
}

if (!hasError)
{
	String tempString;
	//cout << "doesnt have error" << endl;
	*count = 1;
	ptr->getWord(tempString.word, strlen(word));
	wordHeap->insert(tempString);
	return;
}
//error case
Node *ptr2 = root;
//cout << "checking other possible words..." << endl;
//int indexToAdd = 0;
ptr2->checkChild(-1, 2, strlen(word), word, wordHeap, count);
return;
/*
Node *errorPtr[strlen(word)];
for (unsigned int i = 0; i < strlen(word); i++)
{
	Node *ptr2 = root;
	int error = 0;
	//check front 1..i-1 
	unsigned int m = 0;
	unsigned int j;
	for (j = 0; j < i; j++)
	{
		if (m >= i)
			break;
		if (!ptr2->hasChild(word[m]))
			errorPtr[i] = NULL; break;
		ptr2 = ptr2->hasChild(word[m]);
		m++;
	}

	if (j!= i)
		continue;
	m++;
	//check back i+1 ... strlen -1
	unsigned int k;
	for (k = i+1; k < strlen(word); k++)		
	{
		if (m == strlen(word))
			break;
		if (!ptr2->hasChild(word[m]))
			errorPtr[i] = NULL; break;
		ptr2 = ptr2->hasChild(word[m]);
		m++;
	}
	if (k == strlen(word))
		errorPtr[i] = ptr2;
}

*count = 0;

int j = 0;
char wordToCopy[34];
for (unsigned int i = 0; i < strlen(word); i++)
{
	if (!errorPtr[i])
		continue;
	errorPtr[i]->getWord(wordToCopy, strlen(word));
	strcpy(matchingWords[j], wordToCopy);
	*count++;
	j++;
}
*/

}//end of findWord


void Node::checkChild(int positionInWord, int remainingCount, int wordLength, const char *word, BinaryHeap<String> *wordHeap, int *count)
{       
	//cout << "position in word: " << positionInWord << endl;
	if (positionInWord != -1)
	{
        	if (getLetter() != word[positionInWord])
		{	
			//cout << "looking for letter: " << word[positionInWord];
			//cout << ", but found: " << getLetter() << endl;
			//cout << "count decermented. " << endl;
                	(remainingCount)--;
		}
	}
        
        if (remainingCount == 0)
	{
               // cout << getLetter() << " had no chance" << endl;
		return;
        }
        if (positionInWord == wordLength - 1 && isIndependent())
        {       
        		String tempString;
                getWord(tempString.word, wordLength);
                wordHeap->insert(tempString);
                (*count)++;
                //(*indexToAdd)++;
                return;
        }
        for (int i = 0; i < childrenCount; i++)
        {      
		//cout << "checking children of " << getLetter() << ": " << children[i]->getLetter()<< ",";
		//cout << getLetter() << "' " << i << "th child" << endl;
                children[i]->checkChild(positionInWord + 1, remainingCount, wordLength, word, wordHeap, count);
        }
}

void Node::getWord(char *word, int length)
{
	Node *tmp = this;
	int i = length-1;
	while (tmp->parent)
	{
		if (i < 0)
			return;
		word[i] = tmp->getLetter();
		tmp = tmp->parent;
		i--;
	}
	word[length] = '\0';
}

Node::Node()
{
    independent = false;
    childrenCount = 0;
    parent = NULL;
} 

Node::Node(const char &l, Node *nL):letter(l)
{
  independent = false;
  childrenCount = 0;
  parent = NULL;
}

void Node::resizeChildren(int size)
{
	children.resize(size);
}

//return pointer to child
Node* Node::hasChild(const char &l)
{
	if (childrenCount == 0)
		return NULL;
	for (int i = 0; i < childrenCount; i++)
	{
		//cout << "children: " <<  ": " << children[0]->getLetter();
		if (l == children[i]->getLetter())
			return children[i];
	}
	return NULL;
}

//return pointer to child
Node* Node::addChild(const char &l)
{
	children[childrenCount] = new Node(l, NULL);
	childrenCount++;
	return children[childrenCount - 1];
}

void Node::setIndependent()
{
	independent = true;
}

bool Node::isIndependent()
{
	return independent;
}

void Node::setParent(Node *p)
{
	parent = p;
}

char Node::getLetter()const
{
	return letter;
}

