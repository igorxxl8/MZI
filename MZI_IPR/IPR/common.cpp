#include "common.h"


int s_int(string str)
{
	stringstream stream(str);
	int res = 0;
	stream >> res;
	return res;
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