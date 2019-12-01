#include "rsa.h"

#define LIMIT 10000

int powerLog(int n, int p, int mod)
{
	int result = 1;
	for (; p; p >>= 1)
	{
		if (p & 1)
			result = (1LL * result * n) % mod;
		n = (1LL * n * n) % mod;
	}
	return result;
}

int gcd(int a, int b)
{
	while (b)
	{
		int r = a % b;
		a = b;
		b = r;
	}
	return a;
}

int getCoprime(int n)
{
	int generated = rand() % LIMIT;
	while (gcd(n, generated) != 1)
		generated = rand() % LIMIT;
	return generated;
}

int isPrime(int num)
{
	if (num == 2) return true;
	if (num < 2 || num % 2 == 0) return false;

	for (int i = 3; i < (int)pow(num, 0.5) + 2; i += 2) {
		if (num % i == 0) return false;
	}

	return true;
}

pair<int, int> extendedEuclid(int a, int b) {
	if (!b) {
		return { 1, 0 };
	}

	pair<int, int> result = extendedEuclid(b, a % b);
	return { result.second, result.first - (a / b) * result.second };
}

int modularInverse(int n, int mod)
{
	int inverse = extendedEuclid(n, mod).first;
	while (inverse < 0)
		inverse += mod;
	return inverse;
}

pair<pair<int, int>, pair<int, int>> createKeys(int p, int q)
{
	if (!(isPrime(p) && isPrime(q))) {
		throw exception("Numbers must be prime!");
	}
	else if (p == q) {
		throw exception("Numbers must be different!");
	}

	pair<pair<int, int>, pair<int, int>> result;

	int n = p * q;
	int phi = (p - 1) * (q - 1);
	int e = getCoprime(phi);

	result.first = make_pair(n, e);

	int d = modularInverse(e, phi);

	result.second = make_pair(n, d);

	return result;
}

int encryptByte(pair<int, int> key, int value)
{
	return powerLog(value, key.second, key.first);
}

int decryptByte(pair<int, int> key, int value)
{
	return powerLog(value, key.second, key.first);
}

vector<int> encryptRsa(pair<int, int> key, string text) {
	vector<int> encBytes(text.length());
	for (int i = 0; i < text.length(); i++) {
		encBytes[i] = encryptByte(key, (int) text[i]);
	}

	return encBytes;
}

string decryptRsa(pair<int, int> key, vector<int> text) {
	vector<char> decBytes(text.size());
	for (int i = 0; i < text.size(); i++) {
		decBytes[i] = (char)decryptByte(key, text[i]);
	}

	return string(decBytes.begin(), decBytes.end());
}