/*  Caleb Kim
    4/8/2025
    CS 4348.501
    Header file for BankSimulation functions.
*/

#ifndef BANKSIMULATION_H
#define BANKSIMULATION_H

#include "semaphore.h"
#include <atomic> // for atomic counter that safely increments by multiple threads without additional locking.
#include <mutex> // for declaring mutexes protecting protected variables like printMtx (for safe printing) along with various locks needed for teller.
#include <condition_variable> 
#include <queue> // for container of customers waiting for service.
#include <thread> // for simulating delays and random transaction types (deposits, withdrawals, etc.).
#include <string>

// Constants
const int NUM_TELLERS = 3;
const int NUM_CUSTOMERS = 50;

// Gloabl semaphores
extern Semaphore doorSem; // semaphore for door access + only two customers at a time may enter the bank.
extern Semaphore managerSem; // semaphore for manager access + only one teller can interact with the manager at a time.
extern Semaphore safeSem; // semaphore for safe access + only two tellers can be in the safe at once.

// Global counters and flags
extern std::atomic<int> customersServed;

// Synchonrize bank opening
extern std::mutex bankOpenMtx; 
extern std::condition_variable bankOpenCV; // condition variable for bank opening.
extern bool bankOpen;
extern int tellerReadyCount;

// Manage customers waiting in line
extern std::mutex lineMtx; // mutex for protecting the queue of customers waiting in line.
extern std::queue<int> customerQueue;
extern int freeTellers;
extern std::condition_variable lineCV; // condition variable for notifying tellers when customers are waiting in line.
extern std::condition_variable tellerFreeCV; // condition variable for notifying customers when a teller is available.

// Each teller's current customer (-1 if none)
extern int tellerCurrentCustomer[NUM_TELLERS];
extern std::mutex tellerMtx[NUM_TELLERS]; // mutexes for protecting each teller's current customer.
extern std::condition_variable tellerCV[NUM_TELLERS]; // condition variables for notifying tellers when they can serve their current customer.

// For printing output safely
extern std::mutex printMtx;

// Helper functions
void safePrint(const std::string &msg); // function for printing messages safely using the printMtx mutex.
void randomSleep(int min, int max); // function for simulating random sleep times between min and max milliseconds.

#endif