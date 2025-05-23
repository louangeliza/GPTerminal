#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <limits>
#include <algorithm>
#include <cctype>
#include <sstream>
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

// ====================== HELPER FUNCTIONS ======================
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

// ====================== GRAPH CLASS ======================
class HospitalGraph
{
private:
    map<string, vector<pair<string, int>>> adjacencyList;
    map<string, Hospital> hospitals;

    string toLower(const string &s)
    {
        string lower = s;
        transform(lower.begin(), lower.end(), lower.begin(),
                  [](unsigned char c)
                  { return tolower(c); });
        return lower;
    }

public:
    bool hospitalExists(const string &id)
    {
        return hospitals.find(id) != hospitals.end();
    }

    bool roadExists(const string &id1, const string &id2)
    {
        if (!hospitalExists(id1) || !hospitalExists(id2))
            return false;

        for (const auto &conn : adjacencyList[id1])
        {
            if (conn.first == id2)
                return true;
        }
        return false;
    }

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

        adjacencyList[id1].emplace_back(id2, distance);
        adjacencyList[id2].emplace_back(id1, distance);
        cout << GREEN << "✅ Road connection added successfully.\n"
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

        for (auto &conn : adjacencyList[id])
        {
            string otherId = conn.first;
            auto &otherConns = adjacencyList[otherId];
            otherConns.erase(remove_if(otherConns.begin(), otherConns.end(),
                                       [&id](const pair<string, int> &p)
                                       { return p.first == id; }),
                             otherConns.end());
        }
        adjacencyList.erase(id);
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

        auto &conns1 = adjacencyList[id1];
        conns1.erase(remove_if(conns1.begin(), conns1.end(),
                               [&id2](const pair<string, int> &p)
                               { return p.first == id2; }),
                     conns1.end());

        auto &conns2 = adjacencyList[id2];
        conns2.erase(remove_if(conns2.begin(), conns2.end(),
                               [&id1](const pair<string, int> &p)
                               { return p.first == id1; }),
                     conns2.end());

        cout << GREEN << "✅ Road connection deleted successfully.\n"
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
        if (!roadExists(id1, id2))
            return nullptr;

