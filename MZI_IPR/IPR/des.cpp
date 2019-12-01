#include "des.h"

#define DEBUG

pair<string, string> desCipher(string text, string key, DesMode mode)
{
#ifdef DEBUG
	cout << "DES" << endl
		<< "----------------------------" << endl
		<< "Text: '" << text << "' "
		<< "Entered Key: '" << key << "'" << endl;

	if (mode == DesMode::Decrypt) {
		cout << "Decryption..." << endl;
	}
	else {
		cout << "Encryption..." << endl;
	}
#endif // DEBUG

	vector<string> blocks;
	string result = "";

	if (DesMode::Encrypt == mode) {
		prepareText(text);
		createBlocks(blocks, text);
		correctKey(key, text.length() / (2 * blocks.size()));

		cout << key << endl;
		key = toBinary(key);

		for (int i = 0; i < ROUND_NUM; i++) {
			for (int j = 0; j < blocks.size(); j++) {
				blocks[j] = encodeRound(blocks[j], key);
			}
			key = keyForNextRound(key);
		}

		key = keyForPrevRound(key);
		
		key = toString(key);
		for (auto it = blocks.begin(); it != blocks.end(); it++) {
			result += *it;
		}

		result = toString(result);
	}
	else {
		key = toBinary(key);
		text = toBinary(text);
		createBinaryBlocks(blocks, text);

		for (int i = 0; i < ROUND_NUM; i++) {
			for (int j = 0; j < blocks.size(); j++) {
				blocks[j] = decodeRound(blocks[j], key);
			}

			key = keyForPrevRound(key);
		}

		key = keyForNextRound(key);

		for (auto it = blocks.begin(); it != blocks.end(); it++) {
			result += *it;
		}

		result = toString(result, true);
	}

#ifdef DEBUG
	cout << "Result: " << result << endl;
	cout << "----------------------------" << endl;
#endif // DEBUG
	return pair<string, string>(result, key);
}

void createBlocks(vector<string> &blocks, const string& text)
{
	blocks = vector<string>((text.length() * CHAR_SIZE) / BLOCK_SIZE);
	int blockLenght = text.length() / blocks.size();

	for (int i = 0; i < blocks.size(); i++) {
		blocks[i] = text.substr(i * blockLenght, blockLenght);
		blocks[i] = toBinary(blocks[i]);
	}
}

void createBinaryBlocks(vector<string>& blocks, const string& text)
{
	blocks = vector<string>(text.length() / BLOCK_SIZE);
	int blockLenght = text.length() / blocks.size();

	for (int i = 0; i < blocks.size(); i++) {
		blocks[i] = text.substr(i * blockLenght, blockLenght);
	}
}

void prepareText(string& text)
{
	while (((text.length() * CHAR_SIZE) % BLOCK_SIZE) != 0) {
		text += SPEC_CHR;
	}
}

string toBinary(const string& text)
{
	string output = "";
	for (int i = 0; i < text.size(); i++) {
		string binaryChar = bitset<CHAR_SIZE>(text[i]).to_string();		
		output += binaryChar;
	}

	return output;
}

void correctKey(string& key, int keyLength)
{
	if (key.length() > keyLength) {
		key = key.substr(0, keyLength);
	}
	else {
		while (key.length() < keyLength) {
			key = "0" + key;
		}
	}
}

string encodeRound(const string& text, const string& key)
{
	string l = text.substr(0, text.length() / 2);
	string r = text.substr(text.length() / 2, text.length() / 2);

	return r + xOR(l, chiperFunction(r, key));
}

string decodeRound(const string& text, const string& key)
{
	string l = text.substr(0, text.length() / 2);
	string r = text.substr(text.length() / 2, text.length() / 2);

	return xOR(chiperFunction(l, key), r) + l;
}

string xOR(const string& str1, const string& str2)
{
	string result = "";
	for (int i = 0; i < str1.length(); i++) {
		bool a = (bool)(int)(str1[i] - '0');
		bool b = (bool)(int)(str2[i] - '0');

		if (a ^ b) {
			result += "1";
		}
		else {
			result += "0";
		}
	}

	return result;
}

string chiperFunction(const string& str1, const string& str2)
{
	return xOR(str1, str2);
}

string keyForNextRound(string key)
{
	for (int i = 0; i < KEY_SHIFT; i++) {
		key = key[key.length() - 1] + key;
		key = key.erase(key.length() - 1, 1);
	}

	return key;
}

string keyForPrevRound(string key)
{
	for (int i = 0; i < KEY_SHIFT; i++) {
		key = key + key[0];
		key = key.erase(0, 1);
	}

	return key;
}

string toString(string text, bool removeSpecChr)
{
	string output = "";
	while (text.length() > 0) {
		string binaryChar = text.substr(0, CHAR_SIZE);
		text = text.erase(0, CHAR_SIZE);
		int a = 0;
		int degree = binaryChar.length() - 1;

		for (auto it = binaryChar.begin(); it != binaryChar.end(); it++) {
			a += (int)(*it - '0') * (int)pow(2, degree--);
		}

		char chr = (char)a;
		if (SPEC_CHR == chr && removeSpecChr) {

		}
		else {
			output += chr;
		}

	}

	return output;
}

pair<string, string> desEncrypt(string text, string key) {
	return desCipher(text, key);
}

pair<string, string> desDecrypt(string text, string key) {
	return desCipher(text, key, DesMode::Decrypt);
}