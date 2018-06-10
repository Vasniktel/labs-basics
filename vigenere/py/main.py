#!/usr/bin/env python3

from string import ascii_lowercase as alphabet

def index(c):
  return ord(c) - ord('a')

def encode(msg, key, table):
  res = []
  for i in range(len(msg)):
    row = index(msg[i])
    col = index(key[i % len(key)])
    res += table[row][col]
  return ''.join(res)

def decode(msg, key, table):
  res = []
  for i in range(len(msg)):
    row = index(key[i % len(key)])
    res += alphabet[table[row].find(msg[i])]
  return ''.join(res)

if __name__ == '__main__':
  try:
    print('Enter message and key respectively:')
    msg, key = input('> ').split()[:2]
    table = [alphabet[i:] + alphabet[:i] for i in range(len(alphabet))]

    encoded = encode(msg, key, table)
    print('Encoded: ', encoded)

    decoded = decode(encoded, key, table)
    print('Decoded: ', decoded)

    print('Equal: ', decoded == msg)
  except:
    print('Invalid input')
