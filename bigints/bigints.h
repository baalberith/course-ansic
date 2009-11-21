#ifndef _bigints_h_
#define _bigints_h_

extern "C" {
  #include "bignums.h"
}

#include <string>
using std::string;

class Bigint {
  private:
    bignum bigint;
    int minus;
    Bigint(bignum b, int m);
  public:
    Bigint(const Bigint& bi);
    Bigint& operator= (const Bigint& bi);
    Bigint(string s = "0");
    Bigint(long long n);
    operator string();
    operator long long();
    Bigint operator+ (Bigint b);
    Bigint operator- (Bigint b);
    ~Bigint();
};

#endif