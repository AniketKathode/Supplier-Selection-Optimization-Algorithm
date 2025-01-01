#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

int O_Ano = 0;
int distanceMatrix[5][5];
class Supplier;

class Order {
public:
    int orderID;
    int processingTime;
    int weight;
    bool assigned;
    Supplier* assignedSupplier; 

    Order(int orderID, int processingTime, int weight) {
        this->orderID = orderID;
        this->processingTime = processingTime;
        this->weight = weight;
        this->assigned = false;
        this->assignedSupplier = nullptr;
    }
};

class Batch {
public:
    int batchID;
    int priority;
    vector<Order*> orders;

    Batch(int batchID, int priority) {
        this->batchID = batchID;
        this->priority = priority;
    }

    void addOrder(Order* order) {
        orders.push_back(order);
    }
};

class Supplier {
public:
    int supplierID;
    int availabilityTime;
    vector<Order*> orders;
    vector<int> distanceToSuppliers;

    Supplier(int supplierID, int availabilityTime) {
        this->supplierID = supplierID;
        this->availabilityTime = availabilityTime;
    }

    void addOrder(Order* order) {
        orders.push_back(order);
    }

    void updateAvailabilityTime(int processingTime) {
        availabilityTime += processingTime;
    }
};

class Vehicle {
public:
    int vehicleID;
    int speed;
    int remainingCapacity;
    vector<Order*> orders;
    vector<int> distanceToSuppliers;

    Vehicle(int vehicleID, int speed) : vehicleID(vehicleID), speed(speed), remainingCapacity(15) {}

    void addOrder(Order* order) {
        orders.push_back(order);
        remainingCapacity -= order->weight;
    }
};

void readDistanceMatrixFromFile(const string& filename, int distanceMatrix[5][5]) {
    ifstream file(filename);

    if (file.is_open()) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                file >> distanceMatrix[i][j];
            }
        }

        file.close();
    } else {
        cerr << "Error opening file: " << filename << endl;
    }
}

vector<Order*> readOrdersFromFile(const string& filename) {
    vector<Order*> orders;
    ifstream file(filename);

    if (file.is_open()) {
        int orderID, processingTime, weight;

        while (file >> orderID >> processingTime >> weight) {
            orders.push_back(new Order(orderID, processingTime, weight));
        }

        file.close();
    } else {
        cerr << "Error opening file: " << filename << endl;
    }

    return orders;
}

vector<Batch*> readBatchesFromFile(const string& filename) {
    vector<Batch*> batches;
    ifstream file(filename);

    if (file.is_open()) {
        int batchID, priority;

        while (file >> batchID >> priority) {
            batches.push_back(new Batch(batchID, priority));
        }

        file.close();
    } else {
        cerr << "Error opening file: " << filename << endl;
    }

    return batches;
}

vector<Supplier*> readSuppliersFromFile(const string& filename) {
    vector<Supplier*> suppliers;
    ifstream file(filename);

    if (file.is_open()) {
        int supplierID, availabilityTime;

        while (file >> supplierID >> availabilityTime) {
            suppliers.push_back(new Supplier(supplierID, availabilityTime));
        }

        file.close();
    } else {
        cerr << "Error opening file: " << filename << endl;
    }

    return suppliers;
}

vector<Vehicle*> readVehiclesFromFile(const string& filename) {
    vector<Vehicle*> vehicles;
    ifstream file(filename);

    if (file.is_open()) {
        int vehicleID, speed;

        while (file >> vehicleID >> speed) {
            vehicles.push_back(new Vehicle(vehicleID, speed));
        }

        file.close();
    } else {
        cerr << "Error opening file: " << filename << endl;
    }

    return vehicles;
}

Supplier* findAvailableSupplier(Order* order, vector<Supplier*>& suppliers);
void assignVehiclesToOrders(const vector<Order*>& orders, vector<Vehicle*>& vehicles, vector<Supplier*>& suppliers);
int calculateTotalTime(vector<Vehicle*>& vehicles, vector<Supplier*>& suppliers);
int calculateDistance(int vehicleID, int supplierID);

