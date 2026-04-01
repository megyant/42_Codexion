# 42_Codexion
Solving the dining philosopher's problem

Dining philosophers:
- https://www.youtube.com/watch?v=zOpzGHwJ3MU
- https://suspectedoceano.notion.site/Philosophers-b1bf3c57eee6420cafa7d0900b3d3216
- https://f0rkr.github.io/dining-philosophers-problem/
- https://medium.com/@ridwaneelfilali/philosophers-the-dinning-problem-8ea3c0fc8cc7
- https://github.com/DeRuina/philosophers
- https://github.com/zelhajou/ft_unix_philosophers
- https://www.youtube.com/watch?v=UGQsvVKwe90
- https://medium.com/@jalal92/the-dining-philosophers-7157cc05315

  
On threads and deadlocks:
   1. threads:  
    - https://pages.mtu.edu/~shene/NSF-3/e-Book/index.html   
    - https://www.youtube.com/playlist?list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2  
    - https://medium.com/@jalal92/lets-discuss-threads-grab-a-coffee-ad4d4ebf7181   
   2. deadlocks:  
    - https://www.youtube.com/watch?v=rFYYifVj5gs  
    - https://begriffs.com/posts/2020-03-23-concurrent-programming.html  
    - https://cs341.cs.illinois.edu/coursebook/Deadlock#resource-allocation-graphs
    - https://www.youtube.com/watch?v=_ruovgwXyYs  
   3. both:  
    - https://www.youtube.com/watch?v=mvZKu0DfFLQ  
    - https://www.codequoi.com/en/threads-mutexes-and-concurrent-programming-in-c/

  
  
  
AI workflow guidance:  
Since you have your t_args struct ready, here is a logical workflow to tackle the rest of the project step-by-step.  
1. Data Structure Expansion

Your t_args holds the rules, but you need structs to hold the state of the simulation. Define these before writing any logic.

    t_coder: Needs an ID, thread ID (pthread_t), references to their left and right dongles, a timestamp of their last compile start, their total compile count, and a pointer to the shared simulation state.

    t_dongle: Needs a mutex, a condition variable (pthread_cond_t), an availability status, a timestamp for when its cooldown expires, and a pointer to its specific priority queue.

    t_queue: A struct for your heap implementation to manage waiting coders.

    t_shared: The central brain. It should hold the t_args, arrays of t_coder and t_dongle, a simulation-active flag (to signal threads to stop), a mutex for logging (to prevent interleaved text), and a mutex for reading/writing shared simulation states.

2. Time Management and Logging Utility

Precise timing is a strict requirement for this project. Build your utility functions early.

    Write a function to fetch the current time in milliseconds using gettimeofday().

    Create a thread-safe logging function that takes the coder ID and the action string. It must lock the logging mutex, check if the simulation is still active (to avoid printing after a burnout), print the formatted string with the current timestamp, and unlock the mutex.

    Write an exact usleep() wrapper. Standard usleep() can wake up early or late; you will likely need a function that checks gettimeofday() in a small loop to ensure accurate delays for compiling, debugging, and refactoring.

3. Priority Queue (Heap) Implementation

The subject explicitly requires a priority queue (heap) for the fifo and edf scheduling. Build and test this in isolation before hooking it up to threads.

    Implement the core heap operations: push, pop, and peek.

    Create comparison logic based on the scheduler argument. For fifo, compare the arrival timestamp of the requests. For edf, compare last_compile_start + time_to_burnout.

4. Initialization Phase

Write the setup functions to breathe life into your structs.

    Allocate memory for your arrays of coders and dongles based on number_coders.

    Initialize all dongle mutexes, condition variables, and queues.

    Assign the correct left and right dongle IDs to each coder. Remember the circular nature: coder N shares with N-1 and N+1, and the last coder shares with the first.

5. Core Thread Routines

This is the skeleton of your simulation logic.

    The Coder Routine: A loop that runs while the simulation is active. The flow should be: Request Dongles -> Compile -> Release Dongles -> Debug -> Refactor.

    The Monitor Routine: A single, separate thread that loops very fast through all coders. It checks if current_time - last_compile_start >= time_to_burnout. If true, it locks the state, sets the simulation to inactive, prints the burnout message, and breaks. It also checks if all coders have reached total_compiles.

6. Resource Management (The Trickiest Part)

This is where you implement the dongle acquisition and release, utilizing your priority queue and condition variables.

    Acquiring: A coder pushes themselves onto the left and right dongle queues. They use pthread_cond_wait (or pthread_cond_timedwait) inside a loop, waiting until they are at the top of the queue and the dongle's cooldown has expired.

    Releasing: The coder pops themselves from the queues, sets the dongle's cooldown expiration timestamp (current_time + dongle_cooldown), and signals the condition variable (pthread_cond_broadcast) to wake up other coders waiting for that dongle.

7. Thread Launch and Teardown

Bring it all together.

    Create the monitor thread.

    Create the coder threads.

    Use pthread_join to wait for all threads to finish once the monitor detects an end condition.

    Clean up by destroying all mutexes, condition variables, and freeing allocated memory to ensure zero leaks.







Diving into multithreading for the first time is a massive paradigm shift! It is completely normal to feel a bit overwhelmed because you are moving from a world where one thing happens at a time to a world where everything happens at once.

Just to ground things a bit: "data structure expansion" isn't a formal programming term. What we are really talking about here is state modeling for concurrent systems. You are designing containers (structs) to organize data into two distinct categories: things a thread keeps to itself (local state) and things multiple threads need to look at or change (shared state).

Here are the best resources to help you wrap your head around structuring data for threads in C:
1. Video Tutorials (Highly Recommended for Beginners)

    Jacob Sorber’s YouTube Channel: This is arguably the best visual resource for C programming and threading. Search for his videos on "pthreads in C," "mutexes," and "passing arguments to threads." He writes code on screen and explains exactly why structs need to be formatted in certain ways to pass data safely into pthread_create.

    CodeVault on YouTube: Another fantastic channel that breaks down the Dining Philosophers problem (which your assignment is based on), mutexes, and condition variables with excellent animations and C code examples.

2. Free Online Textbooks & Guides

    OSTEP (Operating Systems: Three Easy Pieces): This is a free, highly readable online textbook. You want to read the section on Concurrency. Specifically, the chapters on "Locks" and "Condition Variables" will explain exactly why your t_dongle struct needs a mutex and how threads interact with shared data.

    LLNL POSIX Threads Programming Tutorial: This is the classic, comprehensive written guide provided by the Lawrence Livermore National Laboratory. It is a bit dense, but it is the ultimate cheat sheet for how functions like pthread_create and pthread_mutex_lock actually work with your structs.

3. How to Think About Your Structs Right Now

While you check out those resources, here is the mental model you should use to build your structs:

    The Blueprint (t_args): You already built this! It holds the rules of the simulation that never change.

    Thread-Local Data (t_coder): Think of this as a coder's backpack. When you launch a thread, you hand it this struct. It should only contain what that specific coder needs to know: their own ID, pointers to their specific left and right dongles, and when they last compiled.

    Shared Data (t_dongle & t_shared): Think of this as the table in the middle of the room. Because multiple coders will reach for the same dongles or try to write to the same log, everything on this "table" needs a padlock (a pthread_mutex_t).