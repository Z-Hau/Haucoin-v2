 /*********************************************************************
 * Copyright (c) 2016 Pieter Wuille                                   *
 * Distributed under the MIT software license, see the accompanying   *
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.*
 **********************************************************************/

#ifndef _CTAES_H_
#define _CTAES_H_ 1

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint16_t slice[8];
} AES_state;

typedef struct {
    AES_state rk[11];
} AES128_ctx;

typedef struct {
    AES_state rk[13];
} AES192_ctx;

typedef struct {
    AES_state rk[15];
} AES256_ctx;

void AES128_init(AES128_ctx* ctx,  unsigned char* key16);
void AES128_encrypt( AES128_ctx* ctx, size_t blocks, unsigned char* cipher16,  unsigned char* plain16);
void AES128_decrypt( AES128_ctx* ctx, size_t blocks, unsigned char* plain16,  unsigned char* cipher16);

void AES192_init(AES192_ctx* ctx,  unsigned char* key24);
void AES192_encrypt( AES192_ctx* ctx, size_t blocks, unsigned char* cipher16,  unsigned char* plain16);
void AES192_decrypt( AES192_ctx* ctx, size_t blocks, unsigned char* plain16,  unsigned char* cipher16);

void AES256_init(AES256_ctx* ctx,  unsigned char* key32);
void AES256_encrypt( AES256_ctx* ctx, size_t blocks, unsigned char* cipher16,  unsigned char* plain16);
void AES256_decrypt( AES256_ctx* ctx, size_t blocks, unsigned char* plain16,  unsigned char* cipher16);

#endif
