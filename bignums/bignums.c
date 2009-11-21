#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define max(a,b) (((a) >= (b)) ? (a) : (b))
#define min(a,b) (((a) <= (b)) ? (a) : (b))

const int BASE = 1000000000; // podstawa 10 ^ 9
const int DIGS = 9; // ilosc cyfr podstawy

typedef struct {
  int *num;
  int len;
} bignum;

bignum from_str(char *str) {
  bignum x;
  int len;

  len = strlen(str);

  if (len % DIGS == 0)
    x.len = len / DIGS;
  else
    x.len = len / DIGS + 1;

  x.num = calloc(x.len, sizeof(int));

  len--;

  int i;
  for (i = 0; i < x.len; i++) {
    x.num[i] = 0;

    int k;
    for (k = max(0, len - DIGS + 1); k <= len; k++)
      x.num[i] = 10 * x.num[i] + (str[k] - '0');

    len -= DIGS;
  }

  return x;
}

bignum from_int(int n) {
  bignum x;
  x.len = 1;
  x.num = calloc(x.len, sizeof(int));
  x.num[0] = n;

  return x;
}

char* to_str(bignum x) {
  char* str, *tmp;

  str = calloc(x.len * DIGS + 1, sizeof(char));
  tmp = calloc(DIGS + 1, sizeof(char));

  sprintf(tmp, "%d", x.num[x.len - 1]);
  strcat(str, tmp);

  int i;
  for (i = x.len - 2; i >= 0; i--) {
    sprintf(tmp, "%0*d", DIGS, x.num[i]);
    strcat(str, tmp);
  }

  free(tmp);

  return str;
}

int to_int(bignum x) {
  return x.num[0];
}

bignum plus (bignum x, bignum y) {
  bignum z;
  z.len = min(x.len, y.len);
  int len = max(x.len, y.len) + 1;
  z.num = calloc(len, sizeof(int));

  int r = 0;

  int i;
  for (i = 0; i < z.len; i++) {
    z.num[i] = (x.num[i] + y.num[i] + r) % BASE;
    r = (x.num[i] + y.num[i] + r) / BASE;
  }

  while (z.len < x.len) {
    z.num[z.len] = (x.num[z.len] + r) % BASE;
    r = (x.num[z.len] + r) / BASE;
    z.len++;
  }

  while (z.len < y.len) {
    z.num[z.len] = (y.num[z.len] + r) % BASE;
    r = (y.num[z.len] + r) / BASE;
    z.len++;
  }

  if (r > 0) {
    z.num[z.len] = r;
    z.len++;
  }

  if (z.len == len)
    return z;
  else {
    bignum s;
    s.len = z.len;
    s.num = calloc(s.len, sizeof(int));

    memcpy(s.num, z.num, s.len * sizeof(int));
    free(z.num);

    return s;
  }
}

bignum minus (bignum x, bignum y) {
  bignum z;
  z.len = x.len;
  int len = x.len;
  z.num = calloc(len, sizeof(int));

  int r = 0;

  int i;
  for (i = 0; i < x.len; i++) {
    if (i < y.len)
      z.num[i] = x.num[i] - y.num[i] + r;
    else
      z.num[i] = x.num[i] + r;

    if (z.num[i] < 0) {
      z.num[i] += BASE;
      r = -1;
    } else r = 0;
  }

  while (z.len > 1 && z.num[z.len - 1] == 0)
    z.len--;

  if (z.len == len)
    return z;
  else {
    bignum d;
    d.len = z.len;
    d.num = calloc(d.len, sizeof(int));

    memcpy(d.num, z.num, d.len * sizeof(int));
    free(z.num);

    return d;
  }
}

int lesser (bignum x, bignum y) {
  if (x.len < y.len)
    return 1;
  if (x.len > y.len)
    return 0;

  int i = x.len - 1;
  while (i >= 0 && x.num[i] == y.num[i])
    i--;

  if (i < 0)
    return 0;

  if (x.num[i] < y.num[i])
    return 1;
  else
    return 0;
}

int greater (bignum x, bignum y) {
  return lesser(y, x);
}

int equal(bignum x, bignum y) {
  return !lesser(x, y) && !lesser(y, x);
}

bignum mul_int (bignum x, int y) {
  bignum z;
  z.len = x.len;
  int len = x.len + DIGS;
  z.num = calloc(len, sizeof(int));

  int r = 0;

  int i;
  for (i = 0; i < x.len; i++) {
    z.num[i] = ((long long)(x.num[i]) * y + r) % BASE;
    r = ((long long)(x.num[i]) * y + r) / BASE;
  }

  while (r > 0) {
    z.num[z.len] = r % BASE;
    r = r / BASE;
    z.len++;
  }

  if (z.len == len)
    return z;
  else {
    bignum m;
    m.len = z.len;
    m.num = calloc(m.len, sizeof(int));

    memcpy(m.num, z.num, m.len * sizeof(int));
    free(z.num);

    return m;
  }
}