vector<vector<Order*>> generateAllPossibleCombinationsWithinBatches(vector<Order*>& orders, vector<Batch*>& batches);
vector<vector<Batch*>> generateAllPossiblePermutations(vector<Batch*>& batches);
void generatePermutations(vector<Batch*> currentPermutation, int index, vector<vector<Batch*>>& batchPermutations);

int main() {
	
    string distanceMatrixFile = "C:/Users/sujjit/Desktop/Code/distanceMatrix.txt";
    string ordersFile = "C:/Users/sujjit/Desktop/Code/orders.txt";
    string batchesFile = "C:/Users/sujjit/Desktop/Code/batches.txt";
    string suppliersFile = "C:/Users/sujjit/Desktop/Code/suppliers.txt";
    string vehiclesFile = "C:/Users/sujjit/Desktop/Code/vehicles.txt";

    // Creating distance matrix
    readDistanceMatrixFromFile(distanceMatrixFile, distanceMatrix);

    vector<Order*> orders = readOrdersFromFile(ordersFile);
    vector<Batch*> batches = readBatchesFromFile(batchesFile);
    vector<Supplier*> suppliers = readSuppliersFromFile(suppliersFile);
    vector<Vehicle*> vehicles = readVehiclesFromFile(vehiclesFile);

    // Setting distances between Suppliers using the distance matrix
    for (int i = 0; i < 5; i++) {
        suppliers[i]->distanceToSuppliers.resize(5);

        for (int j = 0; j < 5; j++) {
            if (j < suppliers[i]->distanceToSuppliers.size()) {
                suppliers[i]->distanceToSuppliers[j] = distanceMatrix[i][j];
            }
        }
    }

    // Setting distances between Vehicles and Suppliers using the distance matrix
    for (int i = 0; i < 4; i++) {
        vehicles[i]->distanceToSuppliers.resize(5);

        for (int j = 0; j < 5; j++) {
            if (j < vehicles[i]->distanceToSuppliers.size()) {
                vehicles[i]->distanceToSuppliers[j] = distanceMatrix[vehicles[i]->vehicleID - 1][j];
            }
        }
    }

    // Generating all possible Order allocations within Batches
    vector<vector<Order*>> orderAllocations = generateAllPossibleCombinationsWithinBatches(orders, batches);

    int minimumTotalTime = INT_MAX;

    for (const vector<Order*>& orderAllocation : orderAllocations) {
        O_Ano++;
        cout << "Order Allocation: " << O_Ano << endl;
        for (Order* order : orderAllocation) {
            cout << "Order " << order->orderID << " ";
        }
        cout << endl;

        // Assigning Orders to Available Suppliers
        for (Order* order : orderAllocation) {
            Supplier* availableSupplier = findAvailableSupplier(order, suppliers);
            if (availableSupplier != nullptr) {
                availableSupplier->addOrder(order);
                cout << "Order " << order->orderID << " assigned to Supplier " << availableSupplier->supplierID << endl;
                // Simulate processing time
            } else {
                cout << "No available supplier for Order " << order->orderID << endl;
            }
        }

        assignVehiclesToOrders(orderAllocation, vehicles, suppliers);

        int totalTime = calculateTotalTime(vehicles, suppliers);

        cout << "Total time for combination: " << totalTime << endl;

        for (Vehicle* vehicle : vehicles) {
            cout << "Vehicle " << vehicle->vehicleID << " assigned to Orders: ";
            for (Order* order : vehicle->orders) {
                cout << order->orderID << " ";
            }
            cout << endl;
        }

        cout << "-------------------------------------------" << endl;

        // Updating minimum total time
        if (totalTime < minimumTotalTime) {
            minimumTotalTime = totalTime;
        }

        // Clearing Orders assigned to Suppliers for the next iteration
        for (Supplier* supplier : suppliers) {
            supplier->orders.clear();
        }
    }

    cout << "Optimal Total Time: " << minimumTotalTime << endl;

    return 0;
}

