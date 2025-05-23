#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <limits>
#include <algorithm>
#include <utility>
#include <iomanip>

using namespace std;

// ====================== COLOR DEFINITIONS ======================
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"

// ====================== UI HELPER FUNCTIONS ======================
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void displayHeader(const string &title)
{
    clearScreen();
    cout << BOLD << CYAN << "========================================\n";
    cout << "  HOSPITAL NETWORK MANAGEMENT SYSTEM\n";
    cout << "========================================\n"
         << RESET;
    cout << BOLD << ">> " << title << "\n";
    cout << CYAN << "----------------------------------------\n"
         << RESET;
}

void pressEnterToContinue()
{
    cout << YELLOW << "\nPress Enter to continue..." << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// ====================== HOSPITAL CLASS ======================
class Hospital
{
public:
    string id;
    string name;
    string location;
    int employees;

    Hospital() : id(""), name(""), location(""), employees(0) {}
    Hospital(string i, string n, string loc, int emp)
        : id(i), name(n), location(loc), employees(emp) {}
};

// ====================== ROAD CONNECTION CLASS ======================
class Road
{
public:
    string hospital1;
    string hospital2;
    int distance;

    Road(string h1, string h2, int dist)
        : hospital1(h1), hospital2(h2), distance(dist) {}
};

// ====================== HOSPITAL NETWORK MANAGER ======================
class HospitalNetwork
{
private:
    map<string, Hospital> hospitals;
    vector<Road> roads;

    bool hospitalExists(const string &id) const
    {
        return hospitals.find(id) != hospitals.end();
    }

    bool roadExists(const string &id1, const string &id2) const
    {
        for (const Road &road : roads)
        {
            if ((road.hospital1 == id1 && road.hospital2 == id2) ||
                (road.hospital1 == id2 && road.hospital2 == id1))
            {
                return true;
            }
        }
        return false;
    }

    Road *getRoad(const string &id1, const string &id2)
    {
        for (Road &road : roads)
        {
            if ((road.hospital1 == id1 && road.hospital2 == id2) ||
                (road.hospital1 == id2 && road.hospital2 == id1))
            {
                return &road;
            }
        }
        return nullptr;
    }

public:
    bool addHospital(const string &id, const string &name, const string &location, int employees)
    {
        if (hospitalExists(id))
        {
            cout << RED << "❌ Error: Hospital with ID " << id << " already exists.\n"
                 << RESET;
            return false;
        }
        hospitals.emplace(id, Hospital(id, name, location, employees));
        cout << GREEN << "✅ Hospital added successfully.\n"
             << RESET;
        return true;
    }

    bool addRoad(const string &id1, const string &id2, int distance)
    {
        if (!hospitalExists(id1))
        {
            cout << RED << "❌ Error: Hospital " << id1 << " doesn't exist.\n"
                 << RESET;
            return false;
        }
        if (!hospitalExists(id2))
        {
            cout << RED << "❌ Error: Hospital " << id2 << " doesn't exist.\n"
                 << RESET;
            return false;
        }
        if (roadExists(id1, id2))
        {
            cout << RED << "❌ Error: Road already exists between these hospitals.\n"
                 << RESET;
            return false;
        }
        if (id1 == id2)
        {
            cout << RED << "❌ Error: Cannot connect a hospital to itself.\n"
                 << RESET;
            return false;
        }
        if (distance <= 0)
        {
            cout << RED << "❌ Error: Distance must be positive.\n"
                 << RESET;
            return false;
        }

        roads.emplace_back(id1, id2, distance);
        cout << GREEN << "✅ Road added successfully.\n"
             << RESET;
        return true;
    }

    bool updateHospital(const string &id, const string &newName, const string &newLocation, int newEmployees)
    {
        if (!hospitalExists(id))
        {
            cout << RED << "❌ Error: Hospital " << id << " doesn't exist.\n"
                 << RESET;
            return false;
        }
        hospitals[id].name = newName;
        hospitals[id].location = newLocation;
        hospitals[id].employees = newEmployees;
        cout << GREEN << "✅ Hospital updated successfully.\n"
             << RESET;
        return true;
    }

    bool deleteHospital(const string &id)
    {
        if (!hospitalExists(id))
        {
            cout << RED << "❌ Error: Hospital " << id << " doesn't exist.\n"
                 << RESET;
            return false;
        }

        // Remove all roads connected to this hospital
        roads.erase(remove_if(roads.begin(), roads.end(),
                              [&id](const Road &r)
                              {
                                  return r.hospital1 == id || r.hospital2 == id;
                              }),
                    roads.end());

        hospitals.erase(id);
        cout << GREEN << "✅ Hospital deleted successfully.\n"
             << RESET;
        return true;
    }

    bool deleteRoad(const string &id1, const string &id2)
    {
        if (!roadExists(id1, id2))
        {
            cout << RED << "❌ Error: No road exists between these hospitals.\n"
                 << RESET;
            return false;
        }

        roads.erase(remove_if(roads.begin(), roads.end(),
                              [&id1, &id2](const Road &r)
                              {
                                  return (r.hospital1 == id1 && r.hospital2 == id2) ||
                                         (r.hospital1 == id2 && r.hospital2 == id1);
                              }),
                    roads.end());

        cout << GREEN << "✅ Road deleted successfully.\n"
             << RESET;
        return true;
    }

    Hospital *getHospital(const string &id)
    {
        if (!hospitalExists(id))
            return nullptr;
        return &hospitals[id];
    }

    int *getRoadDistance(const string &id1, const string &id2)
    {
        Road *road = getRoad(id1, id2);
        if (road == nullptr)
            return nullptr;
        return &road->distance;
    }

    void printSummary() const
    {
        displayHeader("Network Summary");

        cout << MAGENTA << "\n🏥 HOSPITALS (" << hospitals.size() << "):\n"
             << RESET;
        cout << CYAN << "----------------------------------------\n"
             << RESET;
        cout << BOLD << left << setw(8) << "ID" << setw(20) << "Name"
             << setw(15) << "Location" << "Employees\n"
             << RESET;
        cout << CYAN << "----------------------------------------\n"
             << RESET;

        for (const auto &h : hospitals)
        {
            cout << setw(8) << h.second.id << setw(20) << h.second.name
                 << setw(15) << h.second.location << h.second.employees << endl;
        }

        cout << BLUE << "\n🛣️  ROAD CONNECTIONS (" << roads.size() << "):\n"
             << RESET;
        cout << CYAN << "----------------------------------------\n"
             << RESET;
        cout << BOLD << left << setw(20) << "Hospital A" << setw(20) << "Hospital B"
             << "Distance (km)\n"
             << RESET;
        cout << CYAN << "----------------------------------------\n"
             << RESET;

        for (const Road &r : roads)
        {
            cout << setw(20) << hospitals.at(r.hospital1).name + " (" + r.hospital1 + ")"
                 << setw(20) << hospitals.at(r.hospital2).name + " (" + r.hospital2 + ")"
                 << r.distance << endl;
        }

        // Find unconnected hospitals
        vector<string> unconnected;
        for (const auto &h : hospitals)
        {
            bool connected = false;
            for (const Road &r : roads)
            {
                if (r.hospital1 == h.first || r.hospital2 == h.first)
                {
                    connected = true;
                    break;
                }
            }
            if (!connected)
                unconnected.push_back(h.first);
        }

        cout << YELLOW << "\n⚠️  UNCONNECTED HOSPITALS (" << unconnected.size() << "):\n"
             << RESET;
        cout << CYAN << "----------------------------------------\n"
             << RESET;
        for (const string &id : unconnected)
        {
            cout << "• " << hospitals.at(id).name << " (" << id << ")\n";
        }
    }

    void saveData() const
    {
        ofstream csvFile("hospital_data.csv");
        ofstream textFile("hospital_data.txt");

        // Save to CSV
        csvFile << "Hospital Data\nID,Name,Location,Employees\n";
        for (const auto &h : hospitals)
        {
            csvFile << h.second.id << "," << h.second.name << ","
                    << h.second.location << "," << h.second.employees << "\n";
        }
        csvFile << "\nRoad Connections\nHospital1 ID,Hospital1 Name,Hospital2 ID,Hospital2 Name,Distance\n";
        for (const Road &r : roads)
        {
            csvFile << r.hospital1 << "," << hospitals.at(r.hospital1).name << ","
                    << r.hospital2 << "," << hospitals.at(r.hospital2).name << ","
                    << r.distance << "\n";
        }
        csvFile.close();

        // Save to text
        textFile << "=== HOSPITAL NETWORK REPORT ===\n\n";
        textFile << "HOSPITALS (" << hospitals.size() << "):\n";
        for (const auto &h : hospitals)
        {
            textFile << "ID: " << h.second.id << ", Name: " << h.second.name
                     << ", Location: " << h.second.location
                     << ", Employees: " << h.second.employees << "\n";
        }
        textFile << "\nROAD CONNECTIONS (" << roads.size() << "):\n";
        for (const Road &r : roads)
        {
            textFile << hospitals.at(r.hospital1).name << " (" << r.hospital1 << ") <--> "
                     << hospitals.at(r.hospital2).name << " (" << r.hospital2 << "), "
                     << r.distance << " km\n";
        }
        textFile.close();

        cout << GREEN << "✅ Data saved to hospital_data.csv and hospital_data.txt\n"
             << RESET;
    }
};

// ====================== INPUT HELPER FUNCTIONS ======================
string getStringInput(const string &prompt)
{
    string input;
    cout << BOLD << prompt << RESET;
    getline(cin, input);
    return input;
}

int getIntegerInput(const string &prompt, int min = 1)
{
    int value;
    while (true)
    {
        cout << BOLD << prompt << RESET;
        cin >> value;
        if (cin.fail() || value < min)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "Invalid input. Please enter a number >= " << min << ".\n"
                 << RESET;
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

void displayHospitalDetails(const Hospital *h)
{
    if (!h)
    {
        cout << RED << "❌ Hospital not found.\n"
             << RESET;
        return;
    }
    cout << MAGENTA << "\n🏥 HOSPITAL DETAILS:\n"
         << RESET;
    cout << CYAN << "----------------------------------------\n"
         << RESET;
    cout << left << setw(15) << "ID:" << h->id << "\n";
    cout << setw(15) << "Name:" << h->name << "\n";
    cout << setw(15) << "Location:" << h->location << "\n";
    cout << setw(15) << "Employees:" << h->employees << "\n";
    cout << CYAN << "----------------------------------------\n"
         << RESET;
}

void displayRoadDetails(const string &id1, const string &id2, int distance)
{
    cout << BLUE << "\n🛣️  ROAD DETAILS:\n"
         << RESET;
    cout << CYAN << "----------------------------------------\n"
         << RESET;
    cout << "From Hospital ID: " << id1 << "\n";
    cout << "To Hospital ID:   " << id2 << "\n";
    cout << "Distance:         " << distance << " km\n";
    cout << CYAN << "----------------------------------------\n"
         << RESET;
}

// ====================== MAIN MENU ======================
void showMainMenu()
{
    displayHeader("Main Menu");
    cout << BOLD << "1. " << RESET << "Add Hospital\n";
    cout << BOLD << "2. " << RESET << "Add Road Connection\n";
    cout << BOLD << "3. " << RESET << "Update Hospital\n";
    cout << BOLD << "4. " << RESET << "Delete Hospital\n";
    cout << BOLD << "5. " << RESET << "Delete Road Connection\n";
    cout << BOLD << "6. " << RESET << "View Hospital Details\n";
    cout << BOLD << "7. " << RESET << "View Road Details\n";
    cout << BOLD << "8. " << RESET << "Network Summary\n";
    cout << BOLD << "9. " << RESET << "Save Data to Files\n";
    cout << BOLD << "10. " << RESET << "Exit\n";
    cout << CYAN << "----------------------------------------\n"
         << RESET;
}

int main()
{
    HospitalNetwork network;
    int choice;

    while (true)
    {
        showMainMenu();
        cout << BOLD << "Enter your choice (1-10): " << RESET;
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
        { // Add Hospital
            displayHeader("Add New Hospital");
            string id = getStringInput("Enter hospital ID: ");
            string name = getStringInput("Enter hospital name: ");
            string loc = getStringInput("Enter location: ");
            int emp = getIntegerInput("Enter employee count: ");
            network.addHospital(id, name, loc, emp);
            pressEnterToContinue();
            break;
        }
        case 2:
        { // Add Road
            displayHeader("Add Road Connection");
            string id1 = getStringInput("Enter first hospital ID: ");
            string id2 = getStringInput("Enter second hospital ID: ");
            int dist = getIntegerInput("Enter distance (km): ");
            network.addRoad(id1, id2, dist);
            pressEnterToContinue();
            break;
        }
        case 3:
        { // Update Hospital
            displayHeader("Update Hospital");
            string id = getStringInput("Enter hospital ID to update: ");
            string name = getStringInput("Enter new name (blank to keep current): ");
            string loc = getStringInput("Enter new location (blank to keep current): ");
            int emp = getIntegerInput("Enter new employee count (0 to keep current): ", 0);

            Hospital *h = network.getHospital(id);
            if (!h)
            {
                pressEnterToContinue();
                break;
            }

            if (name.empty())
                name = h->name;
            if (loc.empty())
                loc = h->location;
            if (emp == 0)
                emp = h->employees;

            network.updateHospital(id, name, loc, emp);
            pressEnterToContinue();
            break;
        }
        case 4:
        { // Delete Hospital
            displayHeader("Delete Hospital");
            string id = getStringInput("Enter hospital ID to delete: ");
            network.deleteHospital(id);
            pressEnterToContinue();
            break;
        }
        case 5:
        { // Delete Road
            displayHeader("Delete Road Connection");
            string id1 = getStringInput("Enter first hospital ID: ");
            string id2 = getStringInput("Enter second hospital ID: ");
            network.deleteRoad(id1, id2);
            pressEnterToContinue();
            break;
        }
        case 6:
        { // View Hospital
            displayHeader("View Hospital Details");
            string id = getStringInput("Enter hospital ID: ");
            displayHospitalDetails(network.getHospital(id));
            pressEnterToContinue();
            break;
        }
        case 7:
        { // View Road
            displayHeader("View Road Details");
            string id1 = getStringInput("Enter first hospital ID: ");
            string id2 = getStringInput("Enter second hospital ID: ");
            int *dist = network.getRoadDistance(id1, id2);
            if (dist)
            {
                displayRoadDetails(id1, id2, *dist);
            }
            else
            {
                cout << RED << "❌ No road found between these hospitals.\n"
                     << RESET;
            }
            pressEnterToContinue();
            break;
        }
        case 8: // Network Summary
            network.printSummary();
            pressEnterToContinue();
            break;
        case 9: // Save Data
            displayHeader("Save Data to Files");
            network.saveData();
            pressEnterToContinue();
            break;
        case 10: // Exit
            displayHeader("Exit System");
            cout << GREEN << "Thank you for using Hospital Network Management System.\n";
            cout << "Goodbye!\n"
                 << RESET;
            return 0;
        default:
            cout << RED << "❌ Invalid choice. Please try again.\n"
                 << RESET;
            pressEnterToContinue();
        }
    }
}