#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "has160.h"

extern "C"
{ // extern open

#define GATE_1(x, y, z)		( (z)^( (x) & ((y)^(z)) ) )
#define GATE_2(x, y, z)		( (x) ^ (y) ^ (z)         )
#define GATE_3(x, y, z)		( (y)^( (x) | ~(z)      ) )

#define ROTL(x, n)     (((x) << (n)) | ((x) >> (32-(n))))		// left rotation

#define HAS160_F(a, b, c, d, e, x, s)        {         \
	(e) += ROTL(a, s) + GATE_1(b, c, d) + (x);               \
	(b) = ROTL(b, 10);                                  \
   }

#define HAS160_G(a, b, c, d, e, x, s)        {         \
	(e) += ROTL(a, s) + GATE_2(b, c, d) + (x) + 0x5A827999L; \
	(b) = ROTL(b, 17);                                  \
   }

#define HAS160_H(a, b, c, d, e, x, s)        {         \
	(e) += ROTL(a, s) + GATE_3(b, c, d) + (x) + 0x6ED9EBA1L; \
	(b) = ROTL(b, 25);                                  \
   }

#define HAS160_G_(a, b, c, d, e, x, s)        {         \
	(e) += ROTL(a, s) + GATE_2(b, c, d) + (x) + 0x8F1BBCDCL; \
	(b) = ROTL(b, 30);                                  \
   }

static void HAS160_compute(KISA_HAS160 *has160);

int KISA_HAS160_init(KISA_HAS160 *has160)
{	
	if(has160 == NULL) {
		return 0;
	}

	has160->state[0] = 0x67452301;	has160->state[1] = 0xefcdab89;
	has160->state[2] = 0x98badcfe;	has160->state[3] = 0x10325476;
	has160->state[4] = 0xc3d2e1f0;
	has160->length[0] = 0;			has160->length[1] = 0;

	return 1;
}

int KISA_HAS160_update(KISA_HAS160 *has160, const unsigned char *data, unsigned int length)
{
	unsigned int len1, len2;
	unsigned int tmp;

	if(has160 == NULL)
		return 0;

	len1 = (has160->length[0] >> 3) & 0x3f;
	len2 = HAS160_BLOCK_SIZE - len1;

//	if (has160->length[0] > (has160->length[0] += (length << 3)))
//		++has160->length[1];
	tmp = has160->length[0];
	has160->length[0] += (length << 3);
	if (tmp > has160->length[0])
		++has160->length[1];

	has160->length[1] += (length >> 29);

	if(length < len2) {
		memcpy(has160->data+len1, data, length);
		return 1;
	}
	else
	{
		memcpy(has160->data+len1, data, len2);
		HAS160_compute(has160);
		data += len2;
		length -= len2;

		while(length >= HAS160_BLOCK_SIZE) {
			memcpy(has160->data, data, HAS160_BLOCK_SIZE);
			HAS160_compute(has160);
			data += HAS160_BLOCK_SIZE;
			length -= HAS160_BLOCK_SIZE;
		}

		memcpy(has160->data, data, length);
		return 1;
	}
}

int KISA_HAS160_final(KISA_HAS160 *has160, unsigned char *out)
{
	unsigned int len;
	unsigned char *pTmp = NULL;

	if(has160 == NULL) {
		return 0;
	}

	len = (has160->length[0] >> 3) & 0x3f;

	has160->data[len++] = 0x80;
	if (len > 56) {
		memset(has160->data + len, 0x00, HAS160_BLOCK_SIZE-len);
		HAS160_compute(has160);
		memset(has160->data, 0x00, 56);
	} 
	else {
		memset(has160->data+len, 0x00, 56-len);
	}

	pTmp = has160->data;
	//((unsigned int *)has160->data)[14] = has160->length[0];
	((unsigned int *)pTmp)[14] = has160->length[0];
	pTmp = has160->data;
	//((unsigned int *)has160->data)[15] = has160->length[1];
	((unsigned int *)pTmp)[15] = has160->length[1];

	HAS160_compute(has160);

	((unsigned int *)out)[0] = has160->state[0];
	((unsigned int *)out)[1] = has160->state[1];
	((unsigned int *)out)[2] = has160->state[2];
	((unsigned int *)out)[3] = has160->state[3];
	((unsigned int *)out)[4] = has160->state[4];

	return 1;
}

