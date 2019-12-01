#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

int powerLog(int n, int p, int mod);
int gcd(int a, int b);
int getCoprime(int n);
int isPrime(int num);
pair<int, int> extendedEuclid(int a, int b);
int modularInverse(int n, int mod);
pair<pair<int, int>, pair<int, int>> createKeys(int, int);
int encryptByte(pair<int, int> key, int value);
int decryptByte(pair<int, int> key, int value);

vector<int> encryptRsa(pair<int, int> key, string text);
string decryptRsa(pair<int, int> key, vector<int> text);