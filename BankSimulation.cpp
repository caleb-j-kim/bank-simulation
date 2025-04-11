/*  Caleb Kim
    4/10/2025
    CS 4348.501
    Helper file that houses helper functions that will be used by the Teller and Customer files.
    (Explanations on why certain packages are being used are included in the header file.)
*/

#include "BankSimulation.h"
#include <iostream> // for printing messages to the console.
#include <random>
#include <chrono>
#include <thread>

// Initialize semaphores (using provided constructor of Semaphore class)
Semaphore doorSem(2);
Semaphore managerSem(1);
Semaphore safeSem(2); // only two bank tellers can be in the safe at once.

// Global counters and flags
std::atomic<int> customersServed(0);
std::mutex bankOpenMtx;
std::condition_variable bankOpenCV;

bool bankOpen = false;
int tellerReadyCount = 0;

// Waiting line for customers
std::mutex lineMtx;
std::queue<int> waitingCustomers;
int freeTellers = NUM_TELLERS;
std::condition_variable lineCV;
std::condition_variable tellerFreeCV;

// Teller customer assignment arrays
int tellerCurrentCustomer[NUM_TELLERS] = {-1, -1, -1}; // -1 means no customer is currently being served
std::mutex tellerMtx[NUM_TELLERS];
std::condition_variable tellerCV[NUM_TELLERS];

// For safe printing
std::mutex printMtx;

void safePrint(const std::string &msg) {
    std::lock_guard<std::mutex> lock(printMtx); // lock the mutex for safe printing
    std::cout << msg << std::endl; // print the message to the console
}

void randomSleep(int min, int max) { // function to simulate random sleep times between min and max milliseconds to simulate customer wait times and teller service times.
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> distr(min, max); // define the range

    std::this_thread::sleep_for(std::chrono::milliseconds(distr(eng))); // sleep for a random duration between min and max milliseconds
}