#ifndef PARKING_H
#define PARKING_H

#include <iostream>
#include <string.h>
using namespace std;

class Parking
{
public:
    string id;
    string code;
    string name;
    string location;
    int available_spaces;
    Parking() : id(""), code(""), name(""), location(""), available_spaces(0) {}
    Parking(string id, string code, string name, string location, int spaces) : id(id), code(code), name(name), location(location), available_spaces(spaces) {}
    void displayParking(const Parking *parking);
};

#endif