static void HAS160_compute(KISA_HAS160 *has160)
{
	unsigned int a, b, c, d, e;
	unsigned int temp;

	unsigned int *X;
	X = (unsigned int *)has160->data;

	a=has160->state[0];		b=has160->state[1];		c=has160->state[2];
	d=has160->state[3];		e=has160->state[4];

	temp = X[ 8]^X[ 9]^X[10]^X[11];		HAS160_F(a, b, c, d, e, temp ,  5); 
	HAS160_F(e, a, b, c, d, X[ 0], 11);	HAS160_F(d, e, a, b, c, X[ 1],  7); 
	HAS160_F(c, d, e, a, b, X[ 2], 15);	HAS160_F(b, c, d, e, a, X[ 3],  6); 

	temp = X[12]^X[13]^X[14]^X[15];		HAS160_F(a, b, c, d, e, temp , 13); 
	HAS160_F(e, a, b, c, d, X[ 4],  8);	HAS160_F(d, e, a, b, c, X[ 5], 14); 
	HAS160_F(c, d, e, a, b, X[ 6],  7);	HAS160_F(b, c, d, e, a, X[ 7], 12); 

	temp = X[ 0]^X[ 1]^X[ 2]^X[ 3];		HAS160_F(a, b, c, d, e, temp ,  9); 
	HAS160_F(e, a, b, c, d, X[ 8], 11);	HAS160_F(d, e, a, b, c, X[ 9],  8); 
	HAS160_F(c, d, e, a, b, X[10], 15);	HAS160_F(b, c, d, e, a, X[11],  6); 

	temp = X[ 4]^X[ 5]^X[ 6]^X[ 7];		HAS160_F(a, b, c, d, e, temp , 12); 
	HAS160_F(e, a, b, c, d, X[12],  9);	HAS160_F(d, e, a, b, c, X[13], 14); 
	HAS160_F(c, d, e, a, b, X[14],  5);	HAS160_F(b, c, d, e, a, X[15], 13); 

	temp = X[11]^X[14]^X[ 1]^X[ 4];		HAS160_G(a, b, c, d, e, temp ,  5);
	HAS160_G(e, a, b, c, d, X[ 3], 11);	HAS160_G(d, e, a, b, c, X[ 6],  7);
	HAS160_G(c, d, e, a, b, X[ 9], 15);	HAS160_G(b, c, d, e, a, X[12],  6);

	temp = X[ 7]^X[10]^X[13]^X[ 0];		HAS160_G(a, b, c, d, e, temp , 13);
	HAS160_G(e, a, b, c, d, X[15],  8);	HAS160_G(d, e, a, b, c, X[ 2], 14);
	HAS160_G(c, d, e, a, b, X[ 5],  7);	HAS160_G(b, c, d, e, a, X[ 8], 12);

	temp = X[ 3]^X[ 6]^X[ 9]^X[12];		HAS160_G(a, b, c, d, e, temp ,  9);
	HAS160_G(e, a, b, c, d, X[11], 11);	HAS160_G(d, e, a, b, c, X[14],  8);
	HAS160_G(c, d, e, a, b, X[ 1], 15);	HAS160_G(b, c, d, e, a, X[ 4],  6);

	temp = X[15]^X[ 2]^X[ 5]^X[ 8];		HAS160_G(a, b, c, d, e, temp , 12);
	HAS160_G(e, a, b, c, d, X[ 7],  9);	HAS160_G(d, e, a, b, c, X[10], 14);
	HAS160_G(c, d, e, a, b, X[13],  5);	HAS160_G(b, c, d, e, a, X[ 0], 13);

	temp = X[ 4]^X[13]^X[ 6]^X[15];		HAS160_H(a, b, c, d, e, temp ,  5);
	HAS160_H(e, a, b, c, d, X[12], 11);	HAS160_H(d, e, a, b, c, X[ 5],  7);
	HAS160_H(c, d, e, a, b, X[14], 15);	HAS160_H(b, c, d, e, a, X[ 7],  6);

	temp = X[ 8]^X[ 1]^X[10]^X[ 3];		HAS160_H(a, b, c, d, e, temp , 13);
	HAS160_H(e, a, b, c, d, X[ 0],  8);	HAS160_H(d, e, a, b, c, X[ 9], 14);
	HAS160_H(c, d, e, a, b, X[ 2],  7);	HAS160_H(b, c, d, e, a, X[11], 12);

	temp = X[12]^X[ 5]^X[14]^X[ 7];		HAS160_H(a, b, c, d, e, temp ,  9);
	HAS160_H(e, a, b, c, d, X[ 4], 11);	HAS160_H(d, e, a, b, c, X[13],  8);
	HAS160_H(c, d, e, a, b, X[ 6], 15);	HAS160_H(b, c, d, e, a, X[15],  6);

	temp = X[ 0]^X[ 9]^X[ 2]^X[11];		HAS160_H(a, b, c, d, e, temp , 12);
	HAS160_H(e, a, b, c, d, X[ 8],  9);	HAS160_H(d, e, a, b, c, X[ 1], 14);
	HAS160_H(c, d, e, a, b, X[10],  5);	HAS160_H(b, c, d, e, a, X[ 3], 13);

	temp = X[15]^X[10]^X[ 5]^X[ 0];			HAS160_G_(a, b, c, d, e, temp ,  5);
	HAS160_G_(e, a, b, c, d, X[ 7], 11);	HAS160_G_(d, e, a, b, c, X[ 2],  7);
	HAS160_G_(c, d, e, a, b, X[13], 15);	HAS160_G_(b, c, d, e, a, X[ 8],  6);

	temp = X[11]^X[ 6]^X[ 1]^X[12];			HAS160_G_(a, b, c, d, e, temp , 13);
	HAS160_G_(e, a, b, c, d, X[ 3],  8);	HAS160_G_(d, e, a, b, c, X[14], 14);
	HAS160_G_(c, d, e, a, b, X[ 9],  7);	HAS160_G_(b, c, d, e, a, X[ 4], 12);

	temp = X[ 7]^X[ 2]^X[13]^X[ 8];			HAS160_G_(a, b, c, d, e, temp ,  9);
	HAS160_G_(e, a, b, c, d, X[15], 11);	HAS160_G_(d, e, a, b, c, X[10],  8);
	HAS160_G_(c, d, e, a, b, X[ 5], 15);	HAS160_G_(b, c, d, e, a, X[ 0],  6);

	temp = X[ 3]^X[14]^X[ 9]^X[ 4];			HAS160_G_(a, b, c, d, e, temp , 12);
	HAS160_G_(e, a, b, c, d, X[11],  9);	HAS160_G_(d, e, a, b, c, X[ 6], 14);
	HAS160_G_(c, d, e, a, b, X[ 1],  5);	HAS160_G_(b, c, d, e, a, X[12], 13);

	has160->state[0] += a;	has160->state[1] += b;	has160->state[2] += c;
	has160->state[3] += d;	has160->state[4] += e;
}


int KISA_HAS160_MD(unsigned char *in, int len, unsigned char *out)
{
	KISA_HAS160 has;
	if(!KISA_HAS160_init(&has))
		return 0;

	if(!KISA_HAS160_update(&has,in,len))
		return 0;

	if(!KISA_HAS160_final(&has,out))
		return 0;

	memset(&has, 0x00, sizeof(KISA_HAS160));

	return HAS160_DIGEST_LENGTH;
}

} // extern close
