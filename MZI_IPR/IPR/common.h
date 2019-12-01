#pragma once

#include <fstream>
#include <string>
#include <streambuf>
#include <vector>
#include <sstream>
#include <ostream>
#include <tuple>
#include <random>

using namespace std;

int s_int(string str);
vector<string> split(const string& s, char delimeter);
string readFile(const char* filename);