#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HOSPITALS 100
#define MAX_NAME_LENGTH 50
#define MAX_CONNECTIONS 20

// Structure to represent a hospital
typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char location[MAX_NAME_LENGTH];
    int num_connections;
    struct {
        int target_hospital_id;
        float distance;
    } connections[MAX_CONNECTIONS];
} Hospital;

// Structure to represent the graph
typedef struct {
    Hospital hospitals[MAX_HOSPITALS];
    int num_hospitals;
} HospitalGraph;

// Function to initialize the graph
void initGraph(HospitalGraph* graph) {
    graph->num_hospitals = 0;
}

// Function to add a hospital to the graph
int addHospital(HospitalGraph* graph, const char* name, const char* location) {
    if (graph->num_hospitals >= MAX_HOSPITALS) {
        return -1; // Graph is full
    }

    Hospital* new_hospital = &graph->hospitals[graph->num_hospitals];
    new_hospital->id = graph->num_hospitals;
    strncpy(new_hospital->name, name, MAX_NAME_LENGTH - 1);
    strncpy(new_hospital->location, location, MAX_NAME_LENGTH - 1);
    new_hospital->num_connections = 0;
    
    graph->num_hospitals++;
    return new_hospital->id;
}

// Function to add a connection between hospitals
int addConnection(HospitalGraph* graph, int hospital1_id, int hospital2_id, float distance) {
    if (hospital1_id < 0 || hospital1_id >= graph->num_hospitals ||
        hospital2_id < 0 || hospital2_id >= graph->num_hospitals) {
        return -1; // Invalid hospital IDs
    }

    Hospital* hospital1 = &graph->hospitals[hospital1_id];
    Hospital* hospital2 = &graph->hospitals[hospital2_id];

    // Add connection to hospital1
    if (hospital1->num_connections >= MAX_CONNECTIONS) {
        return -1; // Too many connections
    }
    hospital1->connections[hospital1->num_connections].target_hospital_id = hospital2_id;
    hospital1->connections[hospital1->num_connections].distance = distance;
    hospital1->num_connections++;

    // Add connection to hospital2 (bidirectional)
    if (hospital2->num_connections >= MAX_CONNECTIONS) {
        return -1; // Too many connections
    }
    hospital2->connections[hospital2->num_connections].target_hospital_id = hospital1_id;
    hospital2->connections[hospital2->num_connections].distance = distance;
    hospital2->num_connections++;

    return 0;
}

// Function to save the graph to a CSV file
void saveToCSV(HospitalGraph* graph, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing\n");
        return;
    }

    // Write header
    fprintf(file, "HospitalID,HospitalName,Location,ConnectedHospitals\n");

    // Write each hospital and its connections
    for (int i = 0; i < graph->num_hospitals; i++) {
        Hospital* hospital = &graph->hospitals[i];
        fprintf(file, "%d,%s,%s,", hospital->id, hospital->name, hospital->location);
        
        // Write connections
        for (int j = 0; j < hospital->num_connections; j++) {
            fprintf(file, "%d(%.2f)", 
                    hospital->connections[j].target_hospital_id,
                    hospital->connections[j].distance);
            if (j < hospital->num_connections - 1) {
                fprintf(file, ";");
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

// Function to load the graph from a CSV file
int loadFromCSV(HospitalGraph* graph, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file for reading\n");
        return -1;
    }

    char line[1024];
    // Skip header
    fgets(line, sizeof(line), file);

    // Read each line
    while (fgets(line, sizeof(line), file)) {
        int id;
        char name[MAX_NAME_LENGTH];
        char location[MAX_NAME_LENGTH];
        char connections[1024];

        // Parse the line
        if (sscanf(line, "%d,%[^,],%[^,],%[^\n]", &id, name, location, connections) == 4) {
            // Add hospital
            int hospital_id = addHospital(graph, name, location);
            
            // Parse connections
            char* conn = strtok(connections, ";");
            while (conn != NULL) {
                int target_id;
                float distance;
                if (sscanf(conn, "%d(%f)", &target_id, &distance) == 2) {
                    addConnection(graph, hospital_id, target_id, distance);
                }
                conn = strtok(NULL, ";");
            }
        }
    }

    fclose(file);
    return 0;
}

// Example usage
int main() {
    HospitalGraph graph;
    initGraph(&graph);

    // Add some sample hospitals
    int h1 = addHospital(&graph, "Kanombe Hospital", "Kanombe");
    int h2 = addHospital(&graph, "Kibagabaga Hospital", "Kibagabaga");
    int h3 = addHospital(&graph, "Kabgayi Hospital", "Kabgayi");

    // Add connections
    addConnection(&graph, h1, h2, 5);
    addConnection(&graph, h2, h3, 3);
    addConnection(&graph, h1, h3, 7);

    // Save to CSV
    saveToCSV(&graph, "hospitals.csv");
    printf("Graph saved to hospitals.csv\n");

    // Load from CSV
    HospitalGraph new_graph;
    initGraph(&new_graph);
    if (loadFromCSV(&new_graph, "hospitals.csv") == 0) {
        printf("Graph loaded successfully\n");
    }

    return 0;
} 