#ifndef _bigints_h_
#define _bigints_h_

extern "C" {
  #include "bignums.h"
}

#include <string>
using namespace std;

class Bigint {
  private:
    bignum bigint;
    int minus;
  public:
    Bigint(long long n);
    Bigint(string s);
    operator long long();
    operator string();
};

#endif