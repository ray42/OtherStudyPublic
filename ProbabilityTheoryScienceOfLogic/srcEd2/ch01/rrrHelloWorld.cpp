/*
 * =========================================================================
 * Copyright (c) 2018, Raymon White - All Rights Reserved
 * Author:  Raymon White (ray), rayaxiom42 AT gmail DOT com
 * =========================================================================
 */
#include<iostream>
#include<thread>

void hello()
{
  std::cout << "HelloWorld from thead!\n";
}

int main()
{
  std::thread t(hello);
  t.join();
  return 0;
}

