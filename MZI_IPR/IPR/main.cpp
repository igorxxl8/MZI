// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

#include "common.h"
#include "des.h"
#include <Windows.h>

using namespace std;

int main()
{
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);
	string desData = readFile("des_data.txt");
	auto text_key = split(desData, ',');
	tuple<string, string> encrypted = desEncrypt(text_key[0], text_key[1]);
	tuple<string, string> dectypted = desDecrypt(encrypted._Myfirst._Val, encrypted._Get_rest()._Myfirst._Val);
}



