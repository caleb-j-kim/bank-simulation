/*  Caleb Kim
    4/10/2025
    CS 4348.501
    Main file that starts the simulation and creates the threads.
*/

#include "BankSimulation.h"
#include "Customer.h"
#include "Teller.h"
#include <iostream> // for printing messages to the console.
#include <thread> // for creating threads for each teller and customer.
#include <vector> // for storing threads in a vector.

int main() {
    std::cout << "Starting bank simulation with " << NUM_TELLERS << " bank tellers and " << NUM_CUSTOMERS << " customers." << std::endl;

    // Create bank teller threads
    std::vector<std::thread> tellers;
    for (int i = 0; i < NUM_TELLERS; i++) {
        tellers.emplace_back(tellerThread, i); // Create a thread for each teller.
    }

    // Create customer threads
    std::vector<std::thread> customers;
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        customers.emplace_back(customerThread, i);
    }

    // Wait for all customers to finish
    for (auto &cust : customers) {
        if (cust.joinable()) {
            cust.join(); // Wait for each customer thread to finish.
        }
    }

    // Wake up any teller threads that are waiting for customers
    {
        std::unique_lock<std::mutex> lock(lineMtx);
        lineCV.notify_all();
    }

    for (auto &t : tellers) {
        if (t.joinable()) {
            t.join(); // Wait for each teller thread to finish.
        }
    }

    std::cout << "Bank simulation is complete." << std::endl;
    return 0;
}