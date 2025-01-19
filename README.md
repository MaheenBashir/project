Project Title
Advanced Flight Booking System

Overview
The project is a comprehensive Flight Booking System implemented in C++. It uses an AVL Tree for efficient flight price searches, handles flight bookings with waiting lists, and supports notifications, recent searches, and flight additions. Key features include:

Flight Management: Adding new flights and managing flight details.
Search Functionality: Find flights by route and price range.
Booking System: Book flights and manage a waiting list.
AVL Tree Integration: Efficient price-based flight search.
Notification and Undo Management: Notify users of updates and manage recent actions.
Installation Instructions
Requirements:

A C++ compiler supporting C++11 or later (e.g., GCC, Clang, or Visual Studio).
Basic knowledge of running C++ programs.
Steps:

Save the provided code to a file named FlightBookingSystem.cpp.
Open a terminal or IDE with a C++ compiler.
Compile the code using the command:
bash
Copy
Edit
g++ -o FlightBookingSystem FlightBookingSystem.cpp
Run the compiled program:
bash
Copy
Edit
./FlightBookingSystem
Usage Guide
Menu Options:

1. Add Flight: Add a new flight with details like ID, source, destination, time, seats, and price.
2. Search Flights: Search for flights by source, destination, and maximum price.
3. Book Flight: Book a flight using its ID and passenger ID.
4. View Recent Searches: View the 10 most recent search queries.
5. Process Notifications: View system notifications such as booking confirmations.
0. Exit: Exit the program.
Example Commands:

Adding a flight:
Flight ID: 101
Source City: New York
Destination City: London
Departure Time: 10:00
Total Seats: 200
Price: 850.50

Searching flights:
Source City: New York
Destination City: London
Maximum Price: 900.00
Results
Example Outputs:
Available Flights:
Flight 101 | Time: 10:00 | Available Seats: 199 | Price: $850.50

Booking Confirmation:
Booking confirmed! Flight ID: 101

Recent Searches:
- New York->London
 
Notifications:
- New flight added: New York to London

Code Organization
File Structure:
src/: Main code file (FlightBookingSystem.cpp).
data/: Not directly used but can store external flight or booking datasets.
results/: Store logs, screenshots, or output files as needed.
requirements.txt: Not applicable but dependencies include:
Standard Template Library (STL) components such as vector, list, stack, and queue.
