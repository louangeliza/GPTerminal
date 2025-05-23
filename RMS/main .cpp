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
