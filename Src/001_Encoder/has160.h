/*!
 * file has160.h
 * brief HAS160 Digest Algorithm(TTAS.KO-12.0011/R1(HAS160))
 * author
 * Copyright (c) 2011 by <KISA>
 */
#ifndef HAS160_HEADER
#define HAS160_HEADER

#ifdef  __cplusplus
extern "C" {
#endif

#define HAS160_BLOCK_SIZE		64
#define HAS160_DIGEST_LENGTH	20

	typedef struct has160_state_st{
		unsigned int state[5];
		unsigned int length[2];
		unsigned char data[64];
	} KISA_HAS160;

	int KISA_HAS160_init(KISA_HAS160 *has160);
	int KISA_HAS160_update(KISA_HAS160 *has160, const unsigned char *data, unsigned int length);
	int KISA_HAS160_final(KISA_HAS160 *has160, unsigned char* md);
	int KISA_HAS160_MD(unsigned char *in, int len, unsigned char *out);
#ifdef  __cplusplus
}
#endif

#endif

