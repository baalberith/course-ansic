#ifndef _bigints_h_
#define _bigints_h_

extern "C" {
  #include "bignums.h"
}

#include <string>
#include <iostream>
using std::string;
using std::istream;
using std::ostream;

class Bigint {
  private:
    bignum bigint;
    int minus;
    Bigint(const bignum b, const int m);
  public:
    Bigint(const Bigint& bi);
    Bigint& operator= (const Bigint& bi);
    Bigint(const string s = "0");
    Bigint(const long long n);
    operator string() const;
    operator long long() const;
    bool operator< (const Bigint& bi) const;
    bool operator> (const Bigint& bi) const;
    bool operator== (const Bigint& bi) const;
    Bigint operator+ (const Bigint& bi) const;
    Bigint& operator+= (const Bigint& bi);
    Bigint operator- (const Bigint& bi) const;
    Bigint& operator-= (const Bigint& bi);
    Bigint operator* (const Bigint& bi) const;
    Bigint& operator*= (const Bigint& bi);
    Bigint operator/ (const Bigint& bi) const;
    Bigint& operator/= (const Bigint& bi);
    Bigint operator% (const Bigint& bi) const;
    Bigint& operator%= (const Bigint& bi);
    ~Bigint();
    friend ostream& operator<<(ostream &s, const Bigint& bi);
    friend istream& operator>>(istream &s, Bigint& bi);
};

#endif
