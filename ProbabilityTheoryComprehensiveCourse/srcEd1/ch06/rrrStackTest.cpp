/*
 * =========================================================================
 * Copyright (c) 2018, Raymon White - All Rights Reserved
 * Author:  Raymon White (ray), rayaxiom42 AT gmail DOT com
 * =========================================================================
 */
#include<iostream>
using std::cout; using std::endl;

#include<exception>
#include<stack> // stack
#include<mutex> //mutex, lock_guard
#include<memory>// shared_ptr
#include<thread>
#include<future> // future, async
#include<string>

struct empty_stack: std::exception
{
  const char* what() const throw();
};

template<typename T>
class threadsafe_stack
{
private:
  std::stack<T> data;
  mutable std::mutex m;

public:
  // Default constructor
  threadsafe_stack(){}
  
  // copy constructor
  threadsafe_stack(const threadsafe_stack& other)
  {
    std::lock_guard<std::mutex> lock(other.m);
    data = other.data;
  }

  // copy assignment operator, deleted for simplicity since mutexes are not
  // copyable.
  threadsafe_stack& operator=(const threadsafe_stack&) = delete;

  // push
  void push(T new_value)
  {
    std::lock_guard<std::mutex> lock(m);
    data.push(std::move(new_value));        //1
  }

  // pop
  std::shared_ptr<T> pop()
  {
    std::lock_guard<std::mutex> lock(m);
    
    if(data.empty()) throw empty_stack();   //2

    std::shared_ptr<T> const res(
      std::make_shared<T>(std::move(data.top()))); //3

    data.pop();                             //4
    return res;
  }
  
  // Another pop
  void pop(T& value)
  {
    std::lock_guard<std::mutex> lock(m);
    if(data.empty()) throw empty_stack();

    value = std::move(data.top());          //5
    data.pop();                             //6
  }

  bool empty() const
  {
    std::lock_guard<std::mutex> lock(m);
    return data.empty();
  }

  size_t size() const
  {
    std::lock_guard<std::mutex> lock(m);
    return data.size();
  }

//  void print(const std::string& str= "") const
//  {
//    if(!str.empty())
//      std::cout << str;
//
//    {
//      std::lock_guard<std::mutex> lock(m);
//      for(const auto& i:data)
//      {
//        cout << i << ' ';
//      }
//    }
//  }

};

threadsafe_stack<int> mystack;

void testThread1()
{
  mystack.push(41);
}

void testThread2()
{
  mystack.push(42);
}

auto main()->int
{
  auto fut1 = std::async(testThread1);
  fut1.wait();
  std::thread thr2(testThread2);
  thr2.join();

  std::cout << mystack.size() << std::endl;

  return 0;
}

