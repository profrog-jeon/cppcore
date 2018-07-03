/*
 * FILE:	sha2.c
 * AUTHOR :	Aaron D. Gifford - http://www.aarongifford.com/
 * MODIFY : Kong Ick Sun
 *
 * Copyright (c) 2000-2001, Aaron D. Gifford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTOR(S) ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  NO EVENT SHALL THE AUTHOR OR CONTRIBUTOR(S) BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id: sha2.c,v 1.1 2001/11/08 00:01:51 adg Exp adg $
 */

extern "C"
{ // extern open

#include <string.h>	/* memcpy()/memset() or bcopy()/bzero() */
#include <assert.h>	/* assert() */
#include "sha256.h"

/*
 * ASSERT NOTE:
 * Some sanity checking code is included using assert().  On my FreeBSD
 * system, this additional code can be removed by compiling with NDEBUG
 * defined.  Check your own systems manpage on assert() to see how to
 * compile WITHOUT the sanity checking code on your system.
 *
 * UNROLLED TRANSFORM LOOP NOTE:
 * You can define SHA2_UNROLL_TRANSFORM to use the unrolled transform
 * loop version for the hash transform rounds (defined using macros
 * later in this file).  Either define on the command line, for example:
 *
 *   cc -DSHA2_UNROLL_TRANSFORM -o sha2 sha2.c sha2prog.c
 *
 * or define below:
 *
 *   #define SHA2_UNROLL_TRANSFORM
 *
 */


/*** SHA-256/384/512 Machine Architecture Definitions *****************/
/*
 * BYTE_ORDER NOTE:
 *
 * Please make sure that your system defines BYTE_ORDER.  If your
 * architecture is little-endian, make sure it also defines
 * LITTLE_ENDIAN and that the two (BYTE_ORDER and LITTLE_ENDIAN) are
 * equivilent.
 *
 * If your system does not define the above, then you can do so by
 * hand like this:
 *
 *   #define LITTLE_ENDIAN 1234
 *   #define BIG_ENDIAN    4321
 *
 * And for little-endian machines, add:
 *
 *   #define BYTE_ORDER LITTLE_ENDIAN
 *
 * Or for big-endian machines:
 *
 *   #define BYTE_ORDER BIG_ENDIAN
 *
 * The FreeBSD machine this was written on defines BYTE_ORDER
 * appropriately by including <sys/types.h> (which in turn includes
 * <machine/endian.h> where the appropriate definitions are actually
 * made).
 */

/*
 * Define the followingsha2_* types to types of the correct length on
 * the native archtecture.   Most BSD systems and Linux define u_intXX_t
 * types.  Machines with very recent ANSI C headers, can use the
 * uintXX_t definintions from inttypes.h by defining SHA2_USE_INTTYPES_H
 * during compile or in the sha.h header file.
 *
 * Machines that support neither u_intXX_t nor inttypes.h's uintXX_t
 * will need to define these three typedefs below (and the appropriate
 * ones in sha.h too) by hand according to their system architecture.
 *
 * Thank you, Jun-ichiro itojun Hagino, for suggesting using u_intXX_t
 * types and pointing out recent ANSI C support for uintXX_t in inttypes.h.
 */
typedef u_int8_t  sha2_byte;	/* Exactly 1 byte */
typedef u_int32_t sha2_word32;	/* Exactly 4 bytes */

/*** SHA-256/384/512 Various Length Definitions ***********************/
/* NOTE: Most of these are in sha2.h */
#define SHA256_SHORT_BLOCK_LENGTH	(SHA256_BLOCK_LENGTH - 8)

/*** ENDIAN REVERSAL MACROS *******************************************/
#define REVERSE32(w,x)	{ \
	u_int32_t tmp = (w); \
	tmp = (tmp >> 16) | (tmp << 16); \
	(x) = ((tmp & 0xff00ff00UL) >> 8) | ((tmp & 0x00ff00ffUL) << 8); \
}

/*
 * Macro for incrementally adding the unsigned 64-bit integer n to the
 * unsigned 128-bit integer (represented using a two-element array of
 * 64-bit words):
 */
