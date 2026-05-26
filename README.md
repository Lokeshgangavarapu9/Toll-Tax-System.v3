# Toll Tax Management System v3.0 (AI Automated)

We developed a terminal-based Toll Tax Management System using C++ and Object-Oriented Programming concepts for our SRM University-AP project. The system is designed to simulate a modern AI-based “stop-free” toll plaza that automates toll collection and improves security.

The project follows important OOP principles such as:

* Inheritance
* Polymorphism
* Encapsulation
* Abstraction


### Main Features of the Project

1. AI-Based License Plate Recognition
   The system simulates dual-camera number plate scanning. It checks both front and rear number plates to detect fake or tampered vehicles.

2. Central Database Verification
   The scanned vehicle number is checked with a simulated national database to identify:

* Stolen vehicles
* Vehicles with unpaid fines

3. AI Weight Fraud Detection
   The system compares the selected vehicle type with the actual vehicle weight.
   For example:

* If a user selects “Car” but the weight sensor detects a heavy truck, the system flags it as fraud.

4. Automatic FASTag Billing
   The toll amount is automatically deducted from the user’s FASTag wallet without stopping the vehicle.
   An SMS receipt is also generated after successful payment.

5. File Handling and Data Storage
   Using C++ `<fstream>`, the project stores:

* Vehicle logs
* Revenue records
* Fraud detection reports
* Security alerts

### Technical Implementation

* Financial data and wallet balances are protected using private static variables.
* Runtime polymorphism is implemented using base class pointers (`Vehicle*`).
* Dynamic memory allocation is handled safely without memory leaks.
* The system processes every vehicle through:
  Camera Scan → Database Verification → Weight Check → Toll Deduction

### How to Run the Project

Compile the program using a C++11 compiler:

```bash
g++ -std=c++11 -Wall -o TollTax TollTax_v3.cpp
./TollTax
```

### Project Objective

The main objective of this project is to demonstrate how AI automation and Object-Oriented Programming can be used together to build a secure, efficient, and smart toll management system for real-world applications.
