#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <limits>

#define MAX 100
using namespace std;

struct City {
    int index;
    string name;
};

vector<City> cities;
int roads[MAX][MAX] = {0};
double budgets[MAX][MAX] = {0.0};

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int findCityIndexByName(string name) {
    transform(name.begin(), name.end(), name.begin(), ::tolower);
    for (size_t i = 0; i < cities.size(); i++) {
        string cityName = cities[i].name;
        transform(cityName.begin(), cityName.end(), cityName.begin(), ::tolower);
        if (cityName == name) return static_cast<int>(i);
    }
    return -1;
}

void addCities() {
    int count;
    cout << "Enter the number of cities to add: ";
    if (!(cin >> count) || count <= 0) {
        cout << "Invalid number.\n";
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    for (int i = 0; i < count; i++) {
        City c;
        c.index = static_cast<int>(cities.size()) + 1;
        cout << "Enter name for city " << c.index << ": ";
        getline(cin, c.name);

        if (findCityIndexByName(c.name) == -1) {
            cities.push_back(c);
        } else {
            cout << "City already exists. Skipping.\n";
        }
    }
}

void addRoad() {
    string c1, c2;
    cout << "Enter the name of the first city: ";
    getline(cin, c1);
    if (c1.empty()) {
        cout << "City name cannot be empty.\n";
        return;
    }

    cout << "Enter the name of the second city: ";
    getline(cin, c2);
    if (c2.empty()) {
        cout << "City name cannot be empty.\n";
        return;
    }

    int i = findCityIndexByName(c1);
    int j = findCityIndexByName(c2);

    if (i != -1 && j != -1 && i != j) {
        roads[i][j] = roads[j][i] = 1;
        cout << "Road added between " << cities[i].name << " and " << cities[j].name << ".\n";
    } else {
        cout << "Invalid cities or same city entered.\n";
    }
}

void addBudget() {
    string c1, c2;
    double budget;
    cout << "Enter the name of the first city: ";
    getline(cin, c1);
    cout << "Enter the name of the second city: ";
    getline(cin, c2);

    int i = findCityIndexByName(c1);
    int j = findCityIndexByName(c2);

    if (i != -1 && j != -1 && roads[i][j] == 1) {
        cout << "Enter the budget for the road: ";
        if (!(cin >> budget) || budget < 0) {
            cout << "Invalid budget.\n";
            clearInputBuffer();
            return;
        }
        clearInputBuffer();
        budgets[i][j] = budgets[j][i] = budget;
        cout << "Budget added between " << cities[i].name << " and " << cities[j].name << ".\n";
    } else {
        cout << "No road exists between the given cities.\n";
    }
}

void editCity() {
    int index;
    string newName;
    cout << "Enter the index of the city to edit: ";
    if (!(cin >> index)) {
        cout << "Invalid index.\n";
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    if (index >= 1 && static_cast<size_t>(index) <= cities.size()) {
        cout << "Enter new name for city: ";
        getline(cin, newName);
        cities[index - 1].name = newName;
        cout << "City updated successfully.\n";
    } else {
        cout << "Invalid city index.\n";
    }
}

void searchCity() {
    int index;
    cout << "Enter city index to search: ";
    if (!(cin >> index)) {
        cout << "Invalid input.\n";
        clearInputBuffer();
        return;
    }
    if (index >= 1 && static_cast<size_t>(index) <= cities.size()) {
        cout << index << ": " << cities[index - 1].name << "\n";
    } else {
        cout << "City not found.\n";
    }
}

void displayCities() {
    cout << "\nCities:\n";
    for (const auto& c : cities) {
        cout << c.index << ": " << c.name << "\n";
    }
}

void displayRoads() {
    cout << "Roads Adjacency Matrix:\n\t";
    for (const auto& c : cities)
        cout << c.name << "\t";
    cout << "\n";

    for (size_t i = 0; i < cities.size(); ++i) {
        cout << cities[i].name << "\t";
        for (size_t j = 0; j < cities.size(); ++j) {
            cout << roads[i][j] << "\t";
        }
        cout << "\n";
    }
}

cout << "\nBudgets Adjacency Matrix:\n\t";
for (const auto& c : cities)
    cout << c.name << "\t";
cout << "\n";

for (size_t i = 0; i < cities.size(); ++i) {
    cout << cities[i].name << "\t";
    for (size_t j = 0; j < cities.size(); ++j) {
        cout << fixed << setprecision(2) << budgets[i][j] << "\t";
    }
    cout << "\n";
}

void saveToFile() {
    ofstream cityFile("cities.txt");
    if (!cityFile) {
        cerr << "Error opening cities.txt\n";
        return;
    }

    cityFile << left << setw(8) << "Index" << "City_Name\n";
    for (const auto& c : cities) {
        cityFile << left << setw(8) << c.index << c.name << "\n";
    }
    cityFile.close();

    ofstream roadFile("roads.txt");
    if (!roadFile) {
        cerr << "Error opening roads.txt\n";
        return;
    }

    roadFile << left << setw(5) << "#" << setw(25) << "Road" << "Budget\n";
    int count = 1;
    for (size_t i = 0; i < cities.size(); ++i) {
        for (size_t j = i + 1; j < cities.size(); ++j) {
            if (roads[i][j]) {
                string roadName = cities[i].name + "-" + cities[j].name;
                roadFile << left << setw(5) << (count++) << setw(25) << roadName << fixed << setprecision(2) << budgets[i][j] << "\n";
            }
        }
    }
    roadFile.close();
}

int main() {
    int choice;
    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Add new city(ies)\n";
        cout << "2. Add roads between cities\n";
        cout << "3. Add the budget for roads\n";
        cout << "4. Edit city\n";
        cout << "5. Search for a city using its index\n";
        cout << "6. Display cities\n";
        cout << "7. Display roads\n";
        cout << "8. Display recorded data on console\n";
        cout << "9. Save and Exit\n";
        cout << "Enter your choice: ";
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: addCities(); break;
            case 2: addRoad(); break;
            case 3: addBudget(); break;
            case 4: editCity(); break;
            case 5: searchCity(); break;
            case 6: displayCities(); break;
            case 7: displayRoads(); break;
            case 8: displayAll(); break;
            case 9: saveToFile(); return 0;
            default: cout << "Invalid choice.\n"; break;
        }
    }
}
