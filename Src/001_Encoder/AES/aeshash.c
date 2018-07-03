#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h> 

#include "aes.h"

enum {
  l_c = 0,
  r_c = 1
};

static const unsigned char key[2][32] = {{0x23, 0xce, 0x45, 0x19, 0xc5, 0xb6, 0xb5, 0x82,
				 0x19, 0x14, 0x36, 0x26, 0xa4, 0xe0, 0x9c, 0xa2},
				{0x20, 0xce, 0x49, 0x19, 0xc5, 0xb6, 0xd5, 0xe2,
				 0xb6, 0x1d, 0xf6, 0x56, 0xa5, 0xef, 0xfc, 0xe2}};
static aes_ctx ctx[2];

void aeshash_type(const unsigned char in[], unsigned char out[], int type)
{
  memset(out, 0, BLOCK_SIZE);
  aes_enc_blk(in, out, ctx + type);
  ((unsigned __int64 *) out)[0] ^= ((unsigned __int64 *) in)[0];
  ((unsigned __int64 *) out)[1] ^= ((unsigned __int64 *) in)[1];
}
void aeshash_l(const unsigned char in[], unsigned char out[])
{
  aeshash_type(in, out, l_c);
}
void aeshash_r(const unsigned char in[], unsigned char out[])
{
  aeshash_type(in, out, r_c);
}

void aeshash_init_type(int type)
{
  ctx[type].n_rnd = 0; // ensure all flags are initially set to zero
  ctx[type].n_blk = 0;
  aes_enc_key(key[type], 16, ctx + type);
}
void aeshash_init(void)
{
  aeshash_init_type(l_c);
  aeshash_init_type(r_c);
}
