#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream> 
#include <iomanip>
#include <fstream>
#include "QueueLL.h"
#include "AvlTree.h"
using namespace std;



//when take in words, make all capitalized letters lower-case
struct Word{
	char word[50];
	int count;
	//used for inserting Word into the avl tree
	QueueLL<long> *lineNumList;
	Word():count(0), QueueLL<long>()
	{};
	Word(char *w):count(0)
	{
		strcpy(word, w);
	}
	~Word()
	{
		delete lineNumList;
	}
	inline bool operator <(const Word &rhs)
	{
		return (strcmp(word, rhs.word) < 0);
	}
	//used for finding an existing Word
	inline bool operator >(const Word &rhs)
	{
		return (strcmp(word, rhs.word) > 0);
	}
	inline bool operator != (const Word &rhs)
	{
		return strcmp(word, rhs.word) != 0;
	}

	void increaseCount()
	{
		count++;
		return;
	}

	//print word, count and lineNum
	//topAndPop to print the lineNumList from the beginning
	friend inline ostream& operator <<(ostream& os, Word &rhs)
	{
		os << setw(18) << left << rhs.word << rhs.count << "   ";//print word and count
		os << rhs.lineNumList->dequeue(); //print 1st lineNum
		while (!rhs.lineNumList->isEmpty())//print the rest
		{
			os << "," << rhs.lineNumList->dequeue();
		}
		os << endl;
		return os;
	}
};


void readWord(char tempWord[50], AvlTree<Word> *tree, long lineNum);
void readWords(char *line, AvlTree<Word> *tree, long lineNum);

int main(int argc, char **argv)
{
	Word ITEM_NOT_FOUND("itemNotFound");
	AvlTree<Word> *tree = new AvlTree<Word>(ITEM_NOT_FOUND);

	ifstream inf(argv[1]);
	char line[200];
	long lineNum = 1;

	while (inf.getline(line, 200))
	{
		//read in words line by line
 		readWords(line, tree, lineNum);
		lineNum++;
	}
	tree->printTree();
	return 0;
}

//pick valid words in one line
void readWords(char *line, AvlTree<Word> *tree, long lineNum)
{
	char tempWord[50];

	//eliminate all non-alphabatic signs
	char *ptr = strtok(line, "\n !\"#$&`()*+,-./:;<=>\?@[\\]%%^_\'{|}~");
	while (ptr != NULL)
	{
		strcpy(tempWord, ptr);
		//cout << ptr << endl;
		readWord(tempWord, tree, lineNum);
		ptr = strtok(NULL, "\n !\"#$&`()*+,-./:;<=>\?@[\\]%%^_\'{|}~");
	}
	return;
}

//insert words on one line into tree
  //if the word exists, increment count, add line number
  //if new word, insert into tree 
void readWord(char tempWord[50], AvlTree<Word> *tree, long lineNum)
{
	//const Word *targetWord;
	Word targetWord;
	Word ITEM_NOT_FOUND("itemNotFound");
	Word tempWordWord;
	strcpy(tempWordWord.word, tempWord);
	//const Word *newWord;
	//if word already exist, use make copy of targetWord ptr
	// make changes to the copy, and delete targetWord in the tree
	targetWord = tree->find(tempWordWord);
	if (targetWord != ITEM_NOT_FOUND) //potential problem here
	{
		//newWord->count += tempWord->count;
		targetWord.increaseCount();
		//newWord->lineNumList = targetWord->lineNumList;
		//newWord->lineNumList->enqueue(lineNum);
		targetWord.lineNumList->enqueue(lineNum);

		tree->remove(tree->find(tempWordWord));
		tree->insert(targetWord);
		//tree->remove(targetWord);
		//tree->insert(newWord);
	}
	//if new word, insert into tree
	else
	{
		//cout << "creating new" << endl;
		for (int i = 0; i< strlen(tempWord); i++)
		{
			if (isupper(tempWord[i]))
				tempWord[i] = tolower(tempWord[i]);
		}
		//strcpy(newWord->word, tempWord);
		//cout << tempWord << endl;
		strcpy(tempWordWord.word, tempWord);
		//newWord->count++;
		tempWordWord.increaseCount();
		//newWord->lineNumList->enqueue(lineNum);
		//cout << "enqueuing num" << endl;
		tempWordWord.lineNumList->enqueue(lineNum);
		//tree->insert(newWord);
		//cout << "inserting into tree" << endl;
		cout << "word to be inserted: " << tempWordWord.word << endl;
		tree->insert(tempWordWord);
	}
	return;
}