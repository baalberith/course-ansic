#include "bigints.h"
#include <iostream>
using namespace std;

int main() {
  Bigint b1("999999999999999999999"), b2(-1);
  cout << (string)(b1 - b2) << endl;
  
  return 0;
}