        for (auto &conn : adjacencyList[id1])
        {
            if (conn.first == id2)
            {
                return &conn.second;
            }
        }
        return nullptr;
    }

    vector<Hospital *> searchHospitals(const string &query)
    {
        vector<Hospital *> results;
        string lowerQuery = toLower(query);

        for (auto &hospital : hospitals)
        {
            string lowerName = toLower(hospital.second.name);
            string lowerLocation = toLower(hospital.second.location);

            if (lowerName.find(lowerQuery) != string::npos ||
                lowerLocation.find(lowerQuery) != string::npos)
            {
                results.push_back(&hospital.second);
            }
        }
        return results;
    }

    void generateGraphvizDOT(const string &filename)
    {
        ofstream dotFile(filename);

        if (!dotFile.is_open())
        {
            cerr << RED << "❌ Error: Could not open file " << filename << RESET << endl;
            return;
        }

        dotFile << "graph HospitalNetwork {\n";
        dotFile << "  node [shape=box, style=filled, fillcolor=lightblue];\n";
        dotFile << "  edge [color=blue];\n";

        // Write nodes
        for (const auto &hospital : hospitals)
        {
            string escapedName = hospital.second.name;
            replace(escapedName.begin(), escapedName.end(), '"', '\'');
            dotFile << "  \"" << hospital.second.id << "\" [label=\""
                    << hospital.second.id << "\\n"
                    << escapedName << "\"];\n";
        }

        // Write edges
        for (const auto &entry : adjacencyList)
        {
            for (const auto &connection : entry.second)
            {
                if (entry.first < connection.first)
                {
                    dotFile << "  \"" << entry.first << "\" -- \""
                            << connection.first << "\" [label=\""
                            << connection.second << " km\"];\n";
                }
            }
        }

        dotFile << "}\n";
        dotFile.close();
        cout << GREEN << "✅ Graphviz DOT file saved to " << filename
             << "\nUse 'dot -Tpng " << filename << " -o hospital_network.png' to render.\n"
             << RESET;
    }

    bool loadFromCSV(const string &filename)
    {
        ifstream csvFile(filename);
        if (!csvFile.is_open())
        {
            cerr << RED << "❌ Error: Could not open file " << filename << RESET << endl;
            return false;
        }

        hospitals.clear();
        adjacencyList.clear();

        string line;
        bool readingHospitals = false, readingRoads = false;

        while (getline(csvFile, line))
        {
            if (line.empty())
                continue;

            if (line == "Hospital Data")
            {
                readingHospitals = true;
                readingRoads = false;
                getline(csvFile, line);
                continue;
            }
            else if (line == "Road Connections")
            {
                readingHospitals = false;
                readingRoads = true;
                getline(csvFile, line);
                continue;
            }

            stringstream ss(line);
            string token;
            vector<string> tokens;

            while (getline(ss, token, ','))
            {
                tokens.push_back(token);
            }

            if (readingHospitals && tokens.size() == 4)
            {
                try
                {
                    int employees = stoi(tokens[3]);
                    addHospital(tokens[0], tokens[1], tokens[2], employees);
                }
                catch (...)
                {
                    cerr << RED << "❌ Error: Invalid hospital data in line: " << line << RESET << endl;
                }
            }
            else if (readingRoads && tokens.size() == 5)
            {
                try
                {
                    int distance = stoi(tokens[4]);
                    addRoad(tokens[0], tokens[2], distance);
                }
                catch (...)
                {
                    cerr << RED << "❌ Error: Invalid road data in line: " << line << RESET << endl;
                }
            }
        }

        csvFile.close();
        cout << GREEN << "✅ Data loaded from " << filename << RESET << endl;
        return true;
    }

    void printGraph()
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

        for (const auto &hospital : hospitals)
        {
            cout << setw(8) << hospital.second.id << setw(20) << hospital.second.name
                 << setw(15) << hospital.second.location << hospital.second.employees << endl;
        }

        cout << BLUE << "\n🛣️  ROAD CONNECTIONS (" << adjacencyList.size() << "):\n"
             << RESET;
        cout << CYAN << "----------------------------------------\n"
             << RESET;
        cout << BOLD << left << setw(20) << "From Hospital" << setw(20) << "To Hospital"
             << "Distance (km)\n"
             << RESET;
        cout << CYAN << "----------------------------------------\n"
             << RESET;

        for (const auto &entry : adjacencyList)
        {
            for (const auto &connection : entry.second)
            {
                if (entry.first < connection.first)
                {
                    cout << setw(20) << hospitals[entry.first].name + " (" + entry.first + ")"
                         << setw(20) << hospitals[connection.first].name + " (" + connection.first + ")"
                         << connection.second << endl;
                }
            }
        }

        cout << YELLOW << "\n⚠️  UNCONNECTED HOSPITALS:\n"
             << RESET;
        cout << CYAN << "----------------------------------------\n"
             << RESET;
        bool anyUnconnected = false;
        for (const auto &hospital : hospitals)
        {
            if (adjacencyList.find(hospital.first) == adjacencyList.end())
            {
                cout << "• " << hospital.second.name << " (" << hospital.first << ")\n";
                anyUnconnected = true;
            }
        }
        if (!anyUnconnected)
        {
            cout << GREEN << "All hospitals are connected!\n"
                 << RESET;
        }
    }

    void saveToCSV(const string &filename)
    {
        ofstream csvFile(filename);

        if (!csvFile.is_open())
        {
            cerr << RED << "❌ Error: Could not open file " << filename << RESET << endl;
            return;
        }

        csvFile << "Hospital Data\n";
        csvFile << "ID,Name,Location,Employees\n";
        for (const auto &hospital : hospitals)
        {
            csvFile << hospital.second.id << ","
                    << hospital.second.name << ","
                    << hospital.second.location << ","
                    << hospital.second.employees << "\n";
        }

        csvFile << "\nRoad Connections\n";
        csvFile << "From ID,From Name,To ID,To Name,Distance (km)\n";
        for (const auto &entry : adjacencyList)
        {
            for (const auto &connection : entry.second)
            {
                if (entry.first < connection.first)
                {
                    csvFile << entry.first << ","
                            << hospitals[entry.first].name << ","
                            << connection.first << ","
                            << hospitals[connection.first].name << ","
                            << connection.second << "\n";
                }
            }
        }

        csvFile.close();
        cout << GREEN << "✅ Data saved to " << filename << RESET << endl;
    }

    void saveToText(const string &filename)
    {
        ofstream textFile(filename);

        if (!textFile.is_open())
        {
            cerr << RED << "❌ Error: Could not open file " << filename << RESET << endl;
            return;
        }

        textFile << "=== HOSPITAL NETWORK REPORT ===\n\n";
        textFile << "HOSPITALS (" << hospitals.size() << "):\n";
        for (const auto &hospital : hospitals)
        {
            textFile << "ID: " << hospital.second.id
                     << ", Name: " << hospital.second.name
                     << ", Location: " << hospital.second.location
                     << ", Employees: " << hospital.second.employees << endl;
        }

        textFile << "\nROAD CONNECTIONS (" << adjacencyList.size() << "):\n";
        for (const auto &entry : adjacencyList)
        {
            for (const auto &connection : entry.second)
            {
                if (entry.first < connection.first)
                {
                    textFile << hospitals[entry.first].name << " (" << entry.first << ") <--> "
                             << hospitals[connection.first].name << " (" << connection.first << "), "
                             << connection.second << " km\n";
                }
            }
        }

        textFile << "\nUNCONNECTED HOSPITALS:\n";
        bool anyUnconnected = false;
        for (const auto &hospital : hospitals)
        {
            if (adjacencyList.find(hospital.first) == adjacencyList.end())
            {
                textFile << hospital.second.id << " (" << hospital.second.name << ")\n";
                anyUnconnected = true;
            }
        }
        if (!anyUnconnected)
        {
            textFile << "All hospitals are connected!\n";
        }

        textFile.close();
        cout << GREEN << "✅ Data saved to " << filename << RESET << endl;
    }
};

// ====================== INPUT FUNCTIONS ======================
int getIntegerInput(const string &prompt, int min = 0)
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
            cout << RED << "Invalid input. Please enter a number";
            if (min > 0)
                cout << " >= " << min;
            cout << ".\n"
                 << RESET;
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

string getStringInput(const string &prompt, bool toLowercase = false)
{
    string input;
    cout << BOLD << prompt << RESET;
    getline(cin, input);
    if (toLowercase)
    {
        transform(input.begin(), input.end(), input.begin(),
                  [](unsigned char c)
                  { return tolower(c); });
    }
    return input;
}

void displayHospital(const Hospital *hospital)
{
    if (hospital == nullptr)
    {
        cout << RED << "❌ Hospital not found.\n"
             << RESET;
        return;
    }
    cout << MAGENTA << "\n🏥 HOSPITAL DETAILS:\n"
         << RESET;
    cout << CYAN << "----------------------------------------\n"
         << RESET;
    cout << left << setw(15) << "ID:" << hospital->id << "\n";
    cout << setw(15) << "Name:" << hospital->name << "\n";
    cout << setw(15) << "Location:" << hospital->location << "\n";
    cout << setw(15) << "Employees:" << hospital->employees << "\n";
    cout << CYAN << "----------------------------------------\n"
         << RESET;
}

// ====================== MAIN MENU ======================
void showMainMenu()
{
    displayHeader("Main Menu");
    cout << BOLD << "1. " << RESET << "Add a hospital\n";
    cout << BOLD << "2. " << RESET << "Add a road connection\n";
    cout << BOLD << "3. " << RESET << "Update hospital information\n";
    cout << BOLD << "4. " << RESET << "Delete a hospital\n";
    cout << BOLD << "5. " << RESET << "Delete a road connection\n";
    cout << BOLD << "6. " << RESET << "View hospital details\n";
    cout << BOLD << "7. " << RESET << "View road details\n";
    cout << BOLD << "8. " << RESET << "View network summary\n";
    cout << BOLD << "9. " << RESET << "Save data to files\n";
    cout << BOLD << "10. " << RESET << "Search hospitals\n";
    cout << BOLD << "11. " << RESET << "Generate visualization\n";
    cout << BOLD << "12. " << RESET << "Exit\n";
    cout << CYAN << "----------------------------------------\n"
         << RESET;
}

int main()
{
    HospitalGraph hospitalNetwork;

    // Attempt to load data at startup
    hospitalNetwork.loadFromCSV("hospital_data.csv");

    while (true)
    {
        showMainMenu();
        int choice = getIntegerInput("Enter your choice (1-12): ");

        switch (choice)
        {
        case 1:
        {
            displayHeader("Add Hospital");
            string id = getStringInput("Enter hospital ID: ");
            string name = getStringInput("Enter hospital name: ");
            string location = getStringInput("Enter hospital location: ");
            int employees = getIntegerInput("Enter number of employees: ", 1);
            hospitalNetwork.addHospital(id, name, location, employees);
            pressEnterToContinue();
            break;
        }
        case 2:
        {
            displayHeader("Add Road Connection");
            string id1 = getStringInput("Enter first hospital ID: ");
            string id2 = getStringInput("Enter second hospital ID: ");
            int distance = getIntegerInput("Enter distance between them (km): ", 1);
            hospitalNetwork.addRoad(id1, id2, distance);
            pressEnterToContinue();
            break;
        }
        case 3:
        {
            displayHeader("Update Hospital");
            string id = getStringInput("Enter hospital ID to update: ");
            string newName = getStringInput("Enter new name (leave blank to keep current): ");
            string newLocation = getStringInput("Enter new location (leave blank to keep current): ");
            int newEmployees = getIntegerInput("Enter new employee count (0 to keep current): ");

            Hospital *hospital = hospitalNetwork.getHospital(id);
            if (hospital)
            {
                if (newName.empty())
                    newName = hospital->name;
                if (newLocation.empty())
                    newLocation = hospital->location;
                if (newEmployees == 0)
                    newEmployees = hospital->employees;
                hospitalNetwork.updateHospital(id, newName, newLocation, newEmployees);
            }
            pressEnterToContinue();
            break;
        }
        case 4:
        {
            displayHeader("Delete Hospital");
            string id = getStringInput("Enter hospital ID to delete: ");
            hospitalNetwork.deleteHospital(id);
            pressEnterToContinue();
            break;
        }
        case 5:
        {
            displayHeader("Delete Road Connection");
            string id1 = getStringInput("Enter first hospital ID: ");
            string id2 = getStringInput("Enter second hospital ID: ");
            hospitalNetwork.deleteRoad(id1, id2);
            pressEnterToContinue();
            break;
        }
        case 6:
        {
            displayHeader("View Hospital");
            string id = getStringInput("Enter hospital ID: ");
            displayHospital(hospitalNetwork.getHospital(id));
            pressEnterToContinue();
            break;
        }
        case 7:
        {
            displayHeader("View Road Details");
            string id1 = getStringInput("Enter first hospital ID: ");
            string id2 = getStringInput("Enter second hospital ID: ");
            int *distance = hospitalNetwork.getRoadDistance(id1, id2);
            if (distance)
            {
                cout << BLUE << "\n🛣️  ROAD DETAILS:\n"
                     << RESET;
                cout << CYAN << "----------------------------------------\n"
                     << RESET;
                cout << "From: " << hospitalNetwork.getHospital(id1)->name << " (" << id1 << ")\n";
                cout << "To:   " << hospitalNetwork.getHospital(id2)->name << " (" << id2 << ")\n";
                cout << "Distance: " << *distance << " km\n";
                cout << CYAN << "----------------------------------------\n"
                     << RESET;
            }
            else
            {
                cout << RED << "❌ No road exists between these hospitals.\n"
                     << RESET;
            }
            pressEnterToContinue();
            break;
        }
        case 8:
            hospitalNetwork.printGraph();
            pressEnterToContinue();
            break;
        case 9:
            displayHeader("Save Data");
            hospitalNetwork.saveToCSV("hospital_data.csv");
            hospitalNetwork.saveToText("hospital_data.txt");
            pressEnterToContinue();
            break;
        case 10:
        {
            displayHeader("Search Hospitals");
            string query = getStringInput("Enter search query: ");
            vector<Hospital *> results = hospitalNetwork.searchHospitals(query);
            if (results.empty())
            {
                cout << RED << "❌ No hospitals found matching '" << query << "'.\n"
                     << RESET;
            }
            else
            {
                cout << GREEN << "\nFound " << results.size() << " matching hospitals:\n"
                     << RESET;
                for (const auto &hospital : results)
                {
                    displayHospital(hospital);
                }
            }
            pressEnterToContinue();
            break;
        }
        case 11:
            displayHeader("Generate Visualization");
            hospitalNetwork.generateGraphvizDOT("hospital_network.dot");
            pressEnterToContinue();
            break;
        case 12:
            displayHeader("Exit");
            cout << GREEN << "Saving data before exiting...\n"
                 << RESET;
            hospitalNetwork.saveToCSV("hospital_data.csv");
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
    return 0;
}