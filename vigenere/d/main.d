#!/usr/bin/env rdmd

import std.ascii: lowercase;

void main() {
  import std.stdio: writeln, readln;
  import std.array: split;

  scope (failure) writeln("Invalid input");
  writeln("Enter message and key respectively:");
  string[] input = readln().split;
  string msg = input[0];
  string key = input[1];

  string[] table;

  foreach (i, _; lowercase) {
    table ~= lowercase[i .. $] ~ lowercase[0 .. i];
  }

  string encoded = encode(msg, key, table);
  writeln("Encoded: ", encoded);

  string decoded = decode(encoded, key, table);
  writeln("Decoded: ", decoded);

  writeln("Equal: ", decoded == msg);
}

string encode(string msg, string key, string[] table) {
  string result;
  foreach (i, c; msg) {
    int row = c - 'a';
    int col = key[i % key.length] - 'a';
    result ~= table[row][col];
  }
  return result;
}

string decode(string msg, string key, string[] table) {
  import std.string: indexOf;
  string result;
  foreach (i, c; msg) {
    int row = key[i % key.length] - 'a';
    result ~= lowercase[table[row].indexOf(c)];
  }
  return result;
}