#define ADDINC128(w,n)	{ \
	(w)[0] += (sha2_word64)(n); \
	if ((w)[0] < (n)) { \
		(w)[1]++; \
	} \
}

/*
 * Macros for copying blocks of memory and for zeroing out ranges
 * of memory.  Using these macros makes it easy to switch from
 * using memset()/memcpy() and using bzero()/bcopy().
 *
 * Please define either SHA2_USE_MEMSET_MEMCPY or define
 * SHA2_USE_BZERO_BCOPY depending on which function set you
 * choose to use:
 */
#if !defined(SHA2_USE_MEMSET_MEMCPY) && !defined(SHA2_USE_BZERO_BCOPY)
/* Default to memset()/memcpy() if no option is specified */
#define	SHA2_USE_MEMSET_MEMCPY	1
#endif
#if defined(SHA2_USE_MEMSET_MEMCPY) && defined(SHA2_USE_BZERO_BCOPY)
/* Abort with an error if BOTH options are defined */
#error Define either SHA2_USE_MEMSET_MEMCPY or SHA2_USE_BZERO_BCOPY, not both!
#endif

#ifdef SHA2_USE_MEMSET_MEMCPY
#define MEMSET_BZERO(p,l)	memset((p), 0, (l))
#define MEMCPY_BCOPY(d,s,l)	memcpy((d), (s), (l))
#endif
#ifdef SHA2_USE_BZERO_BCOPY
#define MEMSET_BZERO(p,l)	bzero((p), (l))
#define MEMCPY_BCOPY(d,s,l)	bcopy((s), (d), (l))
#endif


/*** THE SIX LOGICAL FUNCTIONS ****************************************/
/*
 * Bit shifting and rotation (used by the six SHA-XYZ logical functions:
 *
 *   NOTE:  The naming of R and S appears backwards here (R is a SHIFT and
 *   S is a ROTATION) because the SHA-256/384/512 description document
 *   (see http://csrc.nist.gov/cryptval/shs/sha256-384-512.pdf) uses this
 *   same "backwards" definition.
 */
/* Shift-right (used in SHA-256, SHA-384, and SHA-512): */
#define R(b,x) 		((x) >> (b))
/* 32-bit Rotate-right (used in SHA-256): */
#define S32(b,x)	(((x) >> (b)) | ((x) << (32 - (b))))

/* Two of six logical functions used in SHA-256, SHA-384, and SHA-512: */
#define Ch(x,y,z)	(((x) & (y)) ^ ((~(x)) & (z)))
#define Maj(x,y,z)	(((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

/* Four of six logical functions used in SHA-256: */
#define Sigma0_256(x)	(S32(2,  (x)) ^ S32(13, (x)) ^ S32(22, (x)))
#define Sigma1_256(x)	(S32(6,  (x)) ^ S32(11, (x)) ^ S32(25, (x)))
#define sigma0_256(x)	(S32(7,  (x)) ^ S32(18, (x)) ^ R(3 ,   (x)))
#define sigma1_256(x)	(S32(17, (x)) ^ S32(19, (x)) ^ R(10,   (x)))


/*** INTERNAL FUNCTION PROTOTYPES *************************************/
/* NOTE: These should not be accessed directly from outside this
 * library -- they are intended for private internal visibility/use
 * only.
 */
void SHA256_Transform_Backup(SHA256_CTX*, const sha2_word32*);

/*** SHA-XYZ INITIAL HASH VALUES AND CONSTANTS ************************/
/* Hash constant words K for SHA-256: */
const static sha2_word32 K256[64] = {
	0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL,
	0x3956c25bUL, 0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL,
	0xd807aa98UL, 0x12835b01UL, 0x243185beUL, 0x550c7dc3UL,
	0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL, 0xc19bf174UL,
	0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
	0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL,
	0x983e5152UL, 0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL,
	0xc6e00bf3UL, 0xd5a79147UL, 0x06ca6351UL, 0x14292967UL,
	0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL, 0x53380d13UL,
	0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
	0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL,
	0xd192e819UL, 0xd6990624UL, 0xf40e3585UL, 0x106aa070UL,
	0x19a4c116UL, 0x1e376c08UL, 0x2748774cUL, 0x34b0bcb5UL,
	0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL, 0x682e6ff3UL,
	0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
	0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};

