#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

string gostEncrypt(string text, string key);
vector<uint8_t> gost(vector<uint8_t> data, vector<uint8_t> key);
void gost(uint8_t* data, uint8_t* key, uint8_t* res);
vector<uint8_t> mutateBlock(vector<uint8_t> block, vector<uint32_t> keys);

string gostDecrypt(string data, string key);
void prepareText(string& text);
void removeSpecChr(string& text);

uint32_t subs(uint8_t value);
vector<uint32_t> generateKeys(vector<uint8_t> key);
uint32_t byteToUintCast(uint8_t* pKey, int pos);
uint8_t* uintToByteCast(uint32_t uint);