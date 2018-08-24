#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <fstream>
#include <math.h>
#include <time.h>
using namespace std;

bool point_in_the_list(vector<vector<int> > list, vector<int> p){
    for(int i = 0; i < list.size(); i++){
        if(list[i][0] == p[0] && list[i][1] == p[1]){
            return true;
        }
    }
    return false;
}

int main(){
    srand (time(NULL));
    float intensity = 0.7;
    int num_grids = round(intensity*10);
    vector<int> this_point(2,-1);
    vector<vector<int> > position_list;
    for(int l = 0; l < num_grids; l++){
        int x = rand() % 3;
        int y = rand() % 3;
        this_point[0] = x;
        this_point[1] = y;
        while(point_in_the_list(position_list,this_point)){
            int x = rand() % 3;
            int y = rand() % 3;
            this_point[0] = x;
            this_point[1] = y;
        }
        position_list.push_back(this_point);
    }
    for(int i = 0; i < position_list.size(); i++){
        int x = position_list[i][0];
        int y = position_list[i][1];
        cout << "x: " << x << " y: " << y << endl;
    }
    return 0;
}