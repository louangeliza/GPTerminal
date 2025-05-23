#include "ParkingGraph.h"
#include "Utility.h"
#include <fstream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <unordered_set>

using namespace std;

bool ParkingGraph::parkingExists(const string &id)
{
    return parkings.find(id) != parkings.end();
}

bool ParkingGraph::roadExists(const string &parkingId1, const string &parkingId2)
{
    if (!parkingExists(parkingId1) || !parkingExists(parkingId2))
        return false;

    for (const auto &conn : neighbours[parkingId1])
    {
        if (conn.first == parkingId2)
            return true;
    }
    return false;
}

bool ParkingGraph::addParking(const string &id, const string &code,
                              const string &name, const string &location,
                              const int &available_spaces)
{
    if (parkingExists(id))
    {
        cout << "Error: Parking with ID " << id << " already exists.\n";
        return false;
    }
    parkings.emplace(id, Parking(id, code, name, location, available_spaces));
    return true;
}

bool ParkingGraph::addRoad(const string &parkingId1, const string &parkingId2)
{
    if (!parkingExists(parkingId1))
    {
        cout << "Error: Parking " << parkingId1 << " doesn't exist.\n";
        return false;
    }
    if (!parkingExists(parkingId2))
    {
        cout << "Error: Parking " << parkingId2 << " doesn't exist.\n";
        return false;
    }
    if (roadExists(parkingId1, parkingId2))
    {
        cout << "Error: Road already exists between " << parkingId1 << " and " << parkingId2 << ".\n";
        return false;
    }
    if (parkingId1 == parkingId2)
    {
        cout << "Error: Cannot connect a parking to itself.\n";
        return false;
    }

    // Default distance of 1 unit between connected parkings
    neighbours[parkingId1].emplace_back(parkingId2, 1);
    neighbours[parkingId2].emplace_back(parkingId1, 1);
    return true;
}

bool ParkingGraph::updateParking(const string &id, const string &code,
                                 const string &name, const string &location,
                                 const int &spaces)
{
    if (!parkingExists(id))
    {
        cout << "Error: Parking " << id << " doesn't exist.\n";
        return false;
    }
    parkings[id].code = code;
    parkings[id].name = name;
    parkings[id].location = location;
    parkings[id].available_spaces = spaces;
    return true;
}

bool ParkingGraph::deleteParking(const string &id)
{
    if (!parkingExists(id))
    {
        cout << "Error: Parking " << id << " doesn't exist.\n";
        return false;
    }

    // Remove all roads connected to this parking
    for (auto &conn : neighbours[id])
    {
        string otherId = conn.first;
        auto &otherConns = neighbours[otherId];
        otherConns.erase(remove_if(otherConns.begin(), otherConns.end(),
                                   [&id](const pair<string, int> &p)
                                   { return p.first == id; }),
                         otherConns.end());
    }
    neighbours.erase(id);
    parkings.erase(id);
    return true;
}

bool ParkingGraph::deleteRoad(const string &parkingId1, const string &parkingId2)
{
    if (!roadExists(parkingId1, parkingId2))
    {
        cout << "Error: No road exists between " << parkingId1 << " and " << parkingId2 << ".\n";
        return false;
    }

    // Remove connection from parking1 to parking2
    auto &conns1 = neighbours[parkingId1];
    conns1.erase(remove_if(conns1.begin(), conns1.end(),
                           [&parkingId2](const pair<string, int> &p)
                           { return p.first == parkingId2; }),
                 conns1.end());

    // Remove connection from parking2 to parking1
    auto &conns2 = neighbours[parkingId2];
    conns2.erase(remove_if(conns2.begin(), conns2.end(),
                           [&parkingId1](const pair<string, int> &p)
                           { return p.first == parkingId1; }),
                 conns2.end());

    return true;
}

Parking *ParkingGraph::getParking(const string &id)
{
    if (!parkingExists(id))
        return nullptr;
    return &parkings[id];
}

int *ParkingGraph::getRoadDistance(const string &id1, const string &id2)
{
    if (!roadExists(id1, id2))
        return nullptr;

    for (auto &conn : neighbours[id1])
    {
        if (conn.first == id2)
        {
            return &conn.second;
        }
    }
    return nullptr;
}

