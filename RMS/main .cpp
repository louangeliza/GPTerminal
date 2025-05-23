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

void addCities(CitiesList& list, int maxCities = 100) {
    cout << "\nAdding New Cities... (up to " << maxCities << " cities)\n";
    
    // Ask how many cities the user wants to add
    int citiesToAdd;
    do {
        cout << "Enter number of cities to add: ";
        cin >> citiesToAdd;
    } while (citiesToAdd < 1 || citiesToAdd > maxCities);

    if (citiesToAdd == 0) { // If no cities were added, exit the function
        return;
    }

    for (int i = 0; i < citiesToAdd; ++i) {
        cout << "\nEnter city name: ";
        string name;
        cin >> name;
        nextId++;

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
    }
    
}
void addRoad(){
string fd, sd, r;
cout << "Enter first city: "; cin>> fd;
cout <<"Enter second city: "; cin>>sd;
cin.ignore();
cout <<"Road added between 1 and 2 ";

}
void connectHospitals() {
    string h1, h2, desc;
    cout << "Enter first Hospital ID: "; cin >> h1;
    cout << "Enter second Hospital ID: "; cin >> h2;
    cin.ignore();
    cout << "Enter distance between the hospitals: "; getline(cin, desc);

    if (hospitals.find(h1) == hospitals.end() || hospitals.find(h2) == hospitals.end()) {
        cout << "One or both Hospital IDs not found.\n";
        return;
    }

    auto alreadyExists = [](const vector<pair<string, string>>& conns, const string& id) {
        return any_of(conns.begin(), conns.end(), [&](const pair<string, string>& p) {
            return p.first == id;
        });
    };

    if (alreadyExists(graph[h1], h2) || alreadyExists(graph[h2], h1)) {
        cout << "Connection already exists.\n";
        return;
    }

    graph[h1].push_back({h2, desc});
    graph[h2].push_back({h1, desc});
    saveGraph();
    cout << "Connection added successfully.\n";
}
int main() {
    CitiesList cities;

    char choice;
    do {
        cout << "\n==== City and Road Management System ====\n";
        cout << "1. Add New City(ies)\n2. Add Road Between Cities\n3. Add Road Budget\n4. Edit City Name\n5. Search City by Index\n6. Display Cities\n7. Display Roads\n8. Show Recorded Data\n9. Exit\n";
        
        cin >> choice;
        
        switch (choice) {
            case '1':
                addCities(cities, 5); // Add a maximum of 5 cities when selecting option 1
                break;
            // case '2':
            //     addRoad(); // You'll need to implement this function
            //     break;
            // case '3':
            //     addBudget();
            //     break;
            // case '4':
            //     editCityName(); // You'll need to implement this function
            //     break;
            // case '5':
            //     searchCities();
            //     break;
            // case '6':
            //     displayCities();
            //     break;
            // case '7':
            //     displayRoads();
            //     break;
            // case '8':
            //     showData();
            //     break;
            case '9':
                exit(0);
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != '0');
    
    return 0;
}