Supplier* findAvailableSupplier(Order* order, vector<Supplier*>& suppliers) {
    // Supplier with the least availability time gets the Order
    sort(suppliers.begin(), suppliers.end(), [](const Supplier* a, const Supplier* b) {
        return a->availabilityTime < b->availabilityTime;
    });

    Supplier* availableSupplier = suppliers.front();

    // Updating the Supplier's availability time to whom the Order is allocated
    if (availableSupplier != nullptr) {
        availableSupplier->availabilityTime += order->processingTime;
    }

    return availableSupplier;
}

void assignVehiclesToOrders(const vector<Order*>& orders, vector<Vehicle*>& vehicles, vector<Supplier*>& suppliers) {
    vector<Order*> unassignedOrders = orders;

    for (Vehicle* vehicle : vehicles) {
        Order* nearestOrder = nullptr;
        int minDistance = INT_MAX;
        Supplier* supplier = nullptr;

        for (Order* order : unassignedOrders) {
            supplier = findAvailableSupplier(order, suppliers);
            if (supplier == nullptr) {
                continue;
            }

            int distance = calculateDistance(vehicle->vehicleID, supplier->supplierID);
            if (distance < minDistance && order->weight <= vehicle->remainingCapacity) {
                nearestOrder = order;
                minDistance = distance;
            }
        }

        if (nearestOrder != nullptr) {
            vehicle->addOrder(nearestOrder);
            nearestOrder->assigned = true;
            nearestOrder->assignedSupplier = supplier;
            cout << "Order " << nearestOrder->orderID << " assigned to Supplier " << supplier->supplierID << endl;
        }
    }
}

int calculateTotalTime(vector<Vehicle*>& vehicles, vector<Supplier*>& suppliers) {
    int totalTime = 0;

    for (Vehicle* vehicle : vehicles) {
        int vehicleTime = 0;

        if (!vehicle->orders.empty()) {
            int travelTimeToFirstSupplier = vehicle->distanceToSuppliers[vehicle->orders[0]->assignedSupplier->supplierID] / vehicle->speed;
            vehicleTime += travelTimeToFirstSupplier;
        }

        for (Order* order : vehicle->orders) {
            Supplier* supplier = findAvailableSupplier(order, suppliers);

            int travelTimeToSupplier = vehicle->distanceToSuppliers[supplier->supplierID] / vehicle->speed;
            int processingTime = order->processingTime;
            int travelTimeBack = vehicle->distanceToSuppliers[supplier->supplierID] / vehicle->speed;

            vehicleTime += travelTimeToSupplier + processingTime + travelTimeBack;
        }

        totalTime += vehicleTime;
    }

    return totalTime;
}


int calculateDistance(int vehicleID, int supplierID) {
    return distanceMatrix[vehicleID - 1][supplierID - 1];
}

// Function to generate the next combination of indices
bool next_combination(std::vector<int>& indices, int n, int k) {
    return std::next_permutation(indices.begin(), indices.begin() + k);
}

// Generating the possible combinations of Orders for each Batch
vector<vector<Order*>> generateAllPossibleCombinationsWithinBatches(vector<Order*>& orders, vector<Batch*>& batches) {
    vector<vector<Order*>> orderAllocations;

    for (Batch* batch : batches) {
        int batchSize = min(static_cast<int>(orders.size()), 8);
        vector<Order*> batchOrders(orders.begin(), orders.begin() + batchSize);

        do {
            orderAllocations.push_back(batchOrders);
        } while (next_permutation(batchOrders.begin(), batchOrders.end()));

        orders.erase(orders.begin(), orders.begin() + batchSize);
    }

    return orderAllocations;
}
