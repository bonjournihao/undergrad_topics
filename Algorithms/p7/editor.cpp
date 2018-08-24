#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;


void readFile(const char *sourceFile, vector<string> &str_vec)
{
  ifstream inf(sourceFile);
  string line;
  while (getline(inf, line))
  	str_vec.push_back(line);
} // readFile

void insertString(string line, vector<string> &str_vec)
{
  int blank1 = line.find(" ");
  int blank2 = line.find(" ", blank1 + 1);
  int blank3 = line.find(" ", blank2 + 1);
  int lineNum = atoi(line.substr(blank1 + 1, blank2 - blank1 - 1).c_str());
  int wordPos = atoi(line.substr(blank2 + 1, blank3 - blank2 - 1).c_str());
  string word = line.substr(blank3 + 1);

  vector<string>::iterator itr = str_vec.begin();
  itr += lineNum;
  (*itr).insert(wordPos, word);
}

void replaceString(string line, vector<string> &str_vec)
{
  int blank1 = line.find(" ");
  int blank2 = line.find(" ", blank1 + 1);
  int blank3 = line.find(" ", blank2 + 1);
  int blank4 = line.find(" ", blank3 + 1);
  int lineNum = atoi(line.substr(blank1 + 1, blank2 - blank1 - 1).c_str());
  int startPos = atoi(line.substr(blank2 + 1, blank3 - blank2 - 1).c_str());
  int endPos = atoi(line.substr(blank3 + 1, blank4 - blank3 - 1).c_str());
  string word = line.substr(blank4 + 1);

  vector<string>::iterator itr=str_vec.begin();
  itr += lineNum;
  (*itr).replace(startPos, endPos - startPos + 1, word);
}

void findLines(string line, const vector<string> &str_vec)
{
  int blankPos = line.find(" ");
  string wordToFind = line.substr(blankPos + 1);

  cout << wordToFind.c_str();

  vector<string>::const_iterator itr;
  int i = 0;
  for (itr = str_vec.begin(); itr != str_vec.end(); itr++)
  {
    if ((*itr).find(wordToFind) != string::npos)
      cout << " " << i;
    i++;
  }
  cout << endl;
}

void deleteLines(string line, vector<string> &str_vec)
{
  int blank1 = line.find(" ");
  int blank2 = line.find(" ", blank1 + 1);

  int startPos = atoi(line.substr(blank1 + 1, blank2 - blank1 - 1).c_str());
  int endPos = atoi(line.substr(blank2 + 1).c_str());

  str_vec.erase(str_vec.begin()+startPos, str_vec.begin()+endPos);
}

void eraseString(string line, vector<string> &str_vec)
{
  int blank = line.find(" ");
  string eraseWord = line.substr(blank + 1);

  vector<string>::iterator itr;
  for (itr = str_vec.begin(); itr != str_vec.end(); itr++)
  {
    if (int erasePos = (*itr).find(eraseWord))
    {
      (*itr).erase(erasePos, eraseWord.size());
    }
  }
}

void copyPasteLines(string line, vector<string> &str_vec)
{
  int blank1 = line.find(" ");
  int blank2 = line.find(" ", blank1 + 1);
  int blank3 = line.find(" ", blank2 + 1);

  int startPos = atoi(line.substr(blank1 + 1, blank2 - blank1 - 1).c_str());
  int endPos = atoi(line.substr(blank2 + 1, blank3 - blank2 - 1).c_str());
  int desPos = atoi(line.substr(blank3 + 1).c_str());

  str_vec.insert(str_vec.begin()+desPos, str_vec.begin()+startPos, str_vec.begin()+endPos);
}

void moveLines(string line, vector<string> &str_vec)
{
  copyPasteLines(line, str_vec);

  int lastBlank = line.rfind(" ");
  deleteLines(line.substr(0, lastBlank), str_vec);
}

void writeFile(const char *desFile, const vector<string> &str_vec)
{
  ofstream os(desFile);
  vector<string>::const_iterator itr;
  for (itr = str_vec.begin(); itr != str_vec.end(); itr++)
  	os << *itr << endl; 
}

void readCommand(const char *commandFile, vector<string> &str_vec)
{
  ifstream inf(commandFile);
  string line;
  while (getline(inf, line))
  {
    if (line.compare(0, 1, "I") == 0)
      insertString(line, str_vec);
    if (line.compare(0, 1, "R") == 0)
      replaceString(line, str_vec);
    if (line.compare(0, 1, "F") == 0)
      findLines(line, str_vec);
    if (line.compare(0, 1, "D") == 0)
      deleteLines(line, str_vec);
    if (line.compare(0, 1, "E") == 0)
      eraseString(line, str_vec);
    if (line.compare(0, 1, "C") == 0)
      copyPasteLines(line, str_vec);
    if (line.compare(0, 1, "M") == 0)
      moveLines(line, str_vec);
  }
}

int main(int argc, char **argv)
{
  vector<string> str_vec;

  readFile(*argv, str_vec);
  readCommand((*argv)+1, str_vec);
  writeFile((*argv)+2, str_vec);
  return 0;
} // main()