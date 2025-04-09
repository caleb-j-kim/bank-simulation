-- 4/8/25 - 12:55 am --
* Initialized the repository and planned the steps of how to build the project.
* Read project instructions and understood what was asked of me before beginning to write the code.
* Developing the file structure:
- main.cpp will create the threads and starts the simulation.
- BankSimulation.cpp / .h contains helper functions for low coupling and high cohesion and greater efficiency for the overall project.
- Customer.cpp / .h contains customer thread functionalities.
- Teller.cpp / .h contains teller thread functionalities.
- Semaphore.cpp / .h contains implementation of the Semaphore class. 

-- 4/8/25 - 8:03 pm --
* Imported the Semaphore.cpp / .h from project instructions.
- Create a .gitignore so the semaphore doesn't get pushed into the repository as only functionality that uses this code is required.
* Constructed BankSimulation.h next as the customer and teller files will need to call helper functions from this file.