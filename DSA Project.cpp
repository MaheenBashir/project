#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <list>
#include <algorithm>
#include <unordered_map>

using namespace std;

struct Flight 
{
    int flightId;
    string source;
    string destination;
    string departureTime;
    int totalSeats;
    int availableSeats;
    double price;
    vector<int> waitingList;
    
    Flight(int id, string src, string dest, string time, int seats, double p) 
	{
        flightId = id;
        source = src;
        destination = dest;
        departureTime = time;
        totalSeats = seats;
        availableSeats = seats;
        price = p;
    }
};

struct AVLNode 
{
    double price;
    vector<Flight*> flights;
    AVLNode* left;
    AVLNode* right;
    int height;
    
    AVLNode(double p, Flight* f) : price(p), left(nullptr), right(nullptr), height(1) 
	{
        flights.push_back(f);
    }
};

class AVLTree 
{
private:
    AVLNode* root;
    
    int getHeight(AVLNode* node) 
	{
        return node ? node->height : 0;
    }
    
    int getBalance(AVLNode* node) 
	{
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }
    
    void updateHeight(AVLNode* node) 
	{
        if (node)
		{
            node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        }
    }
    
    AVLNode* rightRotate(AVLNode* y) 
	{
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;
        
        x->right = y;
        y->left = T2;
        
        updateHeight(y);
        updateHeight(x);
        
        return x;
    }
    
    AVLNode* leftRotate(AVLNode* x) 
	{
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;
        
        y->left = x;
        x->right = T2;
        
        updateHeight(x);
        updateHeight(y);
        
        return y;
    }
    