void ParkingGraph::printGraph()
{
    cout << "\n=== Parking Network Summary ===\n";

    cout << "\nParking Lots in the network:\n";
    for (const auto &parking : parkings)
    {
        cout << "ID: " << parking.second.id
             << ", Code: " << parking.second.code
             << ", Name: " << parking.second.name
             << ", Location: " << parking.second.location
             << ", Available Spaces: " << parking.second.available_spaces << endl;
    }

    cout << "\nRoad connections between parking lots:\n";
    for (const auto &entry : neighbours)
    {
        cout << "From " << entry.first << " (" << parkings[entry.first].name << ") to:\n";
        for (const auto &connection : entry.second)
        {
            if (entry.first < connection.first)
            {
                cout << "  - " << connection.first << " (" << parkings[connection.first].name
                     << "), Distance: " << connection.second << " units\n";
            }
        }
    }

    cout << "\nParking lots with no road connections:\n";
    bool anyUnconnected = false;
    for (const auto &parking : parkings)
    {
        if (neighbours.find(parking.first) == neighbours.end())
        {
            cout << parking.second.id << " (" << parking.second.name << ")\n";
            anyUnconnected = true;
        }
    }
    if (!anyUnconnected)
    {
        cout << "All parking lots are connected!\n";
    }
}