/* Initial hash value H for SHA-256: */
const static sha2_word32 sha256_initial_hash_value[8] = {
	0x6a09e667UL,
	0xbb67ae85UL,
	0x3c6ef372UL,
	0xa54ff53aUL,
	0x510e527fUL,
	0x9b05688cUL,
	0x1f83d9abUL,
	0x5be0cd19UL
};

/*
 * Constant used by SHA256/384/512_End() functions for converting the
 * digest to a readable hexadecimal character string:
 */
static const char *sha2_hex_digits = "0123456789abcdef";


/*** SHA-256: *********************************************************/
void E_SHA256_Init(SHA256_CTX* context) {
	if (context == (SHA256_CTX*)0) {
		return;
	}
	//MEMCPY_BCOPY(context->state, sha256_initial_hash_value, SHA256_DIGEST_LENGTH);
	context->state[0] = sha256_initial_hash_value[0];
	context->state[1] = sha256_initial_hash_value[1];
	context->state[2] = sha256_initial_hash_value[2];
	context->state[3] = sha256_initial_hash_value[3];
	context->state[4] = sha256_initial_hash_value[4];
	context->state[5] = sha256_initial_hash_value[5];
	context->state[6] = sha256_initial_hash_value[6];
	context->state[7] = sha256_initial_hash_value[7];

	MEMSET_BZERO(context->buffer, SHA256_BLOCK_LENGTH);
	context->bitcount[0] = context->bitcount[1] = 0;
}

#define ROUND256_0_TO_15(a,b,c,d,e,f,g,h)	\
	T1 = (h) + Sigma1_256(e) + Ch((e), (f), (g)) + \
	     K256[j] + (W256[j] = *data++); \
	(d) += T1; \
	(h) = T1 + Sigma0_256(a) + Maj((a), (b), (c)); \
	j++

#define ROUND256(a,b,c,d,e,f,g,h)	\
	s0 = W256[(j+1)&0x0f]; \
	s0 = sigma0_256(s0); \
	s1 = W256[(j+14)&0x0f]; \
	s1 = sigma1_256(s1); \
	T1 = (h) + Sigma1_256(e) + Ch((e), (f), (g)) + K256[j] + \
	     (W256[j&0x0f] += s1 + W256[(j+9)&0x0f] + s0); \
	(d) += T1; \
	(h) = T1 + Sigma0_256(a) + Maj((a), (b), (c)); \
	j++

