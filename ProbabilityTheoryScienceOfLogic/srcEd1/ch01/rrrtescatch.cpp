/*
 * =========================================================================
 * Copyright (c) 2017, Raymon White - All Rights Reserved
 * Author:  Raymon White (ray), rayaxiom42 AT gmail DOT com
 * =========================================================================
 */

#define CATCH_CONFIG_MAIN // this tells catch to provide  main() - only do
                          // this in one cpp file
#include<iostream>
using std::cout; using std::endl;
#include "./../common/catch.hpp"

int Factorial(int number)
{
  return number <= 1? number : Factorial(number-1)*number;
}

TEST_CASE("Factorials are computed", "[factorial]")
{
  REQUIRE(Factorial(0) == 1);
  REQUIRE(Factorial(1) == 1);
  REQUIRE(Factorial(2) == 2);
  REQUIRE(Factorial(3) == 6);
  REQUIRE(Factorial(10) == 3628800);
}