int mod_int (bignum x, int y) {
  long long z = (long long)(x.num[x.len - 1]) % y;

  int i;
  for (i = x.len - 2; i >= 0; i--)
    z = (z * BASE + x.num[i]) % y;

  return z;
}

bignum div_int (bignum x, int y) {
  bignum z;
  z.len = x.len;
  int len = x.len;
  z.num = calloc(len, sizeof(int));

  int r = 0;

  int i;
  for (i = x.len - 1; i >= 0; i--) {
    z.num[i] = ((long long)(r) * BASE + (long long)(x.num[i])) / y;
    r = ((long long)(r) * BASE + (long long)(x.num[i])) % y;
  }

  while (z.len > 1 && z.num[z.len - 1] == 0)
    z.len--;

  if (z.len == len)
    return z;
  else {
    bignum d;
    d.len = z.len;
    d.num = calloc(d.len, sizeof(int));

    memcpy(d.num, z.num, d.len * sizeof(int));
    free(z.num);

    return d;
  }
}

bignum multiply (bignum x, bignum y) {
  bignum z, zero;
  int len = x.len + y.len;

  zero.len = 1;
  zero.num = calloc(len, sizeof(int));
  zero.num[0] = 0;

  z.len = 1;
  z.num = calloc(len, sizeof(int));
  z.num[0] = 0;

  while (greater(y, zero)) {
    if (mod_int(y, 2) == 1)
      z = plus(z, x);

    x = mul_int(x, 2);
    y = div_int(y, 2);
  }

  free(zero.num);

  if (z.len == len)
    return z;
  else {
    bignum m;
    m.len = z.len;
    m.num = calloc(m.len, sizeof(int));

    memcpy(m.num, z.num, m.len * sizeof(int));
    free(z.num);

    return m;
  }
}

bignum divide (bignum x, bignum y) {
  bignum z;
  z.len = x.len - y.len + 1;
  int len = x.len;
  z.num = calloc(len, sizeof(int));

  if (z.len <= 0) {
    z.len = 1;
    z.num[0] = 0;

    return z;
  }

  bignum tmp;
  tmp.len = y.len;
  tmp.num = calloc(tmp.len, sizeof(int));

  int i;
  for (i = 0; i < tmp.len; i++)
    tmp.num[tmp.len - i - 1] = x.num[x.len - i - 1];

  i = x.len - y.len;
  while (i >= 0) {
    int a = 0;
    int b = BASE - 1;

    while (a < b) {
      int c = (a + b) / 2 + 1;

      if (greater(mul_int(y, c), tmp))
        b = c - 1;
      else
        a = c;
    }

    z.num[i] = a;

    tmp = minus(tmp, mul_int(y, a));

    int j;
    for (j = tmp.len - 2; j >= 0; j--)
      tmp.num[j + 1] = tmp.num[j];

    i--;
    if (i >= 0)
      tmp.num[0] = x.num[i];
  }

  free(tmp.num);

  while (z.len > 0 && z.num[z.len - 1] == 0)
    z.len--;

  if (z.len == len)
    return z;
  else {
    bignum d;
    d.len = z.len;
    d.num = calloc(d.len, sizeof(int));

    memcpy(d.num, z.num, d.len * sizeof(int));
    free(z.num);

    return d;
  }
}

bignum modulo (bignum x, bignum y) {
  return minus(x, multiply(y, divide(x, y)));
}

int main() {
  bignum l1 = from_str("99"),
    l2 = from_str("10");

  printf("%s\n", to_str(from_int(666)));
  printf("%d\n", to_int(from_str("666")));

  bignum sum = plus(l1, l2);
  printf("%s\n", to_str(sum));

  bignum diff = minus(l1, l2);
  printf("%s\n", to_str(diff));

  printf("%i\n", lesser(l1, l2));

  bignum muli = mul_int(l1, 2);
  printf("%s\n", to_str(muli));

  printf("%d\n", mod_int(l1, 10));

  bignum divi = div_int(l1, 10);
  printf("%s\n", to_str(divi));

  bignum mul = multiply(l1, l2);
  printf("%s\n", to_str(mul));

  bignum div = divide(l1, l2);
  printf("%s\n", to_str(div));

  bignum mod = modulo(l1, l2);
  printf("%s\n", to_str(mod));

  return 0;
}
