#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <string>
using namespace std;

class Word()
{
public:
  string word;
  int value;
  Word(string w, int v):value(v) {word.assign(w);}
}

int main(int argc, char *argv[])
{
  map<char, int> &letterValues;
  set<Word> legalWords;

  createMap(letterValues);
  getWords(argv[1], legalWords, letterValues);


  return 0;
}

void createMap(map<char, int> &letterValues)
{
  map<char, int>::iterator itr;
  int i = 0;
  int letters[] = {1, 3, 3, 2, 1, 4, 2, 4, 2, 8, 5, 1, 3, 1, \
  	               1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

  for(itr = letterValues.begin(); itr != letterValues.end(), i < 26; itr++)
  {
  	letterValues.insert(itr, pair<char, int>(a + i, letters[i]))
  	i++;
  } 
}

int getWordValue(string word, const map<char, int> &letterValues)
{
  int wordValue = 0;
  string::iterator strItr;
  map<char, int>::iterator mapItr;

  for (strItr = word.begin(); strItr != word.end(); strItr++)
  {
  	for (mapItr = letterValues.begin(); mapItr != letterValues.end(); mapItr++)
  	{
  	  if ((*strItr) == (*mapItr))
  	  	wordValue+= mapItr->second;
  	}
  }

  return wordValue;
}

void getWords(char *filename, set<Word> &legalWords, const map<char, int> &letterValues)
{
  ifstream inf(filename);
  string line;

  while (getline(inf, line))
  {
  	bool isLower = true;
  	for (string::iterator itr = string.begin(); itr != string.end(); itr++)
  	{
  	  if (!(*itr).islower)
  	  {
  	  	isLower = false;
  	  	break;
  	  }
  	}

  	if ((! isLower) || (! line.size() <= 7))
  	  continue;

  	Word tempWord(line, getWordValue(line, letterValues));
  	legalWords.insert(tempWord);
  }
}

