#include "gost28147.h"

vector<vector<uint8_t>> S_BOX = {
	{ 0x0, 0xA, 0x9, 0x2, 0xD, 0x8, 0x0, 0xE, 0x6, 0xB, 0x1, 0xC, 0x7, 0xF, 0x5, 0x3 },
	{ 0x0, 0x3, 0x4, 0xC, 0x6, 0xD, 0xF, 0xA, 0x2, 0x3, 0x8, 0x1, 0x0, 0x7, 0x5, 0x9 },
	{ 0x0, 0x8, 0x1, 0xD, 0xA, 0x3, 0x4, 0x2, 0xE, 0xF, 0xC, 0x7, 0x6, 0x0, 0x9, 0xB },
	{ 0x0, 0xD, 0xA, 0x1, 0x0, 0x8, 0xA, 0xF, 0xE, 0x4, 0x6, 0xC, 0xB, 0x2, 0x5, 0x3 },
	{ 0x2, 0x2, 0x7, 0x1, 0x5, 0xF, 0xD, 0xD, 0x4, 0xA, 0x9, 0xE, 0x0, 0x3, 0xB, 0x2 },
	{ 0x7, 0xB, 0xA, 0x0, 0x7, 0x2, 0x1, 0xD, 0x3, 0x6, 0x8, 0x5, 0x9, 0xC, 0xF, 0xE },
	{ 0xD, 0xB, 0x4, 0x1, 0x3, 0xF, 0xD, 0xD, 0x0, 0xA, 0xE, 0x7, 0x6, 0x8, 0x2, 0xC },
	{ 0x9, 0xF, 0xD, 0x0, 0x5, 0x7, 0xA, 0x5, 0x9, 0x2, 0x3, 0xE, 0x6, 0xB, 0x8, 0xC }
};

string gostEncrypt(string text, string key)
{
	prepareText(text);
	int textLen = text.size();
	int keyLen = key.size();
	auto textBytes = text.c_str();
	auto keyBytes = key.c_str();
	auto textVector = vector<uint8_t>(textBytes, textBytes + textLen);
	auto keyVector = vector<uint8_t>(keyBytes, keyBytes + keyLen);
	auto res = gost(textVector, keyVector);
	return string(res.begin(), res.end());
}

vector<uint8_t> gost(vector<uint8_t> data, vector<uint8_t> key)
{
	auto subkeys = generateKeys(key);
	auto result = vector<uint8_t>(data.size(), 0);
	auto block = vector<uint8_t>(8, 0);

	for (int i = 0; i < data.size() / 8; i++) {
		copy(data.begin() + 8 * i, data.begin() + 8 * i + 8, block.begin());
		auto eBlock = mutateBlock(block, subkeys);
		copy(eBlock.begin(), eBlock.begin() + 8, result.begin() + 8 * i);
	}

	return result;
}

vector<uint8_t> mutateBlock(vector<uint8_t> block, vector<uint32_t> keys)
{
	uint8_t* pBlock = block.data();

	uint32_t n1 = byteToUintCast(pBlock, 0);
	uint32_t n2 = byteToUintCast(pBlock, 4);

	for (int i = 0; i < 32; i++) {
		int keyIndex = i < 24 ? (i % 8) : (7 - i % 8);
		auto s = (n1 + keys[keyIndex]) % UINT32_MAX;
		s = subs(s);
		s = (s << 11) | (s >> 21);
		s = s ^ n2;

		if (i < 31)
		{
			n2 = n1;
			n1 = s;
		}
		else
		{
			n2 = s;
		}
	}

	vector<uint8_t> output(8, 0);
	uint8_t* n1_bytes = uintToByteCast(n1);
	uint8_t* n2_bytes = uintToByteCast(n2);

	for (int i = 0; i < 4; i++) {
		output[i] = n1_bytes[i];
		output[4 + i] = n2_bytes[i];
	}

	return output;
}

string gostDecrypt(string data, string key)
{
	int textLen = data.size();
	int keyLen = key.size();
	auto textBytes = data.c_str();
	auto keyBytes = key.c_str();

	auto textVector = vector<uint8_t>(textBytes, textBytes + textLen);
	auto keyVector = vector<uint8_t>(keyBytes, keyBytes + keyLen);
	auto res = gost(textVector, keyVector);
	auto decrypted = string(res.begin(), res.end());
	removeSpecChr(decrypted);
	return decrypted;
}

uint32_t subs(uint8_t value)
{
	uint32_t output = 0;

	for (int i = 0; i < 8; i++) {
		uint8_t temp = (uint8_t)(value >> (4 * i) & 0x0F);
		temp = S_BOX[i][temp];
		output |= (uint32_t)temp << (4 * i);
	}

	return output;
}

vector<uint32_t> generateKeys(vector<uint8_t> key)
{
	if (key.size() != 32) {
		throw exception("Key length is wrong!");
	}

	vector<uint32_t> keys(8, 0);
	uint8_t* pKey = key.data();

	for (int i = 0; i < 8; i++) {
		keys[i] = byteToUintCast(pKey, 4 * i);
	}

	return keys;
}

uint32_t byteToUintCast(uint8_t* pKey, int pos)
{
	return *reinterpret_cast<uint32_t*>(pKey + pos);
}

uint8_t* uintToByteCast(uint32_t buf)
{
	uint8_t* bytes = new uint8_t[4];
	for (int i = 0; i < 4; i++)
	{
		bytes[i] = (uint8_t)((buf >> (8 * i)) & 0xFF);
	}

	return bytes;
}
