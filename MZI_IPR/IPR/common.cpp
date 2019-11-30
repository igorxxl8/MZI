#include "common.h"

int greaterCommonDivisor(int a, int b) {
	while (b != 0) {
		a = b;
		b = a % b;
	}

	return a;
}


vector<string> split(const string& s, char delimeter)
{
	vector<string> substrings;
	string token;
	istringstream stream(s);
	while (getline(stream, token, delimeter)) {
		substrings.push_back(token);
	}

	return substrings;
}

string readFile(const char* filename) {
	ifstream in(filename);
	string text((std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());

	return text;
}