    AVLNode* insertNode(AVLNode* node, Flight* flight) 
	{
        if (!node) 
		{
            return new AVLNode(flight->price, flight);
        }
        
        if (flight->price < node->price) 
		{
            node->left = insertNode(node->left, flight);
        }
        else if (flight->price > node->price) 
		{
            node->right = insertNode(node->right, flight);
        }
        else 
		{
            node->flights.push_back(flight);
            return node;
        }
        
        updateHeight(node);
        
        int balance = getBalance(node);
        int l_balance = getBalance(node->left);
        int r_balance = getBalance(node->right);
        
        if (balance > 1 && l_balance >= 0) 
		{
            return rightRotate(node);
        }
        
        if (balance < -1 && r_balance <= 0) 
		{
            return leftRotate(node);
        }
        
        if (balance > 1 && l_balance < 0) 
		{
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        
        if (balance < -1 && r_balance > 0) 
		{
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        
        return node;
    }
    
    void findInRange(AVLNode* node, double minPrice, double maxPrice, vector<Flight*>& results) 
	{
        if (!node) return;
        
        if (node->price >= minPrice && node->price <= maxPrice) 
		{
            results.insert(results.end(), node->flights.begin(), node->flights.end());
        }
        
        if (minPrice < node->price) 
		{
            findInRange(node->left, minPrice, maxPrice, results);
        }
        
        if (maxPrice > node->price) 
		{
            findInRange(node->right, minPrice, maxPrice, results);
        }
    }
    
public:
    AVLTree() : root(nullptr) {}
    
    void insert(Flight* flight) 
	{
        root = insertNode(root, flight);
    }
    
    vector<Flight*> searchPriceRange(double minPrice, double maxPrice) 
	{
        vector<Flight*> results;
        findInRange(root, minPrice, maxPrice, results);
        return results;
    }
};

class FlightBookingSystem 
{
private:
    unordered_map<int, Flight*> flightMap;
    unordered_map<string, vector<Flight*>> routeMap;
    AVLTree priceTree;
    list<string> recentSearches;
    stack<string> undoStack;
    queue<string> notificationQueue;
    
    string createRouteKey(const string& src, const string& dest) 
	{
        return src + "->" + dest;
    }

public:
    void addFlight(Flight* flight) 
	{
        flightMap[flight->flightId] = flight;
        string routeKey = createRouteKey(flight->source, flight->destination);
        routeMap[routeKey].push_back(flight);
        priceTree.insert(flight);
        
        undoStack.push("Added flight " + to_string(flight->flightId));
        notificationQueue.push("New flight added: " + flight->source + " to " + 
                             flight->destination);
    }
    
    static bool compareFlightsByPrice(Flight* a, Flight* b)
    {
      	return a->price < b->price;
	}
    
    void searchFlights(const string& src, const string& dest, double maxPrice) 
	{
        recentSearches.push_front(createRouteKey(src, dest));
        if (recentSearches.size() > 10) 
		{
            recentSearches.pop_back();
        }
        
        string routeKey = createRouteKey(src, dest);
        vector<Flight*> routeFlights = routeMap[routeKey];
        vector<Flight*> priceRangeFlights = priceTree.searchPriceRange(0, maxPrice);
        
        vector<Flight*> availableFlights;
        for (Flight* flight : routeFlights) 
		{
            if (find(priceRangeFlights.begin(), priceRangeFlights.end(), flight) 
                != priceRangeFlights.end() && flight->availableSeats > 0) 
			{
                availableFlights.push_back(flight);
            }
        }
        
        sort(availableFlights.begin(), availableFlights.end(), compareFlightsByPrice);
        
        cout << "\nAvailable Flights (sorted by price):\n";
        for (Flight* flight : availableFlights) 
		{
            cout << "Flight " << flight->flightId 
                 << " | Time: " << flight->departureTime
                 << " | Available Seats: " << flight->availableSeats
                 << " | Price: $" << flight->price << endl;
        }
    }
    
    void bookFlight(int flightId, int passengerId) 
	{
        auto it = flightMap.find(flightId);
        if (it == flightMap.end()) 
		{
            cout << "Flight not found!" << endl;
            return;
        }
        
        Flight* flight = it->second;
        if (flight->availableSeats > 0) 
		{
            flight->availableSeats--;
            cout << "Booking confirmed! Flight ID: " << flightId << endl;
            notificationQueue.push("New booking: Flight " + to_string(flightId));
        }
        else 
		{
            flight->waitingList.push_back(passengerId);
            cout << "Flight full. Added to waiting list." << endl;
        }
    }
    
    void processNotifications() 
	{
        cout << "\nPending Notifications:\n";
        while (!notificationQueue.empty()) 
		{
            cout << "- " << notificationQueue.front() << endl;
            notificationQueue.pop();
        }
    }
    
    void showRecentSearches() 
	{
        cout << "\nRecent Searches:\n";
        for (const string& search : recentSearches) 
		{
            cout << "- " << search << endl;
        }
    }
};

main() 
{  
    FlightBookingSystem system;
    
    Flight* flight1 = new Flight(101, "New York", "London", "10:00", 200, 850.50);
    Flight* flight2 = new Flight(102, "London", "Paris", "14:30", 150, 220.75);
    Flight* flight3 = new Flight(103, "Paris", "Rome", "16:45", 180, 175.25);
    Flight* flight4 = new Flight(104, "New York", "London", "18:00", 200, 750.00);
    Flight* flight5 = new Flight(105, "Tokyo", "Seoul", "09:15", 160, 445.00);
    Flight* flight6 = new Flight(106, "Dubai", "Mumbai", "13:20", 190, 380.25);
    Flight* flight7 = new Flight(107, "Singapore", "Bangkok", "11:30", 170, 290.75);
    Flight* flight8 = new Flight(108, "New York", "London", "22:45", 200, 920.50);
    
    system.addFlight(flight1);
    system.addFlight(flight2);
    system.addFlight(flight3);
    system.addFlight(flight4);
    system.addFlight(flight5);
    system.addFlight(flight6);
    system.addFlight(flight7);
    system.addFlight(flight8);
    
    cout << "\nInitial System Tests:\n";
    cout << "\nSearching flights from New York to London under $900:\n";
    system.searchFlights("New York", "London", 900.0);
    
    cout << "\nMaking some test bookings:\n";
    system.bookFlight(101, 1001);  
    system.bookFlight(104, 1002);  
    
    cout << "\nChecking recent searches:\n";
    system.showRecentSearches();
    
    cout << "\nChecking notifications:\n";
    system.processNotifications();
    
    int choice = 0;
    
    do 
	{
        cout << "\n Advanced Flight Booking System \n";
        cout << "1. Add Flight\n";
        cout << "2. Search Flights\n";
        cout << "3. Book Flight\n";
        cout << "4. View Recent Searches\n";
        cout << "5. Process Notifications\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        
        cin >> choice;
        cin.ignore();  
        
        switch(choice) 
		{
            case 1: 
			{
                int flightId;
                string source, destination, departureTime;
                int totalSeats;
                double price;
                
                cout << "Enter Flight ID: ";
                cin >> flightId;
                cin.ignore();
                
                cout << "Enter Source City: ";
                getline(cin, source);
                
                cout << "Enter Destination City: ";
                getline(cin, destination);
                
                cout << "Enter Departure Time (HH:MM): ";
                getline(cin, departureTime);
                
                cout << "Enter Total Seats: ";
                cin >> totalSeats;
                
                cout << "Enter Price: ";
                cin >> price;
                cin.ignore();
                
                Flight* newFlight = new Flight(flightId, source, destination,
                                             departureTime, totalSeats, price);
                system.addFlight(newFlight);
                cout << "Flight added successfully!\n";
                break;
            }
            
            case 2: 
			{
                string source, destination;
                double maxPrice;
                
                cout << "Enter Source City: ";
                getline(cin, source);
                
                cout << "Enter Destination City: ";
                getline(cin, destination);
                
                cout << "Enter Maximum Price: ";
                cin >> maxPrice;
                cin.ignore();
                
                system.searchFlights(source, destination, maxPrice);
                break;
            }
            
            case 3: 
			{
                int flightId, passengerId;
                
                cout << "Enter Flight ID: ";
                cin >> flightId;
                
                cout << "Enter Passenger ID: ";
                cin >> passengerId;
                cin.ignore();
                
                system.bookFlight(flightId, passengerId);
                break;
            }
            
            case 4:
            {
         	  	system.showRecentSearches();
               	break;
			}
                
            case 5:
            {
                system.processNotifications();
               	break;
			}
                
            case 0:
            {
            	cout << "Thank you for using the Flight Booking System!\n";
                break;
			}
                
            default:
            {
               	cout << "Invalid choice! Please try again.\n";
			}
        }
        
    } while (choice != 0);
}
