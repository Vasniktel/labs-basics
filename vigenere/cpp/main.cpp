#include <iostream>
#include <string>

#define LEN ('z' - 'a' + 1)

using namespace std;

string encode(const string&, const string&, string[]);
string decode(const string&, const string&, string[]);

int main() {
  try {
    string msg, key;

    cout << "Enter message and key respectively:" << endl;
    cin >> msg >> key;

    string table[LEN];

    for (int i = 0; i < LEN; i++) {
      for (int j = 0; j < LEN; j++) {
        table[i] += 'a' + (j + i) % LEN;
      }
    }

    string encoded = encode(msg, key, table);
    cout << "Encoded: " << encoded << endl;

    string decoded = decode(encoded, key, table);
    cout << "Decoded: " << decoded << endl;

    cout << "Equal: " << boolalpha << (decoded == msg) << endl;
  } catch (...) {
    cout << "Invalid input" << endl;
  }

  return 0;
}

string encode(const string& msg, const string& key, string table[]) {
  string result;
  for (unsigned i = 0; i < msg.size(); i++) {
    result += table[msg[i] - 'a'][key[i % key.size()] - 'a'];
  }
  return result;
}

string decode(const string& msg, const string& key, string table[]) {
  string result;
  for (unsigned i = 0; i < msg.size(); i++) {
    result += 'a' + table[key[i % key.size()] - 'a'].find(msg[i]);
  }
  return result;
}
