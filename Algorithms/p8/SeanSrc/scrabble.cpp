#include <map>
#include <set>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <iomanip>

using namespace std;

class Word 
{
  string word;
  short count;
public:
  Word(const string &w = "", short c = 0): word(w), count(c) {}
  short getCount() const { return count; }
  bool operator< (const Word &rhs) const { return word < rhs.word; }
  const string& getString() const {return word; }
}; // class Word


typedef map<char, short> LetterMap;

void setLetterValues(LetterMap &letterMap)
{
  short values[] = {1, 3, 3, 2, 1, 4, 2, 4, 2, 8, 5, 1, 3, 1, 1, 3, 10, 1, 
                    1, 1, 1, 4, 4, 8, 4, 10};

  for (char letter = 'a'; letter <= 'z'; letter++)
    letterMap.insert(LetterMap::value_type(letter, values[letter - 'a']));

}  // setLetterValues();


void readWords(const LetterMap &letterMap, set <Word> &words)
{
  ifstream inf("words.txt");
  string word, sortedWord;
  short value;

  while ( getline(inf, word))
  {
    if (word.length() <= 7
        && word.find_first_not_of("abcedefghijklmnopqrstuvwxyz") 
        == string::npos)
    {
      value = 0;

      for (string::const_iterator itr = word.begin(); itr != word.end(); itr++)
        value += (letterMap.find(*itr))->second;

      words.insert(Word(word, value));
    } // if word up to 7 charactes and no captial letters.
  } // while

}  // readWords()

void writeResults(short bestCount, const set<string> &bestWords,
                  const string word)
{
  cout << setw(2) << bestCount << " " << word << ":";

  if (bestCount == 0)
  {
      cout << " No words found.\n";
      return;
  } // if no words

  for (set<string>::const_iterator itr = bestWords.begin();
      itr != bestWords.end(); itr++)
    cout << " " << *itr;

  cout <<  endl;
} // writeResults()


void processBlankPermutation(const set<Word> &words, string &originalWord,
                             set<string> &bestWords, short &bestCount,
                             const LetterMap &letterMap, string &word,
                             char letter)
{
  for (unsigned int j = 1; j <= word.length(); j++)
  {
    string shortenedWord;
    shortenedWord.assign(word, 0, j);
    set<Word>::const_iterator itr = words.find(Word(shortenedWord, 0));

    if (itr != words.end())
    {
      int count = itr->getCount();

      if (std::count((itr->getString()).begin(), (itr->getString()).end(), 
          letter)
          > std::count(originalWord.begin(), originalWord.end(), letter))
          count -= letterMap.find(letter)->second;

      if (count >= bestCount)
      {
        if (count > bestCount)
        {
          bestWords.clear();
          bestCount = count;
        } // if better than those previous

        bestWords.insert(itr->getString());
      } // if at least as good as previous
   } // if word found
 } // for each word size
} // processBlankPermutation


void processBlank(const set<Word> &words, string &originalWord,
                  set<string> &bestWords, short &bestCount, 
                  const LetterMap &letterMap)
{
  bestCount = 0;
  string word = originalWord;
  int pos = word.find(' ');

  for (char letter = 'a'; letter <= 'z'; letter++)
  {
    string temp(1, letter);
    word.replace(pos, 1, temp);

    for (int i = 0; i < 5040; i++)
    {
      processBlankPermutation(words, originalWord, bestWords, bestCount,
                              letterMap, word, letter);
      next_permutation(word.begin(), word.end());
    } // for i
  } // for each letter in blank
}  // processBlank()


void processNormal(const set<Word> &words, string word, set<string> &bestWords,
                   short &bestCount)
{
  bestCount = 0;

  for (int i = 0; i < 5040; i++)
  {
    for (unsigned int j = 1; j <= word.length(); j++)
    {
      string shortenedWord;
      shortenedWord.assign(word, 0, j);
      set<Word>::const_iterator itr = words.find(Word(shortenedWord, 0));

      if (itr != words.end() && itr->getCount() >= bestCount)
      {
        if (itr->getCount() > bestCount)
        {
          bestWords.clear();
          bestCount = itr->getCount();
        } // if better than those previous

        bestWords.insert(itr->getString());
      } // if at least as good as previous
    } // for each word size

    next_permutation(word.begin(), word.end());
  } // for i
}  // processNormal()


void processFile(char **argv, const set <Word> &words,
                 const LetterMap &letterMap)
{
  set <string> bestWords;
  string word;
  short bestCount;
  ifstream inf(argv[1]);

  while (getline(inf, word))
  {
    if (word.find(' ') != string::npos)
      processBlank(words, word, bestWords, bestCount, letterMap);
    else // not a space character
      processNormal(words, word, bestWords, bestCount);

    writeResults(bestCount, bestWords, word);
  } // while
} // processFile()


int main(int argc, char** argv)
{
  LetterMap letterMap;
  set <Word> words;
  setLetterValues(letterMap);
  readWords(letterMap, words);
  processFile(argv, words, letterMap);
  return 0;
} // main()

