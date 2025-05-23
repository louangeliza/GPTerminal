#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <regex>
using namespace std;
struct Road {
    string name;
};
map<string, Road> roads;
map<string, vector<pair<string, string>>> graph;
const string ROADS_FILE = "roads.txt";

void loadRoads(){
    roads.clear();
    ifstream file(ROADS_FILE);
    string line;
    getline(file,line);
    while(getline(file,line)){
        stringstream ss(line);
        string index,name;
    }
}