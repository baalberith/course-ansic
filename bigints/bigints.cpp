#include "bigints.h"

Bigint::Bigint(long long n) {
  minus = n < 0 ? -1 : 1;
  bigint = from_int(minus * n);
}

Bigint::Bigint(string s) {
  minus = s[0] == '-' ? -1 : 1;
  if (minus == -1)
    s = s.substr(1);
  bigint = from_str(s.c_str());
}

Bigint::operator long long() {
  return minus * to_int(bigint);
}

Bigint::operator string() {
  string s = minus == -1 ? "-" : "";
  s = s + string(to_str(bigint));
  return s;
}