/*
 * =========================================================================
 * Copyright (c) 2018, Raymon White - All Rights Reserved
 * Author:  Raymon White (ray), rayaxiom42 AT gmail DOT com
 * =========================================================================
 */
#include<iostream>
#include<thread>
#include<chrono>

void f1(int n)
{
  std::cout << "Thread 1 id: " << std::this_thread::get_id() << '\n';
  for(int i = 0; i < 5; ++i)
  {
    std::cout << "Thread 1 executing\n";
    ++n;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void f2(int& n)
{
  std::cout << "Thread 2 id: " << std::this_thread::get_id() << '\n';
  for(int i = 0; i < 5; ++i)
  {
    std::cout << "Thread 2 executing\n";
    ++n;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

int main()
{
  int n = 0;
  std::thread t1; // t1 is not a thread
  std::thread t2(f1,n+1); // pass by value
  std::thread t3(f2,std::ref(n)); // pass by reference
  std::cout << "Moving t2\n";
  // t4 is now running f2(). t3 is no longer a thread.
  std::thread t4(std::move(t3));

  t2.join();
  t4.join();

  std::cout << "Final value of n is " << n << '\n';
  return 0;
}

