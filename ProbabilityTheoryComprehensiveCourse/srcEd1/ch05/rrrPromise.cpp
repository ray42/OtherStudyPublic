/*
 * =========================================================================
 * Copyright (c) 2018, Raymon White - All Rights Reserved
 * Author:  Raymon White (ray), rayaxiom42 AT gmail DOT com
 * =========================================================================
 */
#include<vector>
#include<iostream>
#include<thread>
#include<future>
#include<numeric>
#include<chrono>

void accumulate(std::vector<int>::iterator first,
                std::vector<int>::iterator last,
                std::promise<int> accumulate_promise)
{
  int sum = std::accumulate(first,last,0);
  accumulate_promise.set_value(sum);       // Notify future
}

void do_work(std::promise<void> barrier)
{
  std::this_thread::sleep_for(std::chrono::seconds(1));
  barrier.set_value();
}

auto main()->int
{
  // Demonstrate using promise<int> to transmit a result between threads
  std::vector<int> numbers = {1,2,3,4,5,6};

  // Set up a promise and... 
  std::promise<int> accumulate_promise;
  // ... associate a future with it (via the shared state)
  std::future<int> accumulate_future = accumulate_promise.get_future();
  // Now do the work in a separate thread, which sets the promise value
  std::thread work_thread(accumulate,numbers.begin(),numbers.end(),
                          std::move(accumulate_promise));
  // Wait for the result.
  accumulate_future.wait();
  std::cout << "result = " << accumulate_future.get() << '\n';
  // wait for thread completion
  work_thread.join();


  // Demonstrate using promise<void> to signal state between threads
  // Set up promise and associated future
  std::promise<void> barrier;
  std::future<void> barrier_future = barrier.get_future();
  // Do the work on a thread.
  std::thread new_work_thread(do_work,std::move(barrier));
  // Wait for promise to finish and then join thread.
  barrier_future.wait();
  new_work_thread.join();

  return 0;
}

