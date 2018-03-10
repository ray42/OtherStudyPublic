#ifndef RAYRAY_HEADER
#define RAYRAY_HEADER

#include<iostream>
#include<string>

namespace rr
{
  extern bool debug;

  std::ostream& greet(const std::string &, 
                      const void * pt=nullptr,
                      std::ostream& os=std::cout);
}

#endif



