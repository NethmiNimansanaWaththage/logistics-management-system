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
int main() {
    initializeSystem();
    loadData();
    
    int choice;
    do {
        displayMainMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                cityManagement();
                break;
            case 2:
                distanceManagement();
                break;
            case 3:
                vehicleManagement();
                break;
            case 4:
                deliveryRequest();
                break;
            case 5:
                reports();
                break;
            case 6:
                saveData();
                printf("Data saved successfully!\n");
                break;
            case 7:
                saveData();
                printf("Thank you for using Logistics Management System!\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
        printf("\nPress Enter to continue...");
        getchar(); getchar();
    } while(choice != 7);
    
    return 0;
}
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
void vehicleManagement() {
    system("cls");
    printf("=== VEHICLE MANAGEMENT ===\n");
    printf("%-10s %-12s %-15s %-12s %-15s\n", 
           "Type", "Capacity(kg)", "Rate/km(LKR)", "Speed(km/h)", "Fuel Eff(km/l)");
    printf("-----------------------------------------------------------------\n");
    int i;
    for(i = 0; i < 3; i++) {
        printf("%-10s %-12d %-15d %-12d %-15d\n",
               vehicles[i].type, vehicles[i].capacity, vehicles[i].rate_per_km,
               vehicles[i].avg_speed, vehicles[i].fuel_efficiency);
    }
    
    printf("\nThese are fixed vehicle types and cannot be modified.\n");
}

void deliveryRequest() {
    if(city_count < 2) {
        printf("Need at least 2 cities for delivery!\n");
        return;
    }
    
    if(delivery_count >= MAX_DELIVERIES) {
        printf("Maximum delivery limit reached!\n");
        return;
    }
    
    system("cls");
    printf("=== DELIVERY REQUEST ===\n");
    displayCities();
    
    int source, destination, vehicle_choice;
    float weight;
    
    printf("Enter source city number: ");
    scanf("%d", &source);
    printf("Enter destination city number: ");
    scanf("%d", &destination);
    
    if(source < 1 || source > city_count || destination < 1 || destination > city_count) {
        printf("Invalid city numbers!\n");
        return;
    }
    
    if(source == destination) {
        printf("Source and destination cannot be the same!\n");
        return;
    }
    
    if(distances[source-1][destination-1] == -1) {
        printf("No direct route available between %s and %s!\n", 
               cities[source-1], cities[destination-1]);
        return;
    }
    
    printf("\nAvailable Vehicles:\n");
    int i;
    for( i = 0; i < 3; i++) {
        printf("%d. %s (Capacity: %d kg)\n", i+1, vehicles[i].type, vehicles[i].capacity);
    }
    
    printf("Select vehicle type (1-3): ");
    scanf("%d", &vehicle_choice);
    
    if(vehicle_choice < 1 || vehicle_choice > 3) {
        printf("Invalid vehicle choice!\n");
        return;
    }
    
    printf("Enter package weight (kg): ");
    scanf("%f", &weight);
    
    if(weight <= 0) {
        printf("Weight must be positive!\n");
        return;
    }
    
    if(weight > vehicles[vehicle_choice-1].capacity) {
        printf("Weight exceeds vehicle capacity! Maximum: %d kg\n", 
               vehicles[vehicle_choice-1].capacity);
        return;
    }
    
    calculateDelivery(source-1, destination-1, weight, vehicle_choice-1);
}

void calculateDelivery(int source, int destination, float weight, int vehicle_type) {
    Delivery *delivery = &deliveries[delivery_count];
    
    delivery->id = delivery_count + 1;
    strcpy(delivery->source, cities[source]);
    strcpy(delivery->destination, cities[destination]);
    delivery->weight = weight;
    strcpy(delivery->vehicle_type, vehicles[vehicle_type].type);
    
    // Find minimum distance (direct or through other cities)
    delivery->distance = findMinimumDistance(source, destination);
    
    float D = delivery->distance;
    float W = weight;
    float R = vehicles[vehicle_type].rate_per_km;
    float S = vehicles[vehicle_type].avg_speed;
    float E = vehicles[vehicle_type].fuel_efficiency;
    
    // Calculations
    delivery->base_cost = D * R * (1 + W / 10000.0);
    delivery->fuel_used = D / E;
    delivery->fuel_cost = delivery->fuel_used * FUEL_PRICE;
    delivery->operational_cost = delivery->base_cost + delivery->fuel_cost;
    delivery->profit = delivery->base_cost * 0.25;
    delivery->customer_charge = delivery->operational_cost + delivery->profit;
    delivery->delivery_time = D / S;
    delivery->completed = 1;
    
    // Display results
    printf("\n==============================================================\n");
    printf("                DELIVERY COST ESTIMATION\n");
    printf("==============================================================\n");
    printf("From: %s\n", delivery->source);
    printf("To: %s\n", delivery->destination);
    printf("Minimum Distance: %.2f km\n", delivery->distance);
    printf("Vehicle: %s\n", delivery->vehicle_type);
    printf("Weight: %.2f kg\n", delivery->weight);
    printf("==============================================================\n");
    printf("Base Cost: %.2f × %.2f × (1 + %.2f/10000) = %.2f LKR\n", 
           D, R, W, delivery->base_cost);
    printf("Fuel Used: %.2f L\n", delivery->fuel_used);
    printf("Fuel Cost: %.2f LKR\n", delivery->fuel_cost);
    printf("Operational Cost: %.2f LKR\n", delivery->operational_cost);
    printf("Profit: %.2f LKR\n", delivery->profit);
    printf("Customer Charge: %.2f LKR\n", delivery->customer_charge);
    printf("Estimated Time: %.2f hours\n", delivery->delivery_time);
    printf("==============================================================\n");
    
    delivery_count++;
    
    printf("\nDelivery recorded successfully! Delivery ID: %d\n", delivery->id);
}

float findMinimumDistance(int source, int destination) {
    float direct_distance = distances[source][destination];
    float min_distance = direct_distance;
    
    // For small number of cities, try to find better routes
    if(city_count <= 4) {
        int available_cities[MAX_CITIES];
        int path_count = 0;
        
        // Collect all cities except source and destination
        int i;
        for( i = 0; i < city_count; i++) {
            if(i != source && i != destination) {
                available_cities[path_count++] = i;
            }
        }
        
        if(path_count > 0) {
            int best_path[MAX_CITIES];
            int best_path_length = 0;
            
            generatePermutations(available_cities, 0, path_count-1, source, destination, 
                               &min_distance, best_path, &best_path_length);
        }
    }
    
    return min_distance;
}

void generatePermutations(int cities[], int start, int end, int source, int destination, 
                         float *min_distance, int best_path[], int *best_path_length) {
    if(start == end) {
        // Create full path: source -> permutation -> destination
        int path[MAX_CITIES];
        int length = 0;
        
        path[length++] = source;
        int i;
        for(i= 0; i <= end; i++) {
            path[length++] = cities[i];
        }
        path[length++] = destination;
        
        float path_distance = calculatePathDistance(path, length);
        
        if(path_distance < *min_distance && path_distance > 0) {
            *min_distance = path_distance;
            // Store best path (optional for display)
            int i;
            for(i = 0; i < length; i++) {
                best_path[i] = path[i];
            }
            *best_path_length = length;
        }
    } else {
    	int i;
        for(i = start; i <= end; i++) {
            swap(&cities[start], &cities[i]);
            generatePermutations(cities, start+1, end, source, destination, 
                               min_distance, best_path, best_path_length);
            swap(&cities[start], &cities[i]);
        }
    }
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

float calculatePathDistance(int path[], int length) {
    float total_distance = 0;
    int i;
    for(i = 0; i < length - 1; i++) {
        if(distances[path[i]][path[i+1]] == -1) {
            return -1; // Invalid path
        }
        total_distance += distances[path[i]][path[i+1]];
    }
    
    return total_distance;
}













