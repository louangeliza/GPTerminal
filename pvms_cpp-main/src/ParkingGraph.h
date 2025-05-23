#ifndef PARKING_GRAPH
#define PARKING_GRAPH

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include "Parking.h"

using namespace std;

class ParkingGraph
{
private:
    map<string, vector<pair<string, int>>> neighbours;
    map<string, Parking> parkings;

    string toLower(const string &s)
    {
        string lower = s;
        transform(lower.begin(), lower.end(), lower.begin(),
                  [](unsigned char c)
                  { return tolower(c); });
        return lower;
    }

public:
    bool parkingExists(const string &id);
    bool roadExists(const string &parkingId1, const string &parkingId2);
    bool addParking(const string &id, const string &code, const string &name, const string &location, const int &available_spaces);
    bool addRoad(const string &parkingId1, const string &parkingId2);
    bool updateParking(const string &id, const string &code, const string &name, const string &location, const int &spaces);
    bool deleteParking(const string &id);
    bool deleteRoad(const string &parkingId1, const string &parkingId2);
    Parking *getParking(const string &id);
    int *getRoadDistance(const string &id1, const string &id2);
    void saveToCSV(const string &filename);
    void saveToText(const string &filename);
    void printGraph();
    bool loadFromCSV(const string &filename);
    vector<Parking *> searchParking(const string &query);
};

#endif