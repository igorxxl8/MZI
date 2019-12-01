#include "gost3411.h"

typedef uint8_t block[32]; 
typedef uint8_t block32[4];

void A(uint8_t* Y, uint8_t* R) {
	for (int i = 0; i < 24; i++) {
		R[i] = Y[i + 8];
	}
	for (int i = 0; i < 8; i++) {
		R[i + 24] = Y[i] ^ Y[i + 8];
	}
}

int fi(int arg) {
	int i = arg & 0x03;
	int k = arg >> 2; k++;
	return (i << 3) + k - 1;
}
void P(uint8_t Y[], uint8_t R[]) {
	for (int i = 0; i < 32; i++) { 
		R[i] = Y[fi(i)];
	}
}

void psi(uint8_t* arr) {
	uint8_t y16[] = { 0, 0 };
	y16[0] ^= arr[0] ^ arr[2] ^ arr[4] ^ arr[6] ^ arr[24] ^ arr[30];
	y16[1] ^= arr[1] ^ arr[3] ^ arr[5] ^ arr[7] ^ arr[25] ^ arr[31];
	
	for (int i = 0; i < 30; i++) {
		arr[i] = arr[i + 2];
	}

	arr[30] = y16[0];
	arr[31] = y16[1];
}

void psi(uint8_t* arr, int p) {
	while (p--) psi(arr);
}

void f(uint8_t* H, uint8_t* M, uint8_t* newH) {
	block C[4] = {
		{},
		{},
		{0x00, 	 0xFF,	 0x00,	 0xFF,	 0x00,	 0xFF,	 0x00,	 0xFF,	 0xFF,	 0x00,	 0xFF,	 0x00,	 0xFF,	 0x00,	 0xFF,	 0x00,	 0x00,	 0xFF,	 0xFF,	 0x00,	 0xFF,	 0x00,	 0x00,	 0xFF,	 0xFF,	 0x00,	 0x00,	 0x00,	 0xFF,	 0xFF,	 0x00,	 0xFF}
	};

	memset(C, 0, sizeof C);
	block U, V, W, K[4], tmp;
	memcpy(U, H, sizeof U);
	memcpy(V, M, sizeof V);
	for (int i = 0; i < 32; i++) {
		W[i] = U[i] ^ V[i];
	}

	P(W, K[0]);

	for (int step = 1; step < 4; step++) {
		A(U, tmp); for (int i = 0; i < 32; i++) U[i] = tmp[i] ^ C[step][i];
		A(V, tmp); A(tmp, V);
		for (int i = 0; i < 32; i++) {
			W[i] = U[i] ^ V[i];
		}
		P(W, K[step]);
	}

	block S;
	for (int i = 0; i < 32; i += 8) {
		gost(H + i, K[i >> 3], S + i);
	}

	psi(S, 12);
	for (int i = 0; i < 32; i++) {
		S[i] ^= M[i];
	}

	psi(S, 1);

	for (int i = 0; i < 32; i++) {
		S[i] ^= H[i];
	}

	psi(S, 61);
	memcpy(newH, S, sizeof S);
}

void gostHash(uint8_t* data, int len, uint8_t* result) {
	block block, Sum, L, H, newH;
	int pos = 0, posIB = 0;

	memset(Sum, 0, sizeof Sum);
	memset(H, 0, sizeof H);

	while ((posIB < len) || pos) {
		if (posIB < len) block[pos++] = data[posIB++];
		else block[pos++] = 0;
		if (pos == 32) {
			pos = 0;
			int c = 0;
			for (int i = 0; i < 32; i++) {
				c += block[i] + Sum[i];
				Sum[i] = c & 0xFF;
				c >>= 8;
			}

			f(H, block, newH);
			memcpy(H, newH, sizeof newH);
		}
	}

	memset(L, 0, sizeof L);
	int c = len << 3;
	for (int i = 0; i < 32; i++) {
		L[i] = c & 0xFF;
		c >>= 8;
	}

	f(H, L, newH); 
	memcpy(H, newH, sizeof newH);
	f(H, Sum, newH); 
	memcpy(result, newH, sizeof newH);
}
