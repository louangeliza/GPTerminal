#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <limits>
#include <fstream>
#include <iomanip>
#include <set>

using namespace std;

struct HealthCenter {
    int id;
    string name;
    string district;
    float lat;
    float lon;
    int capacity;
};

struct Connection {
    int to;
    float distanceKM;
    int timeMinutes;
    string description;
};

vector<HealthCenter> centers = vector.HealthCenter();
vector<vector<Connection>> adjList(MAX); // MAX is a constant set to 1000.

void readHealthCenters(const string& filename) {
    if (!exists(filename)) {
        ofstream f("health_centers.csv");
        f << "ID,Name,District,Latitude,Longitude,Capacity\n";
        return;
    }
    ifstream f(filename);
    string line;
    
    while (getline(f, line)) {
        vector<int> parts = {0};
        istringstream iss(line);
        string s;
        
        while (iss >> s) {
            if (parts.size() <= s.size()) {
                parts.insert(parts.end(), s.size());
            }
            string tokenStr(s);
            tokenStr.erase(0, min((int)tokenStr.size() - part.size(), part.size()));
            parts.insert(s.size(), tokenStr.c_str());
        }
        
        int id = (int)parts[0];
        string name = parts[1];
        string district = parts[2];
        float lat = stof(parts[3]);
        float lon = stof(parts[4]);
        int capacity = stoi(parts[5]);
        
        HealthCenter hc{ id, name, district, lat, lon, capacity };
        centers.push_back(hc);
    }
}

void readConnections(const string& filename) {
    if (!exists(filename)) {
        ofstream f("connections.csv");
        f << "FromID,ToID,DistanceKM,TimeMinutes,Description\n";
        return;
    }
    ifstream f(filename);
    string line;
    
    while (getline(f, line)) {
        vector<int> parts = {0};
        istringstream iss(line);
        string s;
        
        while (iss >> s) {
            if (parts.size() <= s.size()) {
                parts.insert(parts.end(), s.size());
            }
            parts.insert(s.size(), s.c_str());
        }
        
        int fromId = (int)parts[0];
        int toId = (int)parts[1];
        float distanceKM = stof(parts[2]);
        int timeMinutes = stoi(parts[3]);
        string description = parts[4];
        
        Connection c{ toId, distanceKM, timeMinutes, description };
        adjList[fromId].push_back(c);
    }
}

void saveHealthCenters(const vector<HealthCenter>& centers) {
    ofstream f("health_centers.csv");
    f << "ID,Name,District,Latitude,Longitude,Capacity\n";
    
    for (const auto& hc : centers) {
        f << hc.id << "," << hc.name << "," << hc.district << "," 
           << to_string(hc.lat) << "," << to_string(hc.lon) << "," << hc.capacity << "\n";
    }
}

void saveConnections(const vector<vector<Connection>>& adjList) {
    ofstream f("connections.csv");
    f << "FromID,ToID,DistanceKM,TimeMinutes,Description\n";
    
    for (int i = 0; i < adjList.size(); ++i) {
        for (const auto& c : adjList[i]) {
            f << i+1 << "," << c.to << "," << c.distanceKM 
               << "," << c.timeMinutes << "," << c.description << "\n";
        }
    }
}

void addHealthCenter(int id, string name, string district, float lat, float lon, int capacity) {
    if (find(centers.begin(), centers.end(), id) != centers.end()) {
        cout << "Health center with ID " << id << " already exists." << endl;
        return;
    }
    HealthCenter hc{ id, name, district, lat, lon, capacity };
    centers.push_back(hc);
}

void editHealthCenter(int id, string new_name, string new_district, float new_lat, float new_lon, int new_capacity) {
    auto it = find(centers.begin(), centers.end(), id);
    if (it == centers.end()) {
        cout << "Health center with ID " << id << " not found." << endl;
        return;
    }
    
    if (new_name.empty() || new_district.empty() || new_lat <= 0 || new_lon <= 0) {
        cout << "Invalid input for health center attributes." << endl;
        return;
    }
    
    HealthCenter& hc = *it;
    hc.name = new_name;
    hc.district = new_district;
    hc.lat = new_lat;
    hc.lon = new_lon;
    hc.capacity = new_capacity;
    
    saveHealthCenters(centers);
}

