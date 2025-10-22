#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_CITIES 30
#define MAX_DELIVERIES 50
#define MAX_NAME_LENGTH 50
#define FUEL_PRICE 310.0
// Vehicle structure
typedef struct {
    char type[10];
    int capacity;
    int rate_per_km;
    int avg_speed;
    int fuel_efficiency;
} Vehicle;
//Delivery structure
typedef struct {
    int id;
    char source[MAX_NAME_LENGTH];
    char destination[MAX_NAME_LENGTH];
    float weight;
    char vehicle_type[10];
    float distance;
    float base_cost;
    float fuel_used;
    float fuel_cost;
    float operational_cost;
    float profit;
    float customer_charge;
    float delivery_time;
    int completed;
} Delivery;
// Global Arrays
char cities[MAX_CITIES][MAX_NAME_LENGTH];
float distances[MAX_CITIES][MAX_CITIES];
Delivery deliveries[MAX_DELIVERIES];
Vehicle vehicles[3];
int city_count = 0;
int delivery_count = 0;
// Function prototypes
void initializeSystem();
void displayMainMenu();
void cityManagement();
void distanceManagement();
void vehicleManagement();
void deliveryRequest();
void reports();
void fileHandling();

// City Management functions
void addCity();
void renameCity();
void removeCity();
void displayCities();

// Distance Management functions
void inputDistance();
void editDistance();
void displayDistanceTable();

// Delivery functions
void calculateDelivery(int source, int destination, float weight, int vehicle_type);
float findMinimumDistance(int source, int destination);
void generatePermutations(int cities[], int start, int end, int source, int destination, float *min_distance, int best_path[], int *best_path_length);
void swap(int *a, int *b);
float calculatePathDistance(int path[], int length);

// Utility functions
int findCityIndex(char *city_name);
void toLowerCase(char *str);
int isCityExists(char *city_name);
void loadData();
void saveData();

void initializeSystem() {
    // Initializing vehicles
    strcpy(vehicles[0].type, "Van");
    vehicles[0].capacity = 1000;
    vehicles[0].rate_per_km = 30;
    vehicles[0].avg_speed = 60;
    vehicles[0].fuel_efficiency = 12;
    
    strcpy(vehicles[1].type, "Truck");
    vehicles[1].capacity = 5000;
    vehicles[1].rate_per_km = 40;
    vehicles[1].avg_speed = 50;
    vehicles[1].fuel_efficiency = 6;
    
    strcpy(vehicles[2].type, "Lorry");
    vehicles[2].capacity = 10000;
    vehicles[2].rate_per_km = 80;
    vehicles[2].avg_speed = 45;
    vehicles[2].fuel_efficiency = 4;
    
    // Initializing distance matrix
    for(int i = 0; i < MAX_CITIES; i++) {
        for(int j = 0; j < MAX_CITIES; j++) {
            if(i == j) 
                distances[i][j] = 0;
            else
                distances[i][j] = -1; // -1 is meant to be no connection
        }
    }
}

void displayMainMenu() {
    system("cls"); 
    printf("==============================================\n");
    printf("      LOGISTICS MANAGEMENT SYSTEM\n");
    printf("==============================================\n");
    printf("1. City Management\n");
    printf("2. Distance Management\n");
    printf("3. Vehicle Management\n");
    printf("4. Delivery Request\n");
    printf("5. Reports\n");
    printf("6. Save Data\n");
    printf("7. Exit\n");
    printf("==============================================\n");
}
void cityManagement() {
    int choice;
    do {
        system("cls");
        printf("=== CITY MANAGEMENT ===\n");
        printf("1. Add New City\n");
        printf("2. Rename City\n");
        printf("3. Remove City\n");
        printf("4. Display All Cities\n");
        printf("5. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                addCity();
                break;
            case 2:
                renameCity();
                break;
            case 3:
                removeCity();
                break;
            case 4:
                displayCities();
                break;
            case 5:
                break;
            default:
                printf("Invalid choice!\n");
        }
        if(choice != 5) {
            printf("\nPress Enter to continue...");
            getchar(); getchar();
        }
    } while(choice != 5);
}

