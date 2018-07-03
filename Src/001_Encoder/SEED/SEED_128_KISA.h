/*******************************************************************************
*
* FILE:         SEED_KISA.h
*
* DESCRIPTION:  header file for SEED_KISA.c
*
*******************************************************************************/

#ifndef SEED_H
#define SEED_H


/******************************* Include files ********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/******************************* Type Definitions *****************************/

// If your machine uses different data types, you must change them to proper ones.
#ifndef TYPE_DEFINITION
    #define TYPE_DEFINITION
    #if defined(__alpha)
        typedef unsigned int        DWORD;		// unsigned 4-byte data type
        typedef unsigned short      WORD;		// unsigned 2-byte data type
    #else
#ifdef _MSC_VER
		typedef unsigned long 		DWORD;		// unsigned 4-byte data type(windows)
#else
		typedef unsigned int 		DWORD;		// unsigned 4-byte data type(linux)
#endif
        typedef unsigned short int  WORD;		// unsigned 2-byte data type
    #endif
    typedef unsigned char           BYTE;		// unsigned 1-byte data type
#endif


/***************************** Endianness Define ******************************/
// If endianness is not defined correctly, you must modify here.
// SEED uses the Little endian as a defalut order

#if __alpha__	||	__alpha	||	__i386__	||	i386	||	_M_I86	||	_M_IX86	||	\
	__OS2__		||	sun386	||	__TURBOC__	||	vax		||	vms		||	VMS		||	__VMS 
#define LITTLE_ENDIAN
#else
#ifndef BIG_ENDIAN
#define BIG_ENDIAN
#endif
#endif


/**************************** Constant Definitions ****************************/

#define NoRounds         16						// the number of rounds
#define NoRoundKeys      (NoRounds*2)			// the number of round-keys
#define SeedBlockSize    16    					// block length in bytes
#define SeedBlockLen     128   					// block length in bits


/******************************** Common Macros *******************************/

// macroses for left or right rotations
#if defined(_MSC_VER)
    #define ROTL(x, n)     (_lrotl((x), (n)))		// left rotation
    #define ROTR(x, n)     (_lrotr((x), (n)))		// right rotation
#else
    #define ROTL(x, n)     (((x) << (n)) | ((x) >> (32-(n))))		// left rotation
    #define ROTR(x, n)     (((x) >> (n)) | ((x) << (32-(n))))		// right rotation
#endif

// macroses for converting endianess
#define EndianChange(dwS)                       \
    ( (ROTL((dwS),  8) & (DWORD)0x00ff00ff) |   \
      (ROTL((dwS), 24) & (DWORD)0xff00ff00) )


/*************************** Function Declarations ****************************/
namespace seed128
{
	void SeedEncrypt(		/* encryption function */
		BYTE *pbData, 				// [in,out]	data to be encrypted
		DWORD *pdwRoundKey			// [in]			round keys for encryption
		);

	void SeedDecrypt(		/* decryption function */
		BYTE *pbData, 				// [in,out]	data to be decrypted
		DWORD *pdwRoundKey			// [in]			round keys for decryption
		);

	void SeedRoundKey(		/* key scheduling function */
		DWORD *pdwRoundKey, 		// [out]		round keys for encryption or decryption
		const BYTE *pbUserKey				// [in]			secret user key 
		);
}
/*************************** END OF FILE **************************************/
#endif
