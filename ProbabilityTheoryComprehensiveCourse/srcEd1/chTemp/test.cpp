#include<iostream>
using std::cout; using std::clog; using std::cerr; using std::endl; 
using std::cin;
#include<vector>
using std::vector;
#include<string>
using std::string;


//#include <iostream>
#include <algorithm>
#include <functional>

#include "./../common/rr.h"


using namespace std;
struct Z { 
    function<void()> g;
    Z(function<void()> f) : g(f) { } 
};
struct B;

struct A { 
    vector<B*> v; 
    const vector<B*> &getV() { return v; } 
};

struct B {
    A *a;
    B *b;
    Z *z;
    string s;
    B(A *i, string j) : a(i), s(j) { 
        a->v.push_back(this); 
    }
    
    void setB(B *c) { b = c; }

    void f() { 
        vector<B*> v = a->getV(); 
        for (auto &i : v) cout << i->s;
        v.erase(remove_if(v.begin(), v.end(), [this](const B* b){ return b->a != a or b == this; }), v.end());
        vector<string> w;
        cout << "Hi from f()1" << endl;
        transform(v.begin(), v.end(), back_inserter(w), [](const B *b){ return b->s + 'z'; });
        cout << "Hi from f()2" << endl;
        for(auto i:v)
        {
          cout << "vvvv: " << i<< endl;
        }
        cout << "Done the output" << endl;
        z = new Z([this, v]{ 
            cout << "From lambda, gonna call setB" << endl;
            setB(v.front()); 
//            cout << "v.front().s"<<endl;
//            cout << v.front().s << endl;
            cout << "Calling delete z" << endl;
            delete z; 
            cout << "gonna output b->s" << endl;
            cout << b->s; 
            
            cout << "outputted b->s" << endl;
            });
        cout << "done new Z" << endl;
    }
};

int main() { 
  rr::debug = true;
  cout << rr::debug << endl;
  rr::debug = false;
  cout << rr::debug << endl;
//   A a;
//   B b(&a, "b");
//   B c(&a, "c");
//   c.f();
//   cout << "Gonna call c.z->g" << endl;
//   c.z->g();
//   cout << "cout c.b-s" << endl;
//   cout << c.b->s;
}

//struct B
//{
//  int i;
//};
//
////struct D:public B
////{
////  int j = 2;
////};
//
//int main(int, char*[])
//{
//unsigned x; 
//cout << x << "\n";
//  
//
//
//  return 0;
//}
