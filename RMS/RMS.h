#ifndef RMS_H
#define RMS_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

class RoadManagementSystem {
private:
    std::vector<std::string> cities;  // Stores city names
    std::vector<std::vector<bool>> roadMatrix;  // Adjacency matrix for roads
    std::vector<std::vector<double>> budgetMatrix;  // Adjacency matrix for budgets
    int cityCount;  // Current number of cities

public:
    RoadManagementSystem();
    
    // Core functionality
    void addCities();
    void addRoad();
    void addBudget();
    void editCity();
    void searchCity();
    void displayCities();
    void displayRoads();
    void displayData();
    
    // Helper functions
    int getCityIndex(const std::string& cityName);
    bool cityExists(const std::string& cityName);
    bool roadExists(int city1, int city2);
    void resizeMatrices();
    void displayMenu();
};

#endif 