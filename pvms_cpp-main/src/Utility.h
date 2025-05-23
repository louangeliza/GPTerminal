#ifndef UTILITY
#define UTILITY

#include<iostream>
#include <algorithm>
using namespace std;

class Utility {
    public:
    int getIntegerInput(const string &input, int min = 0);
    string getStringInput(const string &prompt, bool toLowercase = false);
    void generateGraphvizDOT(const string &filename);
};

#endif