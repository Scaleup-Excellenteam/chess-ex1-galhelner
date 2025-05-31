**************************************************
*   C++ Course - Chess Part 3 (MultiThreading)   *
*                                                *
*   Created By: Gal Helner                       *
*                                                *
**************************************************

MultiThreading Algorithm Explanation:
--------------------------------------
In my algorithm that finds the five highly recommended moves I scan
for each piece it's all possible moves.
So, I created a thread pool that contains 8 threads (for each piece)
and now the algorithm for every piece could run concurrently.

Since all the threads are access the same priority queue to store
the recommended moves, I used a mutex lock to ensure that only one
thread at a time is able to access the shared priority queue.
This way, I can avoid cases of race condition between separate threads
that can lead to unexpected behavior.


Automatic Game and Time Measurements Explanation:
--------------------------------------------------
I added to the Chess class two methods:

1. runAutomaticGame - This method runs an automatic 8 moves game
which uses my move recommendation algorithm to decide what move to do.

2. measureGameTimes - This method runs automatic games with 2,4,8 threads,
measure each running time and display the results.

Here's an example for the output of a running measurement I did:
Measurement Results:
Running time for 2 threads: 1267.97
Running time for 4 threads: 1087.7
Running time for 8 threads: 2753.04

Surprisingly, running with 8 threads took more time than 2 or 4,
there might be some reasons for that behavior:

* Thread Overhead - creating and managing more threads introduces overhead.
* Shared Resource Contention - all threads push moves to a shared priority queue protected by a mutex.
* Small Workload per Thread - each thread might not do enough work to justify its own existence.