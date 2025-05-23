#include "ParkingGraph.h"
#include "Utility.h"
#include <iostream>

int main()
{
    ParkingGraph parkingSystem;
    Utility utils;
    int choice;

    cout << "=== Parking Management System ===\n";

    while (true)
    {
        cout << "\nMenu:\n";
        cout << "1. Add a parking lot\n";
        cout << "2. Add a road connection\n";
        cout << "3. Update parking information\n";
        cout << "4. Delete a parking lot\n";
        cout << "5. Delete a road connection\n";
        cout << "6. Get parking information\n";
        cout << "7. Get road information\n";
        cout << "8. View network summary\n";
        cout << "9. Search parking lots\n";
        cout << "10. Save data to files\n";
        cout << "11. Load data from CSV\n";
        cout << "12. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
        { // Add parking
            string id = utils.getStringInput("Enter parking ID: ");
            string code = utils.getStringInput("Enter parking code: ");
            string name = utils.getStringInput("Enter parking name: ");
            string location = utils.getStringInput("Enter parking location: ");
            int spaces = utils.getIntegerInput("Enter available spaces: ", 0);
            if (parkingSystem.addParking(id, code, name, location, spaces))
            {
                cout << "Parking added successfully!\n";
            }
            break;
        }
        case 2:
        { // Add road
            string id1 = utils.getStringInput("Enter first parking ID: ");
            string id2 = utils.getStringInput("Enter second parking ID: ");
            if (parkingSystem.addRoad(id1, id2))
            {
                cout << "Road connection added successfully!\n";
            }
            break;
        }
        case 3:
        { // Update parking
            string id = utils.getStringInput("Enter parking ID to update: ");
            string code = utils.getStringInput("Enter new code (leave blank to keep current): ");
            string name = utils.getStringInput("Enter new name (leave blank to keep current): ");
            string location = utils.getStringInput("Enter new location (leave blank to keep current): ");
            int spaces = utils.getIntegerInput("Enter new available spaces (-1 to keep current): ", -1);

            Parking *parking = parkingSystem.getParking(id);
            if (parking == nullptr)
                break;

            if (code.empty())
                code = parking->code;
            if (name.empty())
                name = parking->name;
            if (location.empty())
                location = parking->location;
            if (spaces == -1)
                spaces = parking->available_spaces;

            if (parkingSystem.updateParking(id, code, name, location, spaces))
            {
                cout << "Parking updated successfully!\n";
            }
            break;
        }
        case 4:
        { // Delete parking
            string id = utils.getStringInput("Enter parking ID to delete: ");
            if (parkingSystem.deleteParking(id))
            {
                cout << "Parking deleted successfully!\n";
            }
            break;
        }
        case 5:
        { // Delete road
            string id1 = utils.getStringInput("Enter first parking ID: ");
            string id2 = utils.getStringInput("Enter second parking ID: ");
            if (parkingSystem.deleteRoad(id1, id2))
            {
                cout << "Road connection deleted successfully!\n";
            }
            break;
        }
        case 6:
        { // Get parking
            string id = utils.getStringInput("Enter parking ID: ");
            Parking *parking = parkingSystem.getParking(id);
            if (parking)
            {
                parking->displayParking(parking);
            }
            else
            {
                cout << "Parking not found.\n";
            }
            break;
        }
        case 7:
        { // Get road
            string id1 = utils.getStringInput("Enter first parking ID: ");
            string id2 = utils.getStringInput("Enter second parking ID: ");
            int *distance = parkingSystem.getRoadDistance(id1, id2);
            if (distance == nullptr)
            {
                cout << "No road exists between these parking lots.\n";
            }
            else
            {
                cout << "Distance between " << id1 << " and " << id2
                     << " is " << *distance << " units.\n";
            }
            break;
        }
        case 8: // View summary
            parkingSystem.printGraph();
            break;
        case 9:
        { // Search
            string query = utils.getStringInput("Enter search term: ");
            vector<Parking *> results = parkingSystem.searchParking(query);
            if (results.empty())
            {
                cout << "No matching parking lots found.\n";
            }
            else
            {
                cout << "Search results:\n";
                for (Parking *p : results)
                {
                    p->displayParking(p);
                }
            }
            break;
        }
        case 10: // Save data
            parkingSystem.saveToCSV("parking_data.csv");
            parkingSystem.saveToText("parking_data.txt");
            break;
        case 11:
        { // Load from CSV
            string filename = utils.getStringInput("Enter CSV filename to load: ");
            if (parkingSystem.loadFromCSV(filename))
            {
                cout << "Data loaded successfully!\n";
            }
            break;
        }
        case 12: // Exit
            cout << "Exiting program. Goodbye!\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}