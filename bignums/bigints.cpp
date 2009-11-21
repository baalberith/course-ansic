#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int BASE = 1000000000;
const int DIGS = 9;
const int LEN = 1000;

struct liczba {
  int t[LEN];
  int l;
};

void czytaj(liczba &x) {
  char s[LEN * DIGS + 1];
  scanf("%s", s);
  int j = strlen(s);
  if (j % DIGS == 0)
    x.l = j / DIGS;
  else
    x.l = j / DIGS + 1;
  j--;
  for (int i = 0; i < x.l; i++) {
    x.t[i] = 0;
    for (int k = max(0, j - DIGS + 1); k <= j; k++)
      x.t[i] = 10 * x.t[i] + (s[k] - '0');
    j -= DIGS;
  }
}

void wypisz(liczba x) {
  printf("%d", x.t[x.l - 1]);
  for (int i = x.l - 2; i >= 0; i--)
    printf("%0*d", DIGS, x.t[i]);
  printf("\n");
}

liczba operator+ (liczba x, liczba y) {
  liczba z;
  z.l = min(x.l, y.l);
  int c = 0;
    for (int i = 0; i < z.l; i++) {
      z.t[i] = (x.t[i] + y.t[i] + c) % BASE;
      c = (x.t[i] + y.t[i] + c) / BASE;
    }
  while (z.l < x.l) {
    z.t[z.l] = (x.t[z.l] + c) % BASE;
    c = (x.t[z.l] + c) / BASE;
    z.l++;
  }
  while (z.l < y.l) {
    z.t[z.l] = (y.t[z.l] + c) % BASE;
    c = (y.t[z.l] + c) / BASE;
    z.l++;
  }
  if (c > 0) {
    z.t[z.l] = c;
    z.l++;
  }
  return z;
}

liczba operator- (liczba x, liczba y) {
  liczba z;
  z.l = x.l;
  int c = 0;
  for (int i = 0; i < x.l; i++) {
    if (i < y.l)
      z.t[i] = x.t[i] - y.t[i] + c;
    else
      z.t[i] = x.t[i] + c;
    if (z.t[i] < 0) {
      z.t[i] += BASE;
      c = -1;
    } else c = 0;
  }
  while (z.l > 1 && z.t[z.l - 1] == 0)
    z.l--;
  return z;
}

bool operator< (liczba x, liczba y) {
  if (x.l < y.l)
    return true;
  if (x.l > y.l)
    return false;
  int i = x.l - 1;
  while (i >= 0 && x.t[i] == y.t[i])
    i--;
  if (i < 0)
    return false;
  if (x.t[i] < y.t[i])
    return true;
  return false;
}

bool operator> (liczba x, liczba y) {
  return y < x;
}

bool operator== (liczba x, liczba y) {
  return !(x < y) && !(y < x);
}

liczba operator* (liczba x, int y) {
  liczba z;
  z.l = x.l;
  int c = 0;
  for (int i = 0; i < x.l; i++)
  {
    z.t[i] = int(((long long)(x.t[i]) * y + c) % BASE);
    c = int(((long long)(x.t[i]) * y + c) / BASE);
  }
  while (c > 0)
  {
    z.t[z.l] = c % BASE;
    c /= BASE;
    z.l++;
  }
  return z;
}

//liczba operator* (liczba x, liczba y) {
//  liczba z;
//  z.l = 1;
//  z.t[0] = 0;
//  for (int i = 0; i < y.l; i++) {
//    liczba pom(x * y.t[i]);
//    for (int j = pom.l - 1; j >= 0; j--)
//      pom.t[j + i] = pom.t[j];
//    for (int j = 0; j < i; j++)
//      pom.t[j] = 0;
//    pom.l = pom.l + i;
//    z = z + pom;
//  }
//  return z;
//}

int operator% (liczba x, int y) {
  long long z = (long long)(x.t[x.l - 1]) % y;
  for (int i = x.l - 2; i >= 0; i--)
    z = (z * BASE + x.t[i]) % y;
  return int(z);
}

liczba operator/ (liczba x, int y) {
  liczba z;
  int c = 0;
  z.l = x.l;
  for (int i = x.l - 1; i >= 0; i--) {
    z.t[i] = int(((long long)(c) * BASE + (long long)x.t[i]) / y);
    c = int(((long long)(c) * BASE + (long long)x.t[i]) % y);
  }
  while (z.l > 1 && z.t[z.l - 1] == 0)
    z.l--;
  return z;
}

liczba operator* (liczba x, liczba y) {
  liczba z, zero;
  zero.l = 1;
  zero.t[0] = 0;
  z = zero;
  while (y > zero) {
    if (y % 2 == 1)
      z = z + x;
    x = x * 2;
    y = y / 2;
  }
  return z;
}

liczba operator/ (liczba x, liczba y) {
  liczba pom, z;
  z.l = x.l - y.l + 1;
  if (z.l <= 0) {
    z.l = 1;
    z.t[0] = 0;
    return z;
  }
  pom.l = y.l;
  for (int i = 0; i < pom.l; i++)
    pom.t[pom.l - i - 1] = x.t[x.l - 1 - i];
  int i = x.l - y.l;
  while (i >= 0) {
    int a = 0, b = BASE - 1;
    while (a < b) {
      int c = (a + b) / 2 + 1;
      if (y * c > pom)
        b = c - 1;
      else
        a = c;
    }
    z.t[i] = a;
    pom = pom - (y * a);
    for (int j = pom.l - 2; j >= 0; j--)
      pom.t[j + 1] = pom.t[j];
    i--;
    if (i >= 0)
      pom.t[0] = x.t[i];
  }
  while (z.l > 0 && z.t[z.l - 1] == 0)
    z.l--;
  return z;
}

liczba operator% (liczba x, liczba y) {
  return x - (y * (x / y));
}

int main() {
  liczba l1, l2;
  czytaj(l1);
  czytaj(l2);
  wypisz(l1 + l2);
  wypisz(l1 - l2);
  printf("%d\n", l1 < l2);
  wypisz(l1 * 10);
  wypisz(l1 * l2);
  wypisz(l1 / 10);
  wypisz(l1 / l2);
  printf("%d\n", l1 % 10);
  wypisz(l1 % l2);

  return 0;
}
