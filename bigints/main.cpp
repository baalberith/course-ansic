#include "bigints.h"
#include <iostream>
using namespace std;

int main() {
  Bigint bi("1234567891011121314999999999999999999999999999999999999999999999888888888888888888888888");
  cout << (string)(bi) << endl;
  
  return 0;
}