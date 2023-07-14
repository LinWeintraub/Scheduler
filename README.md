# Scheduler
The Scheduler is a library that provides functionality for scheduling and executing tasks based on their priority. It utilizes a priority queue implemented using a heap data structure.

# Directory Structure
The relevant files for the Scheduler library are located in the scheduler directory:

* **pq_heap:** Directory containing the source and header files for the priority heap implementation.
* **task:** Directory containing the source and header files for the task.
* **uid:** Directory containing the source and header files for the unique identifier (UID) generator.
  
* **scheduler.c:** Source file containing the implementation of the Scheduler library.
* **scheduler.h:** Header file defining the interface and function prototypes for the Scheduler library.
* **scheduler_test.c:** Test file for unit testing the Scheduler library.

# Functionality
The Scheduler library provides the following functionality:

* **Task Scheduling:** You can create tasks with priorities, schedule them, and execute them based on their priority using the Scheduler library.
* **Priority Queue Implementation:** The Scheduler library utilizes a priority queue implemented as a binary heap to efficiently manage and prioritize tasks.
* **Unique Identifier Generation:** The library includes a UID (Unique Identifier) generator to assign unique identifiers to tasks.

# Functions
The Scheduler library provides the following functions:

* **SchedulerCreate:** Creates a new scheduler instance.
* **SchedulerDestroy:** Destroys the scheduler and releases the allocated memory.
* **SchedulerAddTask:** Adds a new task to the scheduler.
* **SchedulerRemoveTask:** Removes a task from the scheduler based on its unique identifier.
* **SchedulerClear:** Removes all tasks from the scheduler.
* **SchedulerSize:** Retrieves the number of tasks in the scheduler.
* **SchedulerIsEmpty:** Checks if the scheduler is empty.
* **SchedulerRun:** Runs the scheduler and executes the tasks based on their priority and execution time.
* **SchedulerStop:** Stops the scheduler from executing further tasks.

# Usage
To use the Scheduler library, follow these steps:

1. Include the ``scheduler.h`` header file in your C program.
2. Link the ``scheduler.c`` source file along with the relevant dependencies (such as pq_heap, task, and uid) with your program.
3. Build and compile your program.

# Contributing
Contributions to the Scheduler library implementation are welcome. If you find any issues or want to improve the existing implementation, feel free to open a pull request.
