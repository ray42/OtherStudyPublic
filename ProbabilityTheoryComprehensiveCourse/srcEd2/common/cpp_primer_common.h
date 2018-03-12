/*
 * =========================================================================
 *         Author:  Raymon White (ray), rayaxiom42 AT gmail DOT com
 * ========================================================================
 */
#ifndef CPP_PRIMER_COMMON_H
#define CPP_PRIMER_COMMON_H
#include<iostream>

namespace cppp{

// this is taken from exer6-44.cpp
// return the plural version of word if ctr is greater then 1.
inline std::string make_plural(size_t ctr, const std::string& word,
                               const std::string& ending="s")
{
  return (ctr>1)?word+ending:word;
}

template<typename T>
inline void print_seq(const T& c, std::ostream&os=std::cout)
{
  for(const auto& i:c)
  {
    os << i << " ";
  }
}


}
#endif
