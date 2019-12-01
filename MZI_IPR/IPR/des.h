#pragma once

#include <string>
#include <iostream>
#include <bitset>
#include "common.h"

using namespace std;

constexpr int BLOCK_SIZE = 64;
constexpr int CHAR_SIZE = 8;
constexpr int KEY_SHIFT = 2;
constexpr int ROUND_NUM = 16;
constexpr int SPEC_CHR = '~';

enum class DesMode {
	Encrypt,
	Decrypt
};

void createBlocks(vector<string>& blocks, const string& text);
void createBinaryBlocks(vector<string>& blocks, const string& text);
void prepareText(string& text);
void correctKey(string& text, int keyLength);
string toBinary(const string& text);
string encodeRound(const string& text, const string& key);
string decodeRound(const string& text, const string& key);
string xOR(const string& str1, const string& str2);
string chiperFunction(const string& str1, const string& str2);
string keyForNextRound(string key);
string keyForPrevRound(string key);
string toString(string input, bool removeSpecChr = false);


pair<string, string> desCipher(string text, string key, DesMode mode = DesMode::Encrypt);
pair<string, string> desEncrypt(string text, string key);
pair<string, string> desDecrypt(string text, string key);