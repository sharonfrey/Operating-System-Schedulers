# Operating-System-Schedulers

The goal for this project is to learn more about OS scheduling through a hands-on simulation programming experience. The programs were built dynamically such that any number of processes could be computed within the programs without any issues. The schedulers will be implemented as follows.

**First Come First Serve**. The First Come First Serve Scheduler will have a non-preemptive algorithm.

**Shortest Job First**. The Shortest Job First Scheduler will have a non-preemptive algorithm.

**Multi-Level Feedback Queue**. The Mulit-Level Feedback Queue Scheduler will have a preemptive algorithm. It will consist three queues with absolute priority in the higher queues. A process in a lower queue will be interrupted if a process enters a higher queue, however the preempted process will not be downgraded to a lower queue. 
   - Queue 1. The first queue will use a Round Robin Scheduler with a time quantum of five time units. 
   - Queue 2. The second queue will use a Round Robin Scheduler with a time quantum of ten time units. 
   - Queue 3. The third queue will use a First Come First Serve Scheduler.


The schedulers will compute the following eight processes given with the process data consisting of CPU burst times and I/O burst times. Each process will enter the queue in order at execution time 0.

**P1** {5, 27, 3, 31, 5, 43, 4, 18, 6, 22, 4, 26, 3, 24, 4}

**P2** {4, 48, 5, 44, 7, 42, 12, 37, 9, 76, 4, 41, 9, 31, 7, 43, 8} 

**P3** {8, 33, 12, 41, 18, 65, 14, 21, 4, 61, 15, 18, 14, 26, 5, 31, 6} 

**P4** {3, 35, 4, 41, 5, 45, 3, 51, 4, 61, 5, 54, 6, 82, 5, 77, 3} 

**P5** {16, 24, 17, 21, 5, 36, 16, 26, 7, 31, 13, 28, 11, 21, 6, 13, 3, 11, 4} 

**P6** {11, 22, 4, 8, 5, 10, 6, 12, 7, 14, 9, 18, 12, 24, 15, 30, 8} 

**P7** {14, 46, 17, 41, 11, 42, 15, 21, 4, 32, 7, 19, 16, 33, 10} 

**P8** {4, 14, 5, 33, 6, 51, 14, 73, 16, 87, 6} 
