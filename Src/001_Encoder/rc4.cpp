#include "stdafx.h"
#include "rc4.h"
namespace core {
	void RC4Init(RC4_CTX* rcContext, const unsigned char* key, DWORD dwKeyLength)
	{
		BYTE K[256];

		WORD wIndex = 0;
		for (wIndex = 0; wIndex < 256; wIndex++)
		{
			rcContext->SBox[wIndex] = wIndex;
			K[wIndex] = key[wIndex % dwKeyLength];
		}

		WORD wSwapPointerIndex = 0;
		for (wIndex = 0; wIndex < 256; wIndex++)
		{
			wSwapPointerIndex = (wSwapPointerIndex + rcContext->SBox[wIndex] + K[wIndex]) % 256;
			Rc4Swap(rcContext->SBox[wIndex], rcContext->SBox[wSwapPointerIndex]);
		}

		rcContext->wIndex = 0;
		rcContext->wSwapPointerIndex = 0;
	}

	void RC4Update(RC4_CTX* rcContext, unsigned char* strBuf, DWORD dwBufLength)
	{
		BYTE temp = 0;

		for (DWORD counter = 0; counter < dwBufLength; counter++)
		{
			rcContext->wIndex = (rcContext->wIndex + 1) % 256;
			rcContext->wSwapPointerIndex = (rcContext->wSwapPointerIndex + rcContext->SBox[rcContext->wIndex]) % 256;

			Rc4Swap(rcContext->SBox[rcContext->wIndex], rcContext->SBox[rcContext->wSwapPointerIndex]);
			
			temp = (rcContext->SBox[rcContext->wIndex] + rcContext->SBox[rcContext->wSwapPointerIndex]) % 256;

			strBuf[counter] ^= rcContext->SBox[temp];
		}
	}

	void Rc4Swap(BYTE& bLeft, BYTE& bRight)
	{
		BYTE bTemp = bLeft;
		bLeft = bRight;
		bRight = bTemp;
	}
}