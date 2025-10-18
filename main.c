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


int main()
{
    printf("Hello world!\n");
    return 0;
}




