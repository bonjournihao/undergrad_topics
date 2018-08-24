#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <fstream>
#include <math.h>
using namespace std;

int which = 100;
vector<float> mylist;
vector<vector<float> > bigger_list;

bool ascending(int i, int j){
      return bigger_list[which][i] < bigger_list[which][j];
}

int main() {
      mylist.push_back(0.55);
      mylist.push_back(0.65);
      mylist.push_back(0.25);
      mylist.push_back(0.95);
      mylist.push_back(0.02);
      bigger_list.push_back(mylist);

      vector<int> index_list;
      for(int i = 0; i < mylist.size(); i++){
            index_list.push_back(i);
      }

      which = 0;
      sort(index_list.begin(), index_list.end(),ascending);

      for(int i = 0; i < mylist.size(); i++){
            cout << index_list[i] << " ";
      }

      for(int i = 0; i < mylist.size(); i++){
            cout << mylist[index_list[i]] << " ";
      }

      cout <<endl;
}