/*
 * =========================================================================
 * Copyright (c) 2018, Raymon White - All Rights Reserved
 * Author:  Raymon White (ray), rayaxiom42 AT gmail DOT com
 * =========================================================================
 */
#include<iostream>
#include<vector>
#include<algorithm>
#include<numeric>
#include<future>

template<typename RandomIt>
int parallel_sum(RandomIt beg, RandomIt end)
{
  auto len = end - beg;
  std::cout << "Entered parallel_sum\n";
  if(len < 1000)
    return std::accumulate(beg,end,0);

  RandomIt mid = beg + len/2;
  // right half recursively on on async thread
  auto handle = std::async(std::launch::async,
                           parallel_sum<RandomIt>, mid, end);

  // left half recursively on this thread
  int sum = parallel_sum(beg,mid);
  return sum+=handle.get();
}

int main()
{
  std::vector<int> v(1000,1);
  std::cout << "The sum is " << parallel_sum(v.begin(),v.end()) << '\n';
  return 0;
}

