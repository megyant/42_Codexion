# Codexion

*This project has been created as part of the 42 curriculum by <mbotelho>mbotelho.*  

## Description
A multi-threaded simulation exploring POSIX concurrency and resource management. In this project, coder threads compete for limited USB dongles to complete tasks before their burnout threshold (deadline) is reached.

### The codexion problem

The Codexion problem is a specialized adaptation of the Dining Philosophers problem. Both serve as classic models for illustrating concurrent algorithm design and complex synchronization challenges.

In this simulation, five coders are positioned around a circular hub. There are several critical constraints:

- **Resource Scarcity:** There are only five dongles available in total. To perform a compile task, a coder must successfully acquire two dongles (typically the one to their left and the one to their right).

- **The Workflow:** Each coder must follow a strict, sequential routine: Compiling → Debugging → Refactoring.

- **Resource Availability:** Dongles are released and become available to neighbors only when a coder transitions into the debugging or refactoring phases.

- **The Burnout Mechanic:** Time is a finite resource. If a coder remains idle or stuck in a waiting state for too long without starting a compile, they "burn out," resulting in an immediate simulation failure.

The objective of this project is to implement a management system that orchestrates these threads efficiently, ensuring every coder completes their required cycles while avoiding deadlocks and preventing burnout.

### Threads
A thread is a component of a process, the smallest sequence of instructions that can be independently managed by a scheduler. Multiple threads can be executed at the same time, threads of the same process share its code and values of allocated variables. All threads share resources, such as memory.  
  
This architecture enables high-efficiency multitasking. While a single-core CPU achieves concurrency through rapid context switching (interleaving instructions to simulate simultaneous execution), multi-core systems achieve true parallelism by distributing threads across physical hardware units. Modern processors further optimize this through Simultaneous Multithreading (SMT), allowing multiple execution threads to saturate a single physical core's pipeline, maximizing throughput in resource-heavy applications.

### Deadlocks
Deadlock is a state in which a set of concurrent threads is permanently stalled because each thread is waiting for a resource held by another thread in the same set. This creates a circular chain of dependencies where no entity can progress.

Such scenarios are a primary challenge in multithreaded systems with constrained resources, particularly in this simulation where coders must acquire multiple dongles simultaneously. This project addresses deadlock potential by systematically breaking the necessary conditions for their occurrence, as detailed below.

### How blocking cases have been handled
 
 **Deadlock Prevention**  
mutexes

 **Starvation Prevention**  
queue management

 **Burnout Detection**  
 monitor

 **Log serialization**  
print mutex

### Thread sincronization mechanisms

**Wait thread creation**

**Mutexes**

## Instructions

### Instalation
Clone this repository and build it using make
```
git@github.com:megyant/42_Codexion.git codexion
cd codexion
make
```

### Usage

Run the executable with the following positional arguments:
```
./codexion <num_coders> <burnout> <compile> <debug> <refactor> <req_compiles> <cooldown> <scheduler>
```

**Example**:
```
./codexion 5 800 200 200 200 2 60 edf
```

### Argument details
|   Movement               |  Description                                                                                                      |
| -------------------      | ---------------------------------------------------------------------------------------------------               |
| number_of_coders         | The number of coders and also the number of dongles.                                                              |
| time_to_burnout(ms)      | Maximum interval allowed between the start of consecutive compile phases before a thread terminates.              |
| time_to_compile(ms)      | Task duration requiring the simultaneous acquisition of two dongle resources.                                     |
| time_to_debug(ms)        | Duration of the post-compile processing phase.                                                                    |
| number_compiles_required | Global threshold for successful simulation completion.                                                            |
| dongle_cooldown(ms)      | Mandatory latency period during which a released resource remains unavailable for requisition.                    |
| scheduler(edf/fifo)      | Arbitration logic determining resource allocation via FIFO (arrival order) or EDF (proximity to burnout deadline).|

## Resources
### Dining philosophers:
- [The dining Philosophers in C: threads, race conditions and deadlocks](https://www.youtube.com/watch?v=zOpzGHwJ3MU)
- [Philosophers](https://suspectedoceano.notion.site/Philosophers-b1bf3c57eee6420cafa7d0900b3d3216)
- [Dining philosophers problem](https://f0rkr.github.io/dining-philosophers-problem/)
- [Philosophers: The dinning problem](https://medium.com/@ridwaneelfilali/philosophers-the-dinning-problem-8ea3c0fc8cc7)
- [Philosophers 42 Explained / Walkthrough](https://github.com/DeRuina/philosophers)
- [Philosophers Project](https://github.com/zelhajou/ft_unix_philosophers)
- [Philosophers, 42 School Project](https://www.youtube.com/watch?v=UGQsvVKwe90)
- [The dining Philosophers](https://medium.com/@jalal92/the-dining-philosophers-7157cc05315)
- [Dining-Philosophers Problem: Simplified](https://www.youtube.com/watch?v=VSkvwzqo-Pk)
  
### Threads, mutexes and deadlocks:
   - [Multithreaded Programming with ThreadMentor: A Tutorial](https://pages.mtu.edu/~shene/NSF-3/e-Book/index.html)
   - [Unix Threads in C](https://www.youtube.com/playlist?list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2)
   - [Exploring the Practical Use of Threads with Starbucks](https://medium.com/@jalal92/lets-discuss-threads-grab-a-coffee-ad4d4ebf7181)  
   - [What is a deadlock? C code ELI5](https://www.youtube.com/watch?v=rFYYifVj5gs)
   - [Concurrent programming, with examples](https://begriffs.com/posts/2020-03-23-concurrent-programming.html)  
   - [Deadlock - CS431](https://cs341.cs.illinois.edu/coursebook/Deadlock#resource-allocation-graphs)
   - [Threads, Mutexes and Concurrent Programming in C](https://www.codequoi.com/en/threads-mutexes-and-concurrent-programming-in-c/)

### Use of Artificial Intelligence
Gemini was used to optimize the development workflow of this project. Some usages included:
- Assist with testing the program
- Logical improvement of functions
- Improve understanding of some concepts
- Quick and small debug checks througout the making of this program
- Improve this README wording

All algorithm and implementation of the logic in this program are the author's own work.

## IMPORTANT MESSAGE

DO NOT COPY OR CHEAT, YOU WILL NOT BE HELPING YOURSELF.