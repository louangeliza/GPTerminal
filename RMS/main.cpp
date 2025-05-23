#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <limits>
#include <sstream>

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

int findCityIndexByName(const string& name) {
    string lowerName = name;
    transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
    for (size_t i = 0; i < cities.size(); i++) {
        string cityName = cities[i].name;
        transform(cityName.begin(), cityName.end(), cityName.begin(), ::tolower);
        if (cityName == lowerName) return static_cast<int>(i);
    }
    return -1;
}

int addCityIfNotExists(const string& name) {
    if (name.empty()) return -1;
    int index = findCityIndexByName(name);
    if (index == -1) {
        City c;
        c.index = static_cast<int>(cities.size()) + 1;
        c.name = name;
        cities.push_back(c);
        return c.index - 1; // Return the new city's index (0-based)
    }
    return index;
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
        if (c.name.empty()) {
            cout << "City name cannot be empty. Skipping.\n";
            continue;
        }
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

    int i = addCityIfNotExists(c1);
    int j = addCityIfNotExists(c2);

    if (i != -1 && j != -1 && i != j) {
        roads[i][j] = roads[j][i] = 1;
        cout << "Road added between " << cities[i].name << " and " << cities[j].name << ".\n";
    } else {
        cout << "Same city entered. Road not added.\n";
    }
}

void addBudget() {
    string c1, c2;
    double budget;
    cout << "Enter the name of the first city: ";
    getline(cin, c1);
    cout << "Enter the name of the second city: ";
    getline(cin, c2);

    int i = addCityIfNotExists(c1);
    int j = addCityIfNotExists(c2);

    if (i != -1 && j != -1 && i != j) {
        if (roads[i][j] == 0) {
            roads[i][j] = roads[j][i] = 1;
            cout << "Road added between " << cities[i].name << " and " << cities[j].name << ".\n";
        }
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
        cout << "Same city entered. Budget not added.\n";
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
        if (newName.empty()) {
            cout << "City name cannot be empty.\n";
            return;
        }
        if (findCityIndexByName(newName) == -1) {
            cities[index - 1].name = newName;
            cout << "City updated successfully.\n";
        } else {
            cout << "New city name already exists.\n";
        }
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
    clearInputBuffer();
    if (index >= 1 && static_cast<size_t>(index) <= cities.size()) {
        cout << index << ": " << cities[index - 1].name << "\n";
    } else {
        cout << "City not found.\n";
    }
}

void displayCities() {
    if (cities.empty()) {
        cout << "\nNo cities to display.\n";
        return;
    }
    cout << "\nCities:\n";
    for (const auto& c : cities) {
        cout << c.index << ": " << c.name << "\n";
    }
}

void displayRoads() {
    if (cities.empty()) {
        cout << "\nNo cities or roads to display.\n";
        return;
    }
    cout << "\nRoads Adjacency Matrix:\n\t";
    for (const auto& c : cities)
        cout << left << setw(15) << c.name << "\t";
    cout << "\n";

    for (size_t i = 0; i < cities.size(); ++i) {
        cout << left << setw(15) << cities[i].name << "\t";
        for (size_t j = 0; j < cities.size(); ++j) {
            cout << roads[i][j] << "\t";
        }
        cout << "\n";
    }

    cout << "\nBudgets Adjacency Matrix:\n\t";
    for (const auto& c : cities)
        cout << left << setw(15) << c.name << "\t";
    cout << "\n";

    for (size_t i = 0; i < cities.size(); ++i) {
        cout << left << setw(15) << cities[i].name << "\t";
        for (size_t j = 0; j < cities.size(); ++j) {
            cout << fixed << setprecision(2) << budgets[i][j] << "\t";
        }
        cout << "\n";
    }
}

void displayAll() {
    displayCities();
    displayRoads();
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
    cout << "Data saved successfully.\n";
}

int main() {
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
        cout << "9. Exit\n";
        cout << "Enter your choice: ";

        string input;
        getline(cin, input);
        int choice;

        // Trim whitespace from input
        input.erase(0, input.find_first_not_of(" \t"));
        input.erase(input.find_last_not_of(" \t") + 1);

        if (input.empty()) {
            cout << "Input cannot be empty. Please enter a number.\n";
            continue;
        }

        // Try to convert input to integer
        try {
            choice = stoi(input);
        } catch (const invalid_argument&) {
            cout << "Invalid input. Please enter a number.\n";
            continue;
        } catch (const out_of_range&) {
            cout << "Input out of range. Please enter a valid number.\n";
            continue;
        }

        // Process valid choice
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
            default: cout << "Invalid choice. Please select a number between 1 and 9.\n"; break;
        }
    }
    return 0;
}
