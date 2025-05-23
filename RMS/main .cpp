#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <regex>
using namespace std;
struct City {
    int id;
    string name;
};
map<string, City> cities;
map<string, vector<pair<string, string>>> graph;
const string CITIES_FILE = "cities.txt";

void loadRoads(){
    cities.clear();
    ifstream file(CITIES_FILE);
    string line;
    getline(file,line);
    while(getline(file,line)){
        stringstream ss(line);
        string index,name;
    }
}

int main() {
    char choice;
    do {
        cout << "\n==== City and Road Management System ====\n";
        cout << "1. Add New City(ies)\n2. Add Road Between Cities\n3. Add Road Budget\n4. Edit City Name\n5. Search City by Index\n6. Display Cities\n7. Display Roads\n8. Show Recorded Data\n9. Exit\n";
        
        cin >> choice;
        
        switch (choice) {
            case '1':
                addCities();
                break;
            case '2':
                addRoad();
                break;
            case '3':
                addBudget();
                break;
            // Continue with other cases...
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != '0');
    
    return 0;
}
void addCities() {
    int nextId = 1; // Starting ID for new cities
    
    cout << "\nAdding New City(ies): ";
    bool added = false;
    
    while (!added) {
        char c = '\0';
        cin >> c;
        
        if (c == '\n' || c == ' ') {
            addSingleCity(nextId);
            nextId++;
            added = true;
        } else if (isdigit(c)) {
            string name;
            nextId = stoi(name);
            // Handle validation here
            addSingleCity(nextId);
            nextId++;
        }
    }
}
void addCities() {
    int nextId = 1; // Starting ID for new cities
    
    cout << "\nAdding New City(ies): ";
    bool added = false;
    
    while (!added) {
        char c = '\0';
        cin >> c;
        
        if (c == '\n' || c == ' ') {
            addSingleCity(nextId);
            nextId++;
            added = true;
        } else if (isdigit(c)) {
            string name;
            nextId = stoi(name);
            // Handle validation here
            addSingleCity(nextId);
            nextId++;
        }
    }
}
void addSingleCity(int id) {
    struct City newCity = {id, "Enter city name"};
    
    cout << "\nCity ID: " << id << endl;
    cout << "City Name: ";
    string name;
    cin >> name;
    if (name.empty()) {
        cout << "City name cannot be empty. Please try again.\n";
        return;
    }
    newCity.name = name;
    
    // Validate the ID to ensure uniqueness
    bool unique = true;
    for (const City& c : cities) {
        if (c.id == id) {
            unique = false;
            break;
        }
    }
    if (!unique) {
        cout << "ID already exists. Please choose a different number.\n";
        return;
    }
    
    // Add to the list
    cities.push_back(newCity);
    saveCities();
    
    displayCities();
}
void displayCities() {
    cout << "\nList of Cities:\n";
    for (const City& city : cities) {
        cout << "ID: " << city.id << ", Name: " << city.name << endl;
    }
}
void addRoad() {
    int fromId, toId;
    cout << "\nAdding Road: Enter from ID and to ID (or '0' to exit): ";
    while ((cin >> fromId) && (fromId != 0)) {
        char c = '\0';
        do {
            cin >> toId;
            if (toId == '0') break;
            
            // Validate both IDs
            bool valid = true;
            for (const City& city : cities) {
                if (city.id == fromId || city.id == toId) {
                    valid = false;
                    break;
                }
            }
            
            if (!valid) {
                cout << "City ID not found. Please enter existing IDs.\n";
                continue;
            }
            
            // Add the road
            Connection newConnection{fromId, toId, 0.0, "Enter distance in km"};
            roads.push_back(newConnection);
            addBudget(fromId, toId, newConnection);
            
            cout << "Road added between ID " << fromId << " and " << toId << endl;
        } while (true);
    }
}
void addBudget(int fromId, int toId, Connection& connection) {
    double budget;
    cout << "\nEnter road budget: ";
    cin >> budget;
    
    // Check if the road already exists between these IDs
    bool roadExists = false;
    for (const Connection& c : roads) {
        if (c.fromID == fromId && c.toID == toId || 
            c.fromID == toId && c.toID == fromId) {
            roadExists = true;
            break;
        }
    }
    
    if (roadExists) {
        cout << "Road between ID " << fromId << " and " << toId 
              << " already exists. Please update the budget.\n";
        return;
    } else {
        // Find the existing connection to modify
        Connection& foundConnection = roads[0];
        for (size_t i = 1; i < roads.size(); ++i) {
            if (foundConnection.fromID == fromId && 
                foundConnection.toID == toId || 
                foundConnection.fromID == toId && 
                foundConnection.toID == fromId) {
                foundConnection.budget = budget;
                saveRoads();
                cout << "Budget updated successfully.\n";
                return;
            }
        }
    }
    
    // Add the new connection
    connection.budget = budget;
    saveRoads();
    addConnection(connection);
}
void displayRoads() {
    if (roads.empty()) {
        cout << "\nNo roads have been added yet.\n";
        return;
    }
    
    cout << "\nList of Roads:\n";
    for (const Connection& road : roads) {
        // Find the corresponding cities and budget details
        City fromCity = getCity(road.fromID);
        City toCity = getCity(road.toID);
        cout << "From ID: " << fromCity.id << ", Name: " << fromCity.name 
              << "\nTo ID: " << toCity.id << ", Name: " << toCity.name 
              << "\nDistance: " << road.distance << "km, Time: " << road.time 
              << " minutes\nDescription: " << road.description
              << endl;
    }
}
