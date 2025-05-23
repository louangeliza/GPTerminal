#include "Parking.h"
#include <iostream>

using namespace std;

void Parking::displayParking(const Parking *parking)
{
    if (parking == nullptr)
    {
        cout << "Parking not found.\n";
        return;
    }
    cout << "\nParking Details:\n";
    cout << "ID: " << parking->id << "\n";
    cout << "Code: " << parking->code << "\n";
    cout << "Name: " << parking->name << "\n";
    cout << "Location: " << parking->location << "\n";
    cout << "Available Spaces: " << parking->available_spaces << "\n";
    cout << "----------------------------------------\n";
}