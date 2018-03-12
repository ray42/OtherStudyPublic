#include "rr.h"

#include<iostream>
using std::cout; using std::endl;
using std::ostream;
#include<string>
using std::string;


// initialize the debug flag to false. Must set it to true.
bool rr::debug = false;

std::ostream& rr::greet(const string &fun, const void * pt, ostream& os)
{
  if(rr::debug)
  {
    if(pt)
    {
      os << "Hi: " << pt << ", " << fun << endl;
    }
    else
    {
      os << "Hi: " << fun << endl;
    }
  }
  return os;
}
