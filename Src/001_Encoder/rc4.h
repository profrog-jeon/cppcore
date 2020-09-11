#pragma once

namespace core {

	typedef struct {
		WORD wIndex;
		WORD wSwapPointerIndex;
		BYTE SBox[256];
	} RC4_CTX;

	void RC4Init(RC4_CTX* rcContext, const unsigned char* key, DWORD dwKeyLength);
	void RC4Update(RC4_CTX* rcContext, unsigned char* strBuf, DWORD dwBufLength);
	void Rc4Swap(BYTE& bLeft, BYTE& bRight);
}
