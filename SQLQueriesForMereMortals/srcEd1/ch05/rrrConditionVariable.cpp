/*
 * =========================================================================
 * Copyright (c) 2018, Raymon White - All Rights Reserved
 * Author:  Raymon White (ray), rayaxiom42 AT gmail DOT com
 * =========================================================================
 */
#include<iostream>
#include<string>
#include<thread>
#include<mutex>
#include<condition_variable>

// Shared mutex, condition_variable and data
std::mutex m;
std::condition_variable cv;
std::string data;

// Set by main to signal to worker thread
bool ready = false;

// Set by worker thread to signal to main
bool processed = false;

void worker_thread()
{
  // First this thread is doing the waiting. Three steps are:
  // 1) acquire a std::unique_lock<std::mutex>, on the same mutex as used to
  //    protect the shared variable, "data".
  //
  // 2) execute wait, wait_for, or wait_until. The wait operations 
  //    atomically release the mutex and suspend the execution of the thread.
  //
  // 3) When the condition variable is notified, a timeout expires, or a 
  //    spurious wakeup occurs, the thread is awakened, and the mutex is 
  //    atomically reacquired. The thread should then check the condition 
  //    and resume waiting if the wake up was spurious.
  //
  // Wait until main() sends data
  std::unique_lock<std::mutex> lk(m);

  // Putting the wait in a while loop to avoid spurious wake, see point
  // 3 on the waiting thead
  while(!ready)
  {
    cv.wait(lk,[]()
               {
                 return ready;
               });
  }

  // after the wait, we own the lock, so writing to the shared variable 
  // "data" is safe.
  std::cout << "Worker thread is processing data\n";
  data+= "after processing";

  //////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////
  // RAYRAY - now this thread is doing the modifying, main() is doing the 
  // waiting. The steps are:
  // 1) acquire a std::mutex (typically via std::guard) - already done.
  // 2) perform the modification while the lock is held
  // 3) execute notify_one or notify_all on the std::condition variable
  
  // Step2:
  // Send data back to main()
  processed = true;
  std::cout << "Worker thread signals data processing completed\n";

  // Step 3:
  // Manual unlocking is done before notifying, to avoid waking up the
  // waiting thread only to block again (see notify_one for details).
  lk.unlock();
  cv.notify_one();
}


int main()
{
  // First, the main thread is doing the modifying, worker is waiting, then
  // we flip it around.

  std::thread worker(worker_thread);
  // At the point, the worker is waiting on the condition variable.

  // prepare data.
  data = "Example data";
  // send data to the worker thread (i.e. notify the worker thread that the
  // data is ready).
  // Three steps:
  // 1) acquire a std::mutex (typically via std::lock_guard)
  // 2) perform the modification while the lock is held
  // 3) execute notify_one or notify_all on the std::condition_variable
  {
    // step1
    std::lock_guard<std::mutex> lk(m);

    // step2 this is the variable that the conditional variable check.
    ready = true;
    std::cout << "main() signals data ready for processing\n";
    // ^ this should really go next to notify_one
    // mutex is automatically unlocked.
  }
  cv.notify_one();


  // Wait for the worker.
  // Three steps:
  // 1) acquire a std::unique_lock<std::mutex>, on the same mutex as used to
  //    protect the shared variable
  // 2) execute wait, wait_for, or wait_until. The wait operations 
  //    atomically release the mutex and suspend the execution of the 
  //    thread.
  // 3) When the condition variable is notified, a timeout expires, or a 
  //    spurious wakeup occurs, the thread is awakened, and the mutex is 
  //    atomically reacquired. The thread should then check the condition 
  //    and resume waiting if the wake up was spurious.
  {
    //1
    std::unique_lock<std::mutex> lk(m);
    while(!processed) // 3 - check for spurious wakes
    {
      cv.wait(lk,[]()
                 {
                   return processed;
                 });
    }
  }

  std::cout << "Back in main(), data = " << data << '\n';

  worker.join();

  return 0;
}