void SHA256_Transform_Backup(SHA256_CTX* context, const sha2_word32* data) {
	sha2_word32	a, b, c, d, e, f, g, h, s0, s1, T1, T2;
	sha2_word32 *W256;
	int		j;

	W256 = (sha2_word32*)context->buffer;

	/* Initialize registers with the prev. intermediate value */
	a = context->state[0];
	b = context->state[1];
	c = context->state[2];
	d = context->state[3];
	e = context->state[4];
	f = context->state[5];
	g = context->state[6];
	h = context->state[7];

	j = 0;
	do {
		/* Copy data while converting to host byte order */
		REVERSE32(*data++,W256[j]);

		/* Apply the SHA-256 compression function to update a..h */
		T1 = h + Sigma1_256(e) + Ch(e, f, g) + K256[j] + W256[j];

		/* Apply the SHA-256 compression function to update a..h with copy */
		T2 = Sigma0_256(a) + Maj(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + T1;
		d = c;
		c = b;
		b = a;
		a = T1 + T2;

		j++;
	} while (j < 16);

	do {
		/* Part of the message block expansion: */
		s0 = W256[(j+1)&0x0f];
		s0 = sigma0_256(s0);
		s1 = W256[(j+14)&0x0f];
		s1 = sigma1_256(s1);

		/* Apply the SHA-256 compression function to update a..h */
		T1 = h + Sigma1_256(e) + Ch(e, f, g) + K256[j] +
		     (W256[j&0x0f] += s1 + W256[(j+9)&0x0f] + s0);
		T2 = Sigma0_256(a) + Maj(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + T1;
		d = c;
		c = b;
		b = a;
		a = T1 + T2;

		j++;
	} while (j < 64);

	

	/* Compute the current intermediate hash value */
	context->state[0] += a;
	context->state[1] += b;
	context->state[2] += c;
	context->state[3] += d;
	context->state[4] += e;
	context->state[5] += f;
	context->state[6] += g;
	context->state[7] += h;

	/* Clean up */
	a = b = c = d = e = f = g = h = T1 = T2 = 0;
}

void E_SHA256_Update(SHA256_CTX* context, const sha2_byte *data, size_t len) {
	unsigned int	freespace, usedspace;

	u_int32_t nPrevBitCount = 0;

	if (len == 0) {
		/* Calling with no data is valid - we do nothing */
		return;
	}
	
	/* Sanity check: */
	assert(context != (SHA256_CTX*)0 && data != (sha2_byte*)0);

	// 한 블럭(32bytes)에서 사용한 공간을 계산
	usedspace = (context->bitcount[0] >> 3) % SHA256_BLOCK_LENGTH;
	/* Calculate how much free space is available in the buffer */

	nPrevBitCount = context->bitcount[0];
	context->bitcount[0] += (u_int32_t)(len << 3);
	if(context->bitcount[0] < nPrevBitCount)
	{
		context->bitcount[1]++;
	}
	context->bitcount[1] += (u_int32_t)(len >> 29);

	// 이전에 Update가 수행되어 처리된 데이터가 있으면 그 뒤에 새 데이터를 추가하여 계산
	if(usedspace > 0)
	{
		// 한 블럭에서의 남은 버퍼 공간의 크기
		freespace = SHA256_BLOCK_LENGTH - usedspace;

		// Hash할 데이터의 크기가 남은 버퍼 공간보다 크면 Transform 수행
		if (len >= freespace) {
			/* Fill the buffer completely and process it */
			MEMCPY_BCOPY(&context->buffer[usedspace], data, freespace);
			SHA256_Transform_Backup(context, (sha2_word32*)context->buffer);

			len -= freespace;
			data += freespace;

		} else {
			/* The buffer is not yet full */
			// 버퍼에 데이터 저장
			MEMCPY_BCOPY(&context->buffer[usedspace], data, len);
			/* Clean up: */
			usedspace = freespace = 0;
			return;
		}
	}

	// 한 블럭 크기 이상의 나머지 데이터를 Transform
	while (len >= SHA256_BLOCK_LENGTH) {
		/* Process as many complete blocks as we can */
		SHA256_Transform_Backup(context, (sha2_word32*)data);
//		context->bitcount += SHA256_BLOCK_LENGTH << 3;
		len -= SHA256_BLOCK_LENGTH;
		data += SHA256_BLOCK_LENGTH;
	}

	// 한 블럭 크기가 안되는 남은 데이터를 버퍼에 저장
	if (len >= 0) {
		/* There's left-overs, so save 'em */
		MEMCPY_BCOPY(context->buffer, data, len);
	}
	/* Clean up: */
	usedspace = freespace = 0;
}

void E_SHA256_Final(sha2_byte digest[], SHA256_CTX* context)
{
	sha2_word32	*d = (sha2_word32*)digest;
	unsigned int	usedspace = 0;
	unsigned int	CountL = 0;
 	unsigned int	CountH = 0;
	u_int32_t		temp = 0;

	/* Sanity check: */
	assert(context != (SHA256_CTX*)0);

	CountL = context->bitcount[0];
	CountH = context->bitcount[1];

	/* If no digest buffer is passed, we don't bother doing this: */
	if (digest != (sha2_byte*)0)
	{
		// 처리되지 않은 데이터의 크기
		// (블럭을 채우고 남은 처리되지 않은 데이터를 여기서 패딩 붙이고 처리한다.)
		usedspace = (CountL >> 3) % SHA256_BLOCK_LENGTH;

		/* Convert FROM host byte order */
		temp = CountH;
		REVERSE32(CountL, CountH);
		REVERSE32(temp, CountL);

		// 한 블럭이 안되는 데이터 처리
		if (usedspace > 0) {
			/* Begin padding with a 1 bit: */
			// 데이터 끝에 "10000000" 를 append
			context->buffer[usedspace++] = 0x80;

			// 블럭 끝에 데이터의 크기(64bit 필요)를 추가할 수 있을 때
			if (usedspace <= SHA256_SHORT_BLOCK_LENGTH) {
				/* Set-up for the last transform: */
				MEMSET_BZERO(&context->buffer[usedspace], SHA256_SHORT_BLOCK_LENGTH - usedspace);
			} else {
				// 블럭 끝까지 0으로 채우고, Transform 수행(하나의 패딩 블럭이 추가된다.)
				if (usedspace < SHA256_BLOCK_LENGTH) {
					MEMSET_BZERO(&context->buffer[usedspace], SHA256_BLOCK_LENGTH - usedspace);
				}
				/* Do second-to-last transform: */
				SHA256_Transform_Backup(context, (sha2_word32*)context->buffer);

				/* And set-up for the last transform: */
				MEMSET_BZERO(context->buffer, SHA256_SHORT_BLOCK_LENGTH);
			}
		} else {
			// 처리되지 않은 데이터가 없을 때,
			// 마지막 블럭은 데이터가 없는 패딩 블럭이 된다.(1000000....000)
			/* Set-up for the last transform: */
			MEMSET_BZERO(context->buffer, SHA256_SHORT_BLOCK_LENGTH);

			/* Begin padding with a 1 bit: */
			*context->buffer = 0x80;
		}
		/* Set the bit count: */
		// 블럭 끝에 데이터(현재 블럭) 크기 추가
		sha2_word32 *w32Tmp = (sha2_word32*)&context->buffer[SHA256_BLOCK_LENGTH-4];
		*w32Tmp = CountH;
		w32Tmp = (sha2_word32*)&context->buffer[SHA256_BLOCK_LENGTH-8];
	   *w32Tmp = CountL;

		/* Final transform: */
		// 블럭이 완성됐으니 Transform
		SHA256_Transform_Backup(context, (sha2_word32*)context->buffer);

		/* Convert TO host byte order */
		{
			int	j;
			for (j = 0; j < 8; j++) {
				REVERSE32(context->state[j],context->state[j]);
				*d++ = context->state[j];
			}
		}
	}

	/* Clean up state data: */
	size_t tSize = sizeof(context);
	MEMSET_BZERO(context, tSize);
	usedspace = 0;
}

char * E_SHA256_End(SHA256_CTX* context, char buffer[]) {
	sha2_byte	digest[SHA256_DIGEST_LENGTH], *d = digest;
	int		i;

	/* Sanity check: */
	assert(context != (SHA256_CTX*)0);

	if (buffer != (char*)0) {
		E_SHA256_Final(digest, context);

		for (i = 0; i < SHA256_DIGEST_LENGTH; i++) {
			*buffer++ = sha2_hex_digits[(*d & 0xf0) >> 4];
			*buffer++ = sha2_hex_digits[*d & 0x0f];
			d++;
		}
		*buffer = (char)0;
	} else {
		size_t tSize = sizeof(context);
		MEMSET_BZERO(context, tSize);
	}
	MEMSET_BZERO(digest, SHA256_DIGEST_LENGTH);
	return buffer;
}

char* E_SHA256_Data(const sha2_byte* data, size_t len, char digest[SHA256_DIGEST_STRING_LENGTH])
{
	SHA256_CTX	context;

	E_SHA256_Init(&context);
	E_SHA256_Update(&context, data, len);
	return E_SHA256_End(&context, digest);
}

void E_SHA256(u_int8_t *out, u_int8_t *in, int inbytes)
{
	SHA256_CTX ctx;

	E_SHA256_Init(&ctx);
	E_SHA256_Update(&ctx, in, inbytes);
	E_SHA256_Final(out, &ctx );
}

} // extern close
