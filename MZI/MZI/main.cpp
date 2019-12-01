// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <conio.h>

#include "gost28147.h"
#include "gost3411.h"
#include "common.h"
#include "des.h"
#include "rsa.h"

using namespace std;

void ipr() {
	cout << "DES" << endl;
	string desData = readFile("des_data.txt");
	auto text_key = split(desData, ',');
	pair<string, string> encryptedDes = desEncrypt(text_key[0], text_key[1]);
	pair<string, string> dectyptedDes = desDecrypt(encryptedDes.first, encryptedDes.second);


	cout << "Gost 28147" << endl;
	string gostData = readFile("gost_data.txt");
	auto gost_text_key = split(gostData, ',');
	string encryptedGost = gostEncrypt(gost_text_key[0], gost_text_key[1]);
	string decryptedGost = gostDecrypt(encryptedGost, gost_text_key[1]);
	cout << "Encrypted Gost: " << encryptedGost << endl;
	cout << "Dectypted Gost: " << decryptedGost << endl;

	cout << "RSA" << endl;
	string rsaData = readFile("rsa_data.txt");
	auto message_a_b = split(rsaData, ',');
	pair<pair<int, int>, pair<int, int>> keys = createKeys(s_int(message_a_b[0]), s_int(message_a_b[1]));
	pair<int, int> privateKey = keys.first;
	pair<int, int> publicKey = keys.second;
	cout << "Private key: " << privateKey.second << endl << "Public key: " << publicKey.second << endl;
	vector<int> encryptedRsa = encryptRsa(publicKey, message_a_b[2]);
	cout << "Encrypted bytes: " << endl;
	for (auto it = encryptedRsa.begin(); it != encryptedRsa.end(); it++) {
		cout << *it << " ";
	}
	string decryptedRsa = decryptRsa(privateKey, encryptedRsa);
	cout << endl << "Decrypted: " << decryptedRsa << endl;
}

void kr() {
	
	string hash_data = readFile("gost_hash.txt");

	auto strs = split(hash_data, ',');

	int i = 1;
	for (auto it = strs.begin(); it != strs.end(); it++) {
		auto str = *it;
		uint8_t result[32];
		gostHash((uint8_t*)(str.c_str()), str.length(), result);
		cout << i++ << ") " << str << " -> " << "0x";
		for (int i = 31; i >= 0; i--)
			printf("%02X", result[i]);

		cout << endl;
	}
}

int main()
{
	srand(uint32_t(time(0)));
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);
	//ipr();
	kr();

	_getch();
}



