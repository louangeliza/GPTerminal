#include <iostream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

class RoadManagementSystem {
private:
    vector<string> cities;
    vector<vector<bool>> roadMatrix;
    vector<vector<double>> budgetMatrix;
    int cityCount;

public:
    RoadManagementSystem() : cityCount(0) {}

    void addCities() {
        string cityName;
        cout << "Enter city name(s) (type 'done' when finished):\n";
        while (true) {
            cout << "City name: ";
            cin >> cityName;
            if (cityName == "done") break;
            
            cities.push_back(cityName);
            cityCount++;
            
            // Resize matrices
            roadMatrix.resize(cityCount, vector<bool>(cityCount, false));
            budgetMatrix.resize(cityCount, vector<double>(cityCount, 0.0));
            
            cout << "City added with index: " << cityCount << endl;
        }
    }

    void addRoad() {
        string city1, city2;
        cout << "Enter first city name: ";
        cin >> city1;
        cout << "Enter second city name: ";
        cin >> city2;

        int index1 = -1, index2 = -1;
        for (int i = 0; i < cities.size(); i++) {
            if (cities[i] == city1) index1 = i;
            if (cities[i] == city2) index2 = i;
        }

        if (index1 == -1 || index2 == -1) {
            cout << "Error: One or both cities do not exist!\n";
            return;
        }

        roadMatrix[index1][index2] = true;
        roadMatrix[index2][index1] = true;
        cout << "Road added successfully!\n";
    }

    void addBudget() {
        string city1, city2;
        cout << "Enter first city name: ";
        cin >> city1;
        cout << "Enter second city name: ";
        cin >> city2;

        int index1 = -1, index2 = -1;
        for (int i = 0; i < cities.size(); i++) {
            if (cities[i] == city1) index1 = i;
            if (cities[i] == city2) index2 = i;
        }

        if (index1 == -1 || index2 == -1) {
            cout << "Error: One or both cities do not exist!\n";
            return;
        }

        if (!roadMatrix[index1][index2]) {
            cout << "Error: No road exists between these cities!\n";
            return;
        }

        double budget;
        cout << "Enter budget for the road: ";
        cin >> budget;
        budgetMatrix[index1][index2] = budget;
        budgetMatrix[index2][index1] = budget;
        cout << "Budget added successfully!\n";
    }

    void editCity() {
        int index;
        cout << "Enter city index: ";
        cin >> index;
        
        if (index < 1 || index > cityCount) {
            cout << "Error: Invalid city index!\n";
            return;
        }

        string newName;
        cout << "Enter new city name: ";
        cin >> newName;
        cities[index - 1] = newName;
        cout << "City name updated successfully!\n";
    }

    void searchCity() {
        int index;
        cout << "Enter city index: ";
        cin >> index;
        
        if (index < 1 || index > cityCount) {
            cout << "Error: Invalid city index!\n";
            return;
        }

        cout << "City name: " << cities[index - 1] << endl;
    }

    void displayCities() {
        cout << "\nList of Cities:\n";
        for (int i = 0; i < cities.size(); i++) {
            cout << i + 1 << ". " << cities[i] << endl;
        }
    }

    void displayRoads() {
        cout << "\nRoad Connections:\n";
        for (int i = 0; i < cityCount; i++) {
            for (int j = i + 1; j < cityCount; j++) {
                if (roadMatrix[i][j]) {
                    cout << cities[i] << " <-> " << cities[j] << endl;
                }
            }
        }
    }

    void displayData() {
        cout << "\nRoad Matrix:\n";
        cout << "  ";
        for (int i = 0; i < cityCount; i++) {
            cout << cities[i] << " ";
        }
        cout << "\n";
        
        for (int i = 0; i < cityCount; i++) {
            cout << cities[i] << " ";
            for (int j = 0; j < cityCount; j++) {
                cout << roadMatrix[i][j] << " ";
            }
            cout << "\n";
        }

        cout << "\nBudget Matrix:\n";
        cout << "  ";
        for (int i = 0; i < cityCount; i++) {
            cout << cities[i] << " ";
        }
        cout << "\n";
        
        for (int i = 0; i < cityCount; i++) {
            cout << cities[i] << " ";
            for (int j = 0; j < cityCount; j++) {
                cout << budgetMatrix[i][j] << " ";
            }
            cout << "\n";
        }
    }
};

int main() {
    RoadManagementSystem rms;
    int choice;

    while (true) {
        cout << "\n=== Road Management System ===\n";
        cout << "1. Add New City(ies)\n";
        cout << "2. Add Road Between Cities\n";
        cout << "3. Add The Budget For Roads\n";
        cout << "4. Edit City\n";
        cout << "5. Search For A City Using Its Index\n";
        cout << "6. Display Cities\n";
        cout << "7. Display Roads\n";
        cout << "8. Display Recorded Data On Console\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        
        cin >> choice;

        switch (choice) {
            case 1:
                rms.addCities();
                break;
            case 2:
                rms.addRoad();
                break;
            case 3:
                rms.addBudget();
                break;
            case 4:
                rms.editCity();
                break;
            case 5:
                rms.searchCity();
                break;
            case 6:
                rms.displayCities();
                break;
            case 7:
                rms.displayRoads();
                break;
            case 8:
                rms.displayData();
                break;
            case 9:
                cout << "Thank you for using the Road Management System!\n";
                return 0;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }

    return 0;
} 