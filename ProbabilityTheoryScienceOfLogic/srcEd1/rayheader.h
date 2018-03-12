#ifndef RAYHEADER_H
#define RAYHEADER_H

#include<iostream>
#include<iterator>
#include<vector>
#include<string>
#include<cstddef>

namespace ray
{
  template<typename It> 
  void print_sequence(It beg, It end)
  {
    for(It it = beg; it != end; ++it)
    {
      std::cout << *it << " ";
    }
    std::cout << std::endl;
  }

  template<typename T, std::size_t N>
  void print_sequence(T (&arr)[N])
  {
    print_sequence(std::begin(arr), std::end(arr));
  }

  template<typename T>
  void print_sequence(const std::vector<T> &vec)
  {
    print_sequence(vec.cbegin(),vec.cend());
  }
}

#endif
