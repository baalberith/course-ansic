#include "bigints.h"
#include <iostream>
using namespace std;

int main() {
  Bigint b1, b2;
  char op;
  
  while (cin >> b1 >> op >> b2) {
    switch (op) {
      case '+' :
	cout << b1 + b2 << endl;
	break;
      case '-' :
	cout << b1 - b2 << endl;
	break;
      case '*' :
	cout << b1 * b2 << endl;
	break;
      case '/' :
	cout << b1 / b2 << endl;
	break;
      case '%' :
	cout << b1 % b2 << endl;
	break;
    }
  }
  
  return 0;
}
