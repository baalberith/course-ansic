#include "bignums.h"

int main() {
  printf("%lld\n", to_int(from_int(1234567891011121314LL)));
  
  return 0;
}