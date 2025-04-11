/*  Caleb Kim
    4/10/2025
    CS 4348.501
    File that contains all code regarding what Bank Tellers would be doing inside of this simulation (represented as threads).
*/

#include "Teller.h"
#include "BankSimulation.h"
#include <iostream> // for printing messages to the console.
#include <thread> // for creating threads for each teller.
#include <random> // for generating random numbers for transaction types and sleep times.

void tellerThread(int tellerID) {
    {
    // Signal readiness: when all bank tellers are ready, open the bank.
    std::unique_lock<std::mutex> lock(bankOpenMtx);
    tellerReadyCount++;

    if (tellerReadyCount == NUM_TELLERS) {
        bankOpen = true;
        bankOpenCV.notify_all(); // Notify all tellers that the bank is open.
    }
}
    safePrint("Bank Teller #" + std::to_string(tellerID) + " [Bank Teller #" + std::to_string(tellerID) + "]: is Ready to Serve Customers!");

    while (customersServed.load() < NUM_CUSTOMERS) {
        int customerID = -1;
        {
            // Wait for a customer to arrive.
            std::unique_lock<std::mutex> lock(lineMtx);

            while (customerQueue.empty() && (customersServed.load() < NUM_CUSTOMERS)) {
                lineCV.wait(lock); // Wait for a customer to arrive.
            }

            if  (customerQueue.empty() && (customersServed.load() >= NUM_CUSTOMERS)) {
                break; // Exit if the bank is closed and no customers are left.
            }

            customerID = customerQueue.front(); // Get the next customer in line.
            customerQueue.pop(); // Remove the customer from the queue.
            freeTellers--; // Decrease the number of free tellers.
        }

        {
            std::unique_lock<std::mutex> tLock(tellerMtx[tellerID]);
            tellerCurrentCustomer[tellerID] = customerID; // Assign the customer to the teller.
            tellerCV[tellerID].notify_one();
        }

        safePrint("Teller #" + std::to_string(tellerID) + ": [Customer " + std::to_string(customerID) + "]: requests for a transaction.");
        randomSleep(5, 10);
        randomSleep(5, 10);

        // Randomly decide if this is a withdrawal or deposit transaction.
        bool isWithdrawal = false;

        {
            static thread_local std::mt19937 rng{ std::random_device{}() }; // Random number generator for each thread.
            std::uniform_int_distribution<int> dist(0, 1); // 0 for deposit, 1 for withdrawal.

            if (dist(rng) == 1) {
                isWithdrawal = true; // Set the transaction type to withdrawal.
            }
        }

        if (isWithdrawal) { 
            safePrint("Teller #" + std::to_string(tellerID) + " [Manager]: I'm going to the manager.");
            managerSem.wait();
            safePrint("Teller #" + std::to_string(tellerID) + " [Manager]: I'm with the manager.");
            randomSleep(5, 30);
            safePrint("Teller #" + std::to_string(tellerID) + " [Manager]: I'm done with the manager.");
            managerSem.signal();
        }

        safePrint("Teller #" + std::to_string(tellerID) + " [Safe]: I'm going to the safe.");
        safePrint("Teller #" + std::to_string(tellerID) + " [Safe]: I'm using the safe.");
        safeSem.wait(); // Wait for access to the safe.
        randomSleep(5, 30); // Simulate time spent in the safe.
        safePrint("Teller #" + std::to_string(tellerID) + " [Safe]: I'm done using the safe.");
        safeSem.signal(); // Signal that the teller is done with the safe.

        // If the transaction is deposit then nothing additional needs to be done.
        safePrint("Teller #" + std::to_string(tellerID) + " [Customer " + std::to_string(customerID) + "]: Transaction is completed."); 
        randomSleep(2, 5);

        {
            std::unique_lock<std::mutex> tLock(tellerMtx[tellerID]);
            tellerCurrentCustomer[tellerID] = -1; // Mark the teller as available.
            tellerCV[tellerID].notify_one(); // Notify the teller that they are available.
        }

        int servedCount = 1 + customersServed.fetch_add(1);
        if (servedCount >= NUM_CUSTOMERS) {
            break;
        }

        {
            std::unique_lock<std::mutex> lock(lineMtx);
            freeTellers++; // Increase the number of free tellers.
            lineCV.notify_one(); // Notify a teller that a customer is waiting in line
        }
    }

    safePrint("Teller #" + std::to_string(tellerID) + " [Teller " + std::to_string(tellerID) + "]: is done for the day.");
}