void addCity() {
    if(city_count >= MAX_CITIES) {
        printf("Maximum city limit reached (%d cities)\n", MAX_CITIES);
        return;
    }
    
    char city_name[MAX_NAME_LENGTH];
    printf("Enter city name: ");
    scanf("%s", city_name);
    
    if(isCityExists(city_name)) {
        printf("City '%s' already exists!\n", city_name);
        return;
    }
    
    strcpy(cities[city_count], city_name);
    city_count++;
    printf("City '%s' added successfully!\n", city_name);
}

void renameCity() {
    if(city_count == 0) {
        printf("No cities available!\n");
        return;
    }
    
    displayCities();
    int index;
    printf("Enter city number to rename: ");
    scanf("%d", &index);
    
    if(index < 1 || index > city_count) {
        printf("Invalid city number!\n");
        return;
    }
    
    char new_name[MAX_NAME_LENGTH];
    printf("Enter new name for %s: ", cities[index-1]);
    scanf("%s", new_name);
    
    if(isCityExists(new_name)) {
        printf("City '%s' already exists!\n", new_name);
        return;
    }
    
    printf("City '%s' renamed to '%s'\n", cities[index-1], new_name);
    strcpy(cities[index-1], new_name);
}
void removeCity() {
    if(city_count == 0) {
        printf("No cities available!\n");
        return;
    }
    
    displayCities();
    int index;
    printf("Enter city number to remove: ");
    scanf("%d", &index);
    
    if(index < 1 || index > city_count) {
        printf("Invalid city number!\n");
        return;
    }
    
    printf("Are you sure you want to remove '%s'? (y/n): ", cities[index-1]);
    char confirm;
    scanf(" %c", &confirm);
    
    if(confirm == 'y' || confirm == 'Y') {
        // Shift cities array
        int i;
        for( i = index-1; i < city_count-1; i++) {
            strcpy(cities[i], cities[i+1]);
        }
        city_count--;
        printf("City removed successfully!\n");
    }
}

void displayCities() {
    printf("\n=== AVAILABLE CITIES ===\n");
    if(city_count == 0) {
        printf("No cities available.\n");
        return;
    }
    int i;
    for(i = 0; i < city_count; i++) {
        printf("%d. %s\n", i+1, cities[i]);
    }
}
void distanceManagement() {
    int choice;
    do {
        system("cls");
        printf("=== DISTANCE MANAGEMENT ===\n");
        printf("1. Input/Edit Distance\n");
        printf("2. Display Distance Table\n");
        printf("3. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                inputDistance();
                break;
            case 2:
                displayDistanceTable();
                break;
            case 3:
                break;
            default:
                printf("Invalid choice!\n");
        }
        if(choice != 3) {
            printf("\nPress Enter to continue...");
            getchar(); getchar();
        }
    } while(choice != 3);
}
void inputDistance() {
    if(city_count < 2) {
        printf("Need at least 2 cities to input distances!\n");
        return;
    }
    
    displayCities();
    int city1, city2;
    float distance;
    
    printf("Enter source city number: ");
    scanf("%d", &city1);
    printf("Enter destination city number: ");
    scanf("%d", &city2);
    
    if(city1 < 1 || city1 > city_count || city2 < 1 || city2 > city_count) {
        printf("Invalid city numbers!\n");
        return;
    }
    
    if(city1 == city2) {
        printf("Source and destination cannot be the same!\n");
        return;
    }
    
    printf("Enter distance between %s and %s (km): ", cities[city1-1], cities[city2-1]);
    scanf("%f", &distance);
    
    if(distance < 0) {
        printf("Distance cannot be negative!\n");
        return;
    }
    
    distances[city1-1][city2-1] = distance;
    distances[city2-1][city1-1] = distance; // Make symmetric
    printf("Distance updated successfully!\n");
}

void displayDistanceTable() {
    if(city_count == 0) {
        printf("No cities available!\n");
        return;
    }
    
    printf("\n=== DISTANCE TABLE (km) ===\n");
    printf("%-15s", "");
    int i;
    for(i = 0; i < city_count; i++) {
        printf("%-15s", cities[i]);
    }
    printf("\n");
    
    for(i = 0; i < city_count; i++) {
        printf("%-15s", cities[i]);
        int j;
        for(j = 0; j < city_count; j++) {
            if(distances[i][j] == -1)
                printf("%-15s", "N/A");
            else
                printf("%-15.2f", distances[i][j]);
        }
        printf("\n");
    }
}