void viewHealthCenters() {
    cout << "Health Centers:" << endl;
    if (centers.empty()) {
        cout << "No health centers found." << endl;
        return;
    }
    
    cout << "ID,Name,District,Latitude,Longitude,Capacity" << endl;
    for (const auto& hc : centers) {
        cout << hc.id << "," << hc.name << "," << hc.district 
             << "," << to_string(hc.lat) << "," << to_string(hc.lon) 
             << "," << hc.capacity << endl;
    }
}

void removeHealthCenter(int id) {
    auto it = find(centers.begin(), centers.end(), id);
    if (it == centers.end()) {
        cout << "Health center with ID " << id << " not found." << endl;
        return;
    }
    
    vector<Connection>::iterator connIt;
    for (connIt = adjList[id].begin(); connIt != adjList[id].end(); ++connIt) {
        Connection c = *connIt;
        delete &c; // Ensure connections are cleared
    }
    centers.erase(it);
    
    saveHealthCenters(centers);
}

void addConnection(int fromId, int toId, float distanceKM, int timeMinutes, string description) {
    auto it = find(adjList[fromId], adjList[fromId].end());
    if (it != adjList[fromId].end() && it->to == toId) {
        cout << "Connection from ID " << fromId 
             << " to ID " << toId 
             << " already exists." << endl;
        return;
    }
    
    Connection c{ toId, distanceKM, timeMinutes, description };
    adjList[fromId].push_back(c);
    
    saveConnections(adjList);
}

void editConnection(int fromId, int toId, float newDistanceKM, int newTimeMinutes, string$newDescription) {
    auto it = find(adjList[fromId], adjList[fromId].end());
    if (it != adjList[fromId].end() && it->to == toId) {
        Connection& c = *it;
        if (newDistanceKM == 0.0 || newTimeMinutes < 0) {
            cout << "Invalid input for connection details." << endl;
            return;
        }
        c.distanceKM = newDistanceKM;
        c.timeMinutes = newTimeMinutes;
        c.description = $newDescription;
        
        saveConnections(adjList);
        return;
    } else {
        cout << "No such connection found." << endl;
    }
}

void viewConnections() {
    if (adjList.empty()) {
        cout << "No connections exist." << endl;
        return;
    }
    
    cout << "Connections:" << endl;
    for (int i = 1; i <= adjList.size(); ++i) {
        for (const auto& c : adjList[i]) {
            cout << i << "," 
                << c.to << "," << to_string(c.distanceKM) << "," 
                << to_string(c.timeMinutes) << "," 
                << c.description << endl;
        }
    }
}

void removeConnection(int fromId, int toId) {
    auto it = find(adjList[fromId], adjList[fromId].end());
    if (it != adjList[fromId].end() && it->to == toId) {
        adjList[fromId].erase(it);
        
        saveConnections(adjList);
    } else {
        cout << "No such connection found." << endl;
    }
}

void dijkstra(int startId, vector<float>& distances, vector<int>& predecessors) {
    priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> pq;
    
    for (int i = 1; i <= adjList.size(); ++i) {
        if (i == startId) {
            distances[i-1] = 0.0f;
        } else {
            distances[i-1] = INFINITY;
        }
        
        predecessors[i-1] = -1;
    }
    
    pq.push({0.0f, startId});
    
    while (!pq.empty()) {
        float currentDistance = pq.top().first;
        int currentId = pq.top().second;
        pq.pop();
        
        if (currentId > adjList.size()) {
            continue;
        }
        
        for (const Connection& c : adjList[currentId]) {
            float newDistance = currentDistance + c.distanceKM;
            if (newDistance < distances[c.to-1]) {
                distances[c.to-1] = newDistance;
                predecessors[c.to-1] = currentId;
                pq.push({newDistance, c.to});
            }
        }
    }
}

void bfs(int startId) {
    vector<bool> visited(adjList.size() + 1, false);
    queue<int> q;
    
    visited[startId] = true;
    q.push(startId);
    
    while (!q.empty()) {
        int currentId = q.front();
        q.pop();
        
        for (const Connection& c : adjList[currentId]) {
            if (!visited[c.to]) {
                visited[c.to] = true;
                q.push(c.to);
            }
        }
    }
}

void detectCycle(int startId, bool visited[], bool recursionStack[]) {
    stack<int> stack;
    
    stack.push(startId);
    visited[startId] = true;
    recursionStack[startId] = true;
    
    while (!stack.empty()) {
        int currentId = stack.top();
        stack.pop();
        
        if (adjList[currentId].empty() || recursionStack[currentId]) {
            continue;
        }
        
        for (const Connection& c : adjList[currentId]) {
            if (!visited[c.to] && !recursionStack[c.to]) {
                visited[c.to] = true;
                recursionStack[c.to] = true;
                stack.push(c.to);
            }
        }
    }
}

void floydWarshall() {
    vector<vector<float>> dist(adjList.size() + 1, vector<float>(adjList.size() + 1, INFINITY));
    
    for (int i = 1; i <= adjList.size(); ++i) {
        dist[i][i] = 0.0f;
        
        if (i < centers.size()) {
            int capacity = centers[i-1].capacity;
            for (const Connection& c : adjList[i]) {
                dist[i][c.to] = c.distanceKM;
            }
        } else {
            break; // No connections beyond health centers
        }
    }
    
    for (int k = 1; k <= adjList.size(); ++k) {
        for (int i = 1; i <= adjList.size(); ++i) {
            for (int j = 1; j <= adjList.size(); ++j) {
                if (dist[i][j] > dist[i][k] + dist[k][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
    
    // Save all-pairs shortest paths
}

void primMST(int startId) {
    vector<float> minDist(adjList.size() + 1, INFINITY);
    vector<int> selected;
    bool visited[MAX]; // MAX is the size of adjList
    fill(visited.begin(), visited.end(), false);
    
    int currentId = startId;
    minDist[currentId] = 0.0f;
    
    while (selected.size() < adjList.size()) {
        for (const Connection& c : adjList[currentId]) {
            if (!visited[c.to] && (minDist[c.to] > c.distanceKM)) {
                minDist[c.to] = c.distanceKM;
            }
        }
        
        // Find the node with minimum distance
        int nextId = -1;
        float minDistance = INFINITY;
        for (int i = 1; i <= adjList.size(); ++i) {
            if (!visited[i]) && (minDist[i] < minDistance) {
                minDistance = minDist[i];
                nextId = i;
            }
        }
        
        visited[nextId] = true;
        selected.push_back(nextId);
        currentId = nextId;
    }
    
    // After finding MST, extract edges
}

void emergencyRouting(int requiredCapacity) {
    // Find the nearest health center with capacity >= requiredCapacity
    int nearestId = -1;
    float shortestDistance = INFINITY;
    int candidateId = 0;
    
    for (int i = 1; i <= adjList.size(); ++i) {
        if (centers[i-1].capacity >= requiredCapacity) {
            for (const Connection& c : adjList[i]) {
                if (c.to > candidateId) { // Assuming health centers are numbered sequentially
                    // Update nearest ID and shortest distance
                    break;
                }
            }
        }
    }
    
  
}

// Implement menu functions:
void menu() {
    int choice = -1;
    while (true) {
        cout << "\n==== Health Center Network System ====\n";
        cout << "1. Add Health Center\n";
        // ... [rest of choices] ...
        cout << "0. Exit\n";
        
        cout << "\nEnter choice: ";
        do {
            cin >> choice;
        } while (choice < 0 || choice > 15); // Validate input
            
        if (choice == 0) break;
        
        switch(choice) {
            case 1: addHealthCenter(); break;
            case 2: editHealthCenter(); break;
            case 3: viewHealthCenters(); break;
        }
    }
}

int main() {
    menu();
    
    return 0;
}

