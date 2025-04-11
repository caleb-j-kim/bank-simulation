/*  Caleb Kim
    4/10/2025
    CS 4348.501
    File that contains all code regarding what Customers would be doing inside of this simulation (represented as threads).
*/

#include "Customer.h"
#include "BankSimulation.h"
#include <iostream> // for printing messages to the console.
#include <thread> // for creating threads for each teller.
#include <random> // for generating random numbers for transaction types and sleep times.

void customerThread(int customerID) {
    static thread_local std::mt19937 rng{ std::random_device{}() }; // Random number generator for each thread.
    std::uniform_int_distribution<int> distType(0, 1); // 0 for deposit, 1 for withdrawal.
    bool isWithdrawal = (distType(rng) == 1); // Randomly decide if this is a withdrawal or deposit transaction.
    randomSleep(0, 100); // Simulate time taken to enter the bank.

    // Wait for the bank to open.
    {
        std::unique_lock<std::mutex> lock(bankOpenMtx);

        while (!bankOpen) {
            bankOpenCV.wait(lock);
        }
    }

    // Enter the bank (door permits 2 at a time max).
    doorSem.wait();
    safePrint("Customer #" + std::to_string(customerID) + " [Door]: enters the bank.");
    randomSleep(1, 5);
    doorSem.signal(); // Signal that the customer has entered the bank.

    // Join the customer queue.
    {
        std::unique_lock<std::mutex> lock(lineMtx);
        customerQueue.push(customerID); // Add the customer to the queue.
        lineCV.notify_one();
        
        if (freeTellers <= 0)
            safePrint("Customer #" + std::to_string(customerID) + " [Line]: is waiting in line.");
    }

    // Wait to be assigned a bank teller.
    int assignedTeller = -1;
    while (true) {
        bool found = false;

        for (int t = 0; t < NUM_TELLERS; t++) {
            std::unique_lock<std::mutex> tLock(tellerMtx[t]);

            if (tellerCurrentCustomer[t] == customerID) {
                assignedTeller = t; // Assign the teller to the customer.
                found = true;
                break; // Break out of the loop if the teller is found.
            }
        }

        if (found) break;
        std::this_thread::yield(); // Yield to allow other threads to run.
    }

    safePrint("Customer #" + std::to_string(customerID) + " [Teller #" + std::to_string(assignedTeller) + "]: is being served.");
    safePrint("Customer #" + std::to_string(customerID) + " [Teller #" + std::to_string(assignedTeller) + "]: Greets one another.");
    randomSleep(1, 5); // Simulate time taken to greet the teller.
    std::string tx = (isWithdrawal ? "Withdrawal" : "Deposit"); // Set the transaction type.
    safePrint("Customer #" + std::to_string(customerID) + " [Teller #" + std::to_string(assignedTeller) + "]: Requests for a " + tx + ".");

    // Wait for the teller to finish the transaction.
    while (true) {
        std::unique_lock<std::mutex> tLock(tellerMtx[assignedTeller]);

        if (tellerCurrentCustomer[assignedTeller] == -1) {
            break; // Bank teller is done with the transaction.
        }

        tLock.unlock(); // Unlock the teller mutex to allow other threads to access it.
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Customer waits for the teller to finish.
    }

    safePrint("Customer #" + std::to_string(customerID) + " [Door]: Leaves the bank.");
}