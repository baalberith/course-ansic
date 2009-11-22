#include "bigints.h"

Bigint::Bigint(const bignum b, const int m) {
  bigint = copy(b);
  minus = m;
}

Bigint::Bigint(const Bigint& bi) {
  bigint = copy(bi.bigint);
  minus = bi.minus;
}

Bigint& Bigint::operator= (const Bigint& bi) {
  if (this != &bi) {
    xcopy(&bigint, bi.bigint);
    minus = bi.minus;
  }
  return *this;
}

Bigint::Bigint(const string s) {
  minus = s[0] == '-' ? -1 : 1;
  string ss;
  if (minus == -1)
    ss = s.substr(1);
  else ss = s;
  bigint = from_str(ss.c_str());
}

Bigint::Bigint(const long long n) {
  minus = n < 0 ? -1 : 1;
  bigint = from_int(minus * n);
}

Bigint::operator string() const {
  string s1 = minus == -1 ? "-" : "";
  string s2(to_str(bigint));
  return s1 + s2;
}

Bigint::operator long long() const {
  return minus * to_int(bigint);
}

bool Bigint::operator< (const Bigint& bi) const {
  bignum b1 = bigint;
  int m1 = minus;
  bignum b2 = bi.bigint;
  int m2 = bi.minus;
  
  bool comp;
  if (m1 == 1 && m2 == 1)
    comp = lesser(b1, b2);
  else if (m1 == -1 && m2 == -1) {
    comp = lesser(b1, b2);
    if (!equal(b1, b2))
      comp = !comp;
  }
  else if (m1 == 1 && m2 == -1)
    comp = false;
  else if (m1 = -1 && m2 == 1)
    comp = true;
  return comp;
}

bool Bigint::operator> (const Bigint& bi) const {
  return bi < *this;
}

bool Bigint::operator== (const Bigint& bi) const {
  return !(*this < bi) && !(*this > bi);
}

Bigint Bigint::operator+ (const Bigint& bi) const {
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

Bigint& Bigint::operator+= (const Bigint& bi) {
  *this = *this + bi;
  return *this;
}
    
Bigint Bigint::operator- (const Bigint& bi) const {
  Bigint bi2(bi.bigint, (-1) * bi.minus);
  return Bigint((*this) + bi2);
}

Bigint& Bigint::operator-= (const Bigint& bi) {
  *this = *this - bi;
  return *this;
}

Bigint Bigint::operator* (const Bigint& bi) const {
  bignum b1 = bigint;
  int m1 = minus;
  bignum b2 = bi.bigint;
  int m2 = bi.minus;
  
  bignum b = multiply(b1, b2);
  int m = m1 * m2;
  return Bigint(b, m);
}

Bigint& Bigint::operator*= (const Bigint& bi) {
  *this = *this * bi;
  return *this;
}

Bigint Bigint::operator/ (const Bigint& bi) const {
  bignum b1 = bigint;
  int m1 = minus;
  bignum b2 = bi.bigint;
  int m2 = bi.minus;
  
  bignum b = divide(b1, b2);
  int m = m1 * m2;
  return Bigint(b, m);
} 

Bigint& Bigint::operator/= (const Bigint& bi) {
  *this = *this / bi;
  return *this;
}

Bigint Bigint::operator% (const Bigint& bi) const {
  bignum b1 = bigint;
  int m1 = minus;
  bignum b2 = bi.bigint;
  int m2 = bi.minus;
  
  bignum b = modulo(b1, b2);
  int m;
  if (m1 == 1 && m2 == 1) 
    m = 1;
  else if (m1 == -1 && m2 == -1)
    m = -1;
  else if (m1 == 1 && m2 == -1) 
    m = 1;
  else
    m = -1;
  return Bigint(b, m);
} 

Bigint& Bigint::operator%= (const Bigint& bi) {
  *this = *this % bi;
  return *this;
}

Bigint::~Bigint() {
  del(&bigint);
}

ostream & operator<<(ostream &s, const Bigint& bi) {
  s << (string) bi;
  return s;
}

istream & operator>>(istream &s, Bigint& bi) {
  string str;
  s >> str;
  bi = Bigint(str);
  return s;
}
