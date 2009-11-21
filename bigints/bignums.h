#ifndef _bignums_h_
#define _bignums_h_

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define max(a,b) (((a) >= (b)) ? (a) : (b))
#define min(a,b) (((a) <= (b)) ? (a) : (b))

extern const int BASE;
extern const int DIGS;

typedef struct {
  int *num;
  int len;
} bignum;

bignum from_str(const char *str);
bignum from_int(long long n);
const char* to_str(bignum x);
long long to_int(bignum x);
bignum plus (bignum x, bignum y);
bignum minusb (bignum x, bignum y);
int lesser (bignum x, bignum y);
bignum mul_int (bignum x, int y);
int mod_int (bignum x, int y);
bignum div_int (bignum x, int y);
bignum multiply (bignum x, bignum y);
bignum divide (bignum x, bignum y);
bignum modulo (bignum x, bignum y);
bignum copy(bignum b);
void del(bignum *b);

#endif
