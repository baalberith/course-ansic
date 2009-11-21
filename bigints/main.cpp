#include "bigints.h"
#include <iostream>
using namespace std;

int main() {
  Bigint b1("999999999999999999999"), b2(-1),
    b3 = b1 - b2, b4(b3),
    b5(-5), b6(3);
  cout << (string)(b5 / b6) << endl;
  cout << (string)(b5 % b6) << endl;
  
  return 0;
}