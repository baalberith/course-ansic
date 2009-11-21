#include "bigints.h"

Bigint::Bigint(bignum b, int m) {
  bigint = copy(b);
  minus = m;
}

Bigint::Bigint(const Bigint &bi) {
  bigint = copy(bi.bigint);
  minus = bi.minus;
}

Bigint& Bigint::operator= (const Bigint& bi) {
  bigint = copy(bi.bigint);
  minus = bi.minus;
  return *this;
}

Bigint::Bigint(string s) {
  minus = s[0] == '-' ? -1 : 1;
  if (minus == -1)
    s = s.substr(1);
  bigint = from_str(s.c_str());
}

Bigint::Bigint(long long n) {
  minus = n < 0 ? -1 : 1;
  bigint = from_int(minus * n);
}

Bigint::operator string() {
  string s1 = minus == -1 ? "-" : "";
  string s2(to_str(bigint));
  return s1 + s2;
}

Bigint::operator long long() {
  return minus * to_int(bigint);
}

Bigint Bigint::operator+ (Bigint bi) {
  bignum b1 = bigint;
  int m1 = minus;
  bignum b2 = bi.bigint;
  int m2 = bi.minus;
  
  bignum b;
  int m;
  if (m1 == 1 && m2 == 1) {
    b = plus(b1, b2);
    m = 1;
  }
  else if (m1 == -1 && m2 == -1) {
    b = plus(b1, b2);
    m = -1;
  }
  else if (m1 == 1 && m2 == -1) {
    if (lesser(b1, b2)) {
      b = minusb(b2, b1);
      m = -1;
    }
    else {
      b = minusb(b1, b2);
      m = 1;
    }
  }
  else {
    if (lesser(b2, b1)) {
      b = minusb(b1, b2);
      m = -1;
    }
    else {
      b = minusb(b2, b1);
      m = 1;
    }
  }
  return Bigint(b, m);
}
    
Bigint Bigint::operator- (Bigint bi) {
  Bigint bi2(bi.bigint, (-1) * bi.minus);
  return Bigint((*this) + bi2);
}

Bigint::~Bigint() {
  del(&bigint);
}
