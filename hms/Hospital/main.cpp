// Hospital Network Management System
// main.cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <regex>

using namespace std;

struct Hospital {
    string id;
    string name;
    string location;
    int patients;
};

map<string, Hospital> hospitals;
map<string, vector<pair<string, string>>> graph; // id -> list of (connected_id, description)

const string HOSPITAL_FILE = "hospitals.csv";
const string GRAPH_FILE = "graph.txt";
const string RELATIONSHIPS_FILE = "relationships.csv";

// Helper Functions
bool isNumber(const string& s) {
    return regex_match(s, regex("[0-9]+"));
}

void loadHospitals() {
    hospitals.clear();
    ifstream file(HOSPITAL_FILE);
    string line;
    getline(file, line); // skip header
    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, location, patients_str;
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, location, ',');
        getline(ss, patients_str);
        Hospital h{id, name, location, stoi(patients_str)};
        hospitals[id] = h;
    }
    file.close();
}

void saveHospitals() {
    ofstream file(HOSPITAL_FILE);
    file << "ID,Name,Location,Number of Patients\n";
    for (const auto& [id, h] : hospitals) {
        file << h.id << "," << h.name << "," << h.location << "," << h.patients << "\n";
    }
    file.close();
}

void loadGraph() {
    graph.clear();
    ifstream file(GRAPH_FILE);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string center;
        getline(ss, center, ',');
        string conn;
        while (getline(ss, conn, ',')) {
            size_t pos = conn.find(":");
            if (pos != string::npos) {
                string target = conn.substr(0, pos);
                string desc = conn.substr(pos + 1);
                graph[center].push_back({target, desc});
            }
        }
    }
    file.close();
}

void saveGraph() {
    ofstream file(GRAPH_FILE);
    for (const auto& [center, connections] : graph) {
        file << center;
        for (const auto& [target, desc] : connections) {
            file << "," << target << ":" << desc;
        }
        file << "\n";
    }
    file.close();
}

void addHospital() {
    string id, name, location, patient_str;
    cout << "Enter Hospital ID: "; cin >> id;
    if (hospitals.find(id) != hospitals.end()) {
        cout << "Hospital ID already exists.\n";
        return;
    }
    cout << "Enter Name: "; cin.ignore(); getline(cin, name);
    cout << "Enter Location: "; getline(cin, location);
    cout << "Enter Number of Patients: "; getline(cin, patient_str);
    if (!isNumber(patient_str)) {
        cout << "Invalid patient number.\n";
        return;
    }
    Hospital h{id, name, location, stoi(patient_str)};
    hospitals[id] = h;
    saveHospitals();
    cout << "Hospital added successfully.\n";
}

void viewHospitals() {
    cout << "ID\tName\tLocation\tPatients\n";
    for (const auto& [id, h] : hospitals) {
        cout << h.id << "\t" << h.name << "\t" << h.location << "\t" << h.patients << "\n";
    }
}

void updateHospital() {
    string id;
    cout << "Enter Hospital ID to update: "; cin >> id;
    if (hospitals.find(id) == hospitals.end()) {
        cout << "Hospital ID not found.\n";
        return;
    }
    string name, location, patient_str;
    cout << "Enter New Name: "; cin.ignore(); getline(cin, name);
    cout << "Enter New Location: "; getline(cin, location);
    cout << "Enter New Number of Patients: "; getline(cin, patient_str);
    if (!isNumber(patient_str)) {
        cout << "Invalid patient number.\n";
        return;
    }
    hospitals[id] = {id, name, location, stoi(patient_str)};
    saveHospitals();
    cout << "Hospital updated successfully.\n";
}

void deleteHospital() {
    string id;
    cout << "Enter Hospital ID to delete: "; cin >> id;
    if (hospitals.erase(id)) {
        graph.erase(id);
        for (auto& [center, connections] : graph) {
            connections.erase(remove_if(connections.begin(), connections.end(), [id](const pair<string, string>& p) {
                return p.first == id;
            }), connections.end());
        }
        saveHospitals();
        saveGraph();
        cout << "Hospital deleted successfully.\n";
    } else {
        cout << "Hospital ID not found.\n";
    }
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

void viewConnections() {
    cout << "Hospital Center\tConnected Hospitals\tDescriptions\n";
    for (const auto& [id, _] : hospitals) {
        cout << id << "\t";
        vector<string> cons, descs;
        for (const auto& [target, desc] : graph[id]) {
            cons.push_back(target);
            descs.push_back(desc);
        }
        cout << (cons.empty() ? "None" : cons[0]);
        for (size_t i = 1; i < cons.size(); ++i) cout << ", " << cons[i];
        cout << "\t";
        cout << (descs.empty() ? "None" : descs[0]);
        for (size_t i = 1; i < descs.size(); ++i) cout << ", " << descs[i];
        cout << "\n";
    }
}

void exportRelationships() {
    ofstream file(RELATIONSHIPS_FILE);
    file << "Hospital Center,Connected Hospitals,Description\n";
    for (const auto& [id, _] : hospitals) {
        file << id << ",";
        vector<string> cons, descs;
        for (const auto& [target, desc] : graph[id]) {
            cons.push_back(target);
            descs.push_back(desc);
        }
        file << (cons.empty() ? "" : cons[0]);
        for (size_t i = 1; i < cons.size(); ++i) file << " " << cons[i];
        file << ",";
        file << (descs.empty() ? "" : descs[0]);
        for (size_t i = 1; i < descs.size(); ++i) file << " " << descs[i];
        file << "\n";
    }
    file.close();
    cout << "Exported to relationships.csv\n";
}

void predefinedScenario() {
    vector<Hospital> preset = {
        {"H1", "City General", "Kigali", 120},
        {"H2", "Northern Hospital", "Musanze", 90},
        {"H3", "Eastern Medical", "Rwamagana", 75},
        {"H4", "Western Health", "Rubavu", 85},
        {"H5", "Southern Clinic", "Huye", 60},
        {"H6", "Central Hospital", "Muhanga", 105}
    };
    for (auto& h : preset) hospitals[h.id] = h;
    saveHospitals();

    vector<tuple<string, string, string>> links = {
        {"H1", "H6", "Emergency backup"},
        {"H1", "H4", "Referral support"},
        {"H2", "H3", "Standard route"},
        {"H6", "H5", "Ambulance path"},
        {"H5", "H4", "Night transfer"},
        {"H4", "H1", "City route"},
        {"H2", "H3", "Backup route"}
    };
    for (auto& [a, b, desc] : links) {
        graph[a].push_back({b, desc});
        graph[b].push_back({a, desc});
    }
    saveGraph();
    cout << "Predefined hospitals and connections added.\n";
}

int main() {
    loadHospitals();
    loadGraph();
    int choice;
    while (true) {
        cout << "\n--- Hospital Network Management ---\n";
        cout << "1. Add Hospital\n2. View Hospitals\n3. Update Hospital\n4. Delete Hospital\n";
        cout << "5. Connect Hospitals\n6. View Connections\n7. Predefined Setup\n8. Export Relationships\n9. Exit\nChoose: ";
        cin >> choice;
        switch (choice) {
            case 1: addHospital(); break;
            case 2: viewHospitals(); break;
            case 3: updateHospital(); break;
            case 4: deleteHospital(); break;
            case 5: connectHospitals(); break;
            case 6: viewConnections(); break;
            case 7: predefinedScenario(); break;
            case 8: exportRelationships(); break;
            case 9: return 0;
            default: cout << "Invalid option.\n"; break;
        }
    }
}
