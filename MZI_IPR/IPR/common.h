#pragma once

#include <fstream>
#include <string>
#include <streambuf>
#include <vector>
#include <sstream>
#include <ostream>

using namespace std;

int greaterCommonDivisor(int a, int b);
vector<string> split(const string& s, char delimeter);
string readFile(const char* filename);