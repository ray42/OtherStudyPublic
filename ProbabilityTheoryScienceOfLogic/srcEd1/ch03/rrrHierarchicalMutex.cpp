/*
 * =========================================================================
 * Copyright (c) 2018, Raymon White - All Rights Reserved
 * Author:  Raymon White (ray), rayaxiom42 AT gmail DOT com
 * =========================================================================
 */
#include<iostream>
using std::cout; using std::endl;
#include<mutex>
#include<thread>
#include<climits>

// p52
class hierarchical_mutex
{
  std::mutex internal_mutex{};

  // set by constructor argument
  unsigned long const hierarchy_value;

  // IDK why I need this, init to zero by constructor
  unsigned long previous_hierarchy_value;

  // hierarchy value of the thread, local to this thread, but it's static
  // This keeps track of the smallest hierarchy value locked by this thread
  // so far.
  static thread_local unsigned long this_thread_hierarchy_value; //1

  void check_for_hierarchy_violation()
  {
    if(this_thread_hierarchy_value <= hierarchy_value)           //2
    {
      throw std::logic_error("mutex hierarchy violated");
    }
  }
  void update_hierarchy_value()
  {
    // Why do we need to store the previous thread value? IDK....
    previous_hierarchy_value = this_thread_hierarchy_value;      //3
    this_thread_hierarchy_value = hierarchy_value;
  }

public:
  explicit hierarchical_mutex(unsigned long value):
    hierarchy_value(value),
    previous_hierarchy_value(0)
  {}

  void lock()
  {
    check_for_hierarchy_violation();
    internal_mutex.lock();                                       //4
    update_hierarchy_value();                                    //5
  }

  void unlock()
  {
    // OHH, so we re-set the thread hierarchy value when we unlock it. 
    // Makes sense.
    this_thread_hierarchy_value = previous_hierarchy_value;      //6
    internal_mutex.unlock();
  }

  bool try_lock()
  {
    check_for_hierarchy_violation();
    
    if(!internal_mutex.try_lock())                               //7
      return false;

    update_hierarchy_value();
    return true;
  }
};

thread_local unsigned long
  hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);    //8



int g_i = 0;
std::mutex g_i_mutex; // protects g_i

void hello()
{
  std::thread::id this_id = std::this_thread::get_id();
  std::cout << "Hello from thread " << this_id << '\n';
}


void safe_increment()
{
  std::lock_guard<std::mutex> lock(g_i_mutex);
  ++g_i;

  std::cout << std::this_thread::get_id() << ": " << g_i << '\n';

  // g_i_mutex is automatically released when lock
  // goes out of scope
}

int main()
{
  std::thread threadhello(hello);
  threadhello.join();

  std::cout << "main: " << g_i << '\n';
  std::thread t1(safe_increment);
  std::thread t2(safe_increment);

  t1.join();
  t2.join();
  std::cout << "main: " << g_i << '\n';

  return 0;
}