void ParkingGraph::saveToCSV(const string &filename)
{
    ofstream csvFile(filename);

    if (!csvFile.is_open())
    {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    // Write parking data
    csvFile << "Parking Data\n";
    csvFile << "ID,Code,Name,Location,Available_Spaces\n";
    for (const auto &parking : parkings)
    {
        csvFile << parking.second.id << ","
                << parking.second.code << ","
                << parking.second.name << ","
                << parking.second.location << ","
                << parking.second.available_spaces << "\n";
    }

    // Write road connections
    csvFile << "\nRoad Connections\n";
    csvFile << "From ID,From Name,To ID,To Name,Distance (units)\n";
    for (const auto &entry : neighbours)
    {
        for (const auto &connection : entry.second)
        {
            if (entry.first < connection.first)
            {
                csvFile << entry.first << ","
                        << parkings[entry.first].name << ","
                        << connection.first << ","
                        << parkings[connection.first].name << ","
                        << connection.second << "\n";
            }
        }
    }

    csvFile.close();
    cout << "\nData saved to " << filename << endl;
}

void ParkingGraph::saveToText(const string &filename)
{
    ofstream textFile(filename);

    if (!textFile.is_open())
    {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    textFile << "=== Parking Network Report ===\n\n";
    textFile << "Parking Lots in the network:\n";
    for (const auto &parking : parkings)
    {
        textFile << "ID: " << parking.second.id
                 << ", Code: " << parking.second.code
                 << ", Name: " << parking.second.name
                 << ", Location: " << parking.second.location
                 << ", Available Spaces: " << parking.second.available_spaces << endl;
    }

    textFile << "\nRoad connections between parking lots:\n";
    for (const auto &entry : neighbours)
    {
        textFile << "From " << entry.first << " (" << parkings[entry.first].name << ") to:\n";
        for (const auto &connection : entry.second)
        {
            if (entry.first < connection.first)
            {
                textFile << "  - " << connection.first << " (" << parkings[connection.first].name
                         << "), Distance: " << connection.second << " units\n";
            }
        }
    }

    textFile << "\nParking lots with no road connections:\n";
    bool anyUnconnected = false;
    for (const auto &parking : parkings)
    {
        if (neighbours.find(parking.first) == neighbours.end())
        {
            textFile << parking.second.id << " (" << parking.second.name << ")\n";
            anyUnconnected = true;
        }
    }
    if (!anyUnconnected)
    {
        textFile << "All parking lots are connected!\n";
    }

    textFile.close();
    cout << "Data saved to " << filename << endl;
}

bool ParkingGraph::loadFromCSV(const string &filename)
{
    ifstream csvFile(filename);
    if (!csvFile.is_open())
    {
        cerr << "Error: Could not open file " << filename << endl;
        return false;
    }

    // Clear existing data
    parkings.clear();
    neighbours.clear();

    string line;
    bool readingParkings = false;
    bool readingRoads = false;
    unordered_set<string> loadedParkingIds; // To track which parkings we've loaded

    while (getline(csvFile, line))
    {
        // Trim whitespace from the line
        line.erase(line.find_last_not_of(" \n\r\t") + 1);

        // Skip empty lines
        if (line.empty())
            continue;

        // Check section headers
        if (line == "Parking Data")
        {
            readingParkings = true;
            readingRoads = false;
            getline(csvFile, line); // Skip the column header line
            continue;
        }
        else if (line == "Road Connections")
        {
            readingParkings = false;
            readingRoads = true;
            getline(csvFile, line); // Skip the column header line
            continue;
        }

        if (readingParkings)
        {
            // Parse parking data line (ID,Code,Name,Location,Available_Spaces)
            vector<string> tokens;
            string token;
            istringstream tokenStream(line);

            while (getline(tokenStream, token, ','))
            {
                // Remove quotes if present
                if (!token.empty() && token.front() == '"' && token.back() == '"')
                {
                    token = token.substr(1, token.size() - 2);
                }
                tokens.push_back(token);
            }

            if (tokens.size() >= 5)
            {
                try
                {
                    string id = tokens[0];
                    string code = tokens[1];
                    string name = tokens[2];
                    string location = tokens[3];
                    int spaces = stoi(tokens[4]);

                    if (!addParking(id, code, name, location, spaces))
                    {
                        cerr << "Warning: Failed to add parking " << id << endl;
                    }
                    else
                    {
                        loadedParkingIds.insert(id);
                    }
                }
                catch (const exception &e)
                {
                    cerr << "Warning: Invalid data format in line: " << line
                         << " (" << e.what() << ")" << endl;
                }
            }
            else
            {
                cerr << "Warning: Incomplete parking data in line: " << line << endl;
            }
        }
        else if (readingRoads)
        {
            // Parse road connection line (From ID,From Name,To ID,To Name,Distance)
            vector<string> tokens;
            string token;
            istringstream tokenStream(line);

            while (getline(tokenStream, token, ','))
            {
                // Remove quotes if present
                if (!token.empty() && token.front() == '"' && token.back() == '"')
                {
                    token = token.substr(1, token.size() - 2);
                }
                tokens.push_back(token);
            }

            if (tokens.size() >= 5)
            {
                try
                {
                    string fromId = tokens[0];
                    string toId = tokens[2];
                    int distance = stoi(tokens[4]);

                    // Only add if both parkings exist
                    if (loadedParkingIds.count(fromId) && loadedParkingIds.count(toId))
                    {
                        if (!roadExists(fromId, toId))
                        {
                            neighbours[fromId].emplace_back(toId, distance);
                            neighbours[toId].emplace_back(fromId, distance);
                        }
                    }
                    else
                    {
                        cerr << "Warning: Missing parking for connection "
                             << fromId << " to " << toId << endl;
                    }
                }
                catch (const exception &e)
                {
                    cerr << "Warning: Invalid distance format in line: " << line
                         << " (" << e.what() << ")" << endl;
                }
            }
            else
            {
                cerr << "Warning: Incomplete road data in line: " << line << endl;
            }
        }
    }

    csvFile.close();

    if (parkings.empty())
    {
        cerr << "Warning: No parking data was loaded from the file." << endl;
        return false;
    }

    cout << "Successfully loaded " << parkings.size() << " parking lots and "
         << neighbours.size() << " connections." << endl;
    return true;
}
vector<Parking *> ParkingGraph::searchParking(const string &query)
{
    vector<Parking *> results;
    string lowerQuery = toLower(query);

    for (auto &entry : parkings)
    {
        Parking &p = entry.second;
        if (toLower(p.id).find(lowerQuery) != string::npos ||
            toLower(p.code).find(lowerQuery) != string::npos ||
            toLower(p.name).find(lowerQuery) != string::npos ||
            toLower(p.location).find(lowerQuery) != string::npos)
        {
            results.push_back(&p);
        }
    }
    return results;
}