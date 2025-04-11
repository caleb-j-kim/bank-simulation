-- 4/8/25 - 12:55 am --
* Initialized the repository and planned the steps of how to build the project.
* Read project instructions and understood what was asked of me before beginning to write the code.
* Developing the file structure:
- main.cpp will create the threads and starts the simulation.
- BankSimulation.cpp / .h contains helper functions for low coupling and high cohesion and greater efficiency for the overall project.
- Customer.cpp / .h contains customer thread functionalities.
- Teller.cpp / .h contains teller thread functionalities.
- Semaphore.cpp / .h contains implementation of the Semaphore class. 
* Future plans: Import the semaphore.cpp / .h and code the BankSimulation.cpp / .h

-- 4/8/25 - 8:03 pm --
* Imported the Semaphore.cpp / .h from project instructions.
- Create a .gitignore so the semaphore doesn't get pushed into the repository as only functionality that uses this code is required.
* Constructed BankSimulation.h next as the customer and teller files will need to call helper functions from this file.
* Future plans: Code the BankSimulation.cpp and work on the Teller.cpp / .h next.

-- 4/10/25 - 8:48 pm --
* Updated devlog to contain future plans after each session as per assignment requirements.
* For convenience, removed the .gitignore of the semaphore.cpp / .h files to make running the project take way less time.
* Created a README.me that briefly explains what the project is and what it hopes to accomplish.
- Explained the concepts of threads and POSIX semaphores are to better understand what I need to code for the project and for those who are less technologically knowledgable to comprehend how the project works.
- I felt it was better to work on the README.me before continuing on with the code so I can ensure that I thoroughly understand what I am doing before I create threads and use the Semaphore class.
* Future plans: Code the BankSimulation.cpp and work on the Teller.cpp / .h next.

-- 4/10/25 - 9:03 pm --
* Implemented the BankSimulation.cpp by referencing how the BankSimulation.h referenced everything that was required.
* Future plans: Code the Teller.cpp / .h files next.

