# Codexion

*This project has been created as part of the 42 curriculum by <mbotelho>mbotelho.*  

## Description
A multi-threaded simulation exploring POSIX concurrency and resource management. In this project, coder threads compete for limited USB dongles to complete tasks before their burnout threshold (deadline) is reached.

### The codexion problem

### Threads


### Deadlocks

### How blocking cases have been handled

### Thread sincronization mechanisms

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
./codexion 5 800 200 200 200 200 60 edf
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