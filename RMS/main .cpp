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

int nextId = 1; // Starting ID for new cities
    
struct CitiesList {
    vector<City> cities; // This is a local variable, so it's only accessible within this function
};

void addCities(CitiesList& list) {
    cout << "\nAdding New Cities(ies): ";
    
    while (true) { 
        
        cout << "Enter city name: ";
        string name;
        cin >> name;
        
        if (name.empty()) {
            cout << "City name cannot be empty. Please try again.\n";
            continue;
        }
        
        // Validate the ID to ensure uniqueness
        bool unique = true;
        for (const City& c : list.cities) {
            if (c.id == nextId) {
                unique = false;
                break;
            }
        }
        if (!unique) {
            cout << "ID already exists. Please choose a different number.\n";
            continue;
        }
        
        nextId++;
        list.cities.push_back({nextId, name});
        
        cout << "\nCities added successfully!\n";
    }
}

int main() {
    CitiesList cities; // This is a local variable, so it's only accessible within this function
    
    char choice;
    do {
        cout << "\n==== City and Road Management System ====\n";
        cout << "1. Add New City(ies)\n2. Add Road Between Cities\n3. Add Road Budget\n4. Edit City Name\n5. Search City by Index\n6. Display Cities\n7. Display Roads\n8. Show Recorded Data\n9. Exit\n";
        
        cin >> choice;
        
        switch (choice) {
            case '1':
                addCities(cities); // Pass the cities variable to addCities
                break;
            // case '2':
            //     addRoad();
            //     break;
            // case '3':
            //     addBudget();
            //     break;
            // Continue with other cases...
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != '0');
    
    return 0;
}
