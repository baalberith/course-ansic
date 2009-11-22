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
bignum from_int(const long long n);
const char* to_str(const bignum x);
long long to_int(const bignum x);
bignum copy(const bignum b);
void xcopy(bignum *b, const bignum b2);
bignum plus (const bignum x, const bignum y);
bignum minusb (const bignum x, const bignum y);
int lesser (const bignum x, const bignum y);
int greater (const bignum x, const bignum y);
int equal (const bignum x, const bignum y);
bignum mul_int (const bignum x, const int y);
int mod_int (const bignum x, const int y);
bignum div_int (const bignum x, const int y);
bignum multiply (const bignum x, const bignum y);
bignum divide (const bignum x, const bignum y);
bignum modulo (const bignum x, const bignum y);
void del(bignum *b);

#endif
