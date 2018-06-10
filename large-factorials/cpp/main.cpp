#include <iostream>
#include <string>

#include "Big.cpp"

using namespace std;
using namespace big;

string factorial(int arg) {
  string result;
  try {
    result = big_num(arg).fact().to_string();
  } catch (...) {
    result = "";
  }
  return result;
}

int main() {
  unsigned num;
  cin >> num;
  cout << factorial(num) << endl;
}
