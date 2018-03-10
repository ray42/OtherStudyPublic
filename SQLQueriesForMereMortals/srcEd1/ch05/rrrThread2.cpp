/*
 * =========================================================================
 * Copyright (c) 2018, Raymon White - All Rights Reserved
 * Author:  Raymon White (ray), rayaxiom42 AT gmail DOT com
 * =========================================================================
 */
#include<iostream>
#include<thread>
#include<string>

class bar{
public:
  void foo(const std::string& str=""){
    std::cout << str;
  }
  std::thread spawn(){
    return std::thread(&bar::foo, this,"From spawn\n");
  }
};
int main()
{
  std::thread t1(&bar::foo, bar{}, "From main\n");
  std::thread t2 = bar{}.spawn();
  t1.join();
  t2.join();
  return 0;
}

