/**********************************************************************
 * Copyright (c) 2013, 2014 Pieter Wuille                             *
 * Distributed under the MIT software license, see the accompanying   *
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.*
 **********************************************************************/

#ifndef SECP256K1_ECDSA_H
#define SECP256K1_ECDSA_H

#include <stddef.h>

#include "scalar.h"
#include "group.h"
#include "ecmult.h"

static int secp256k1_ecdsa_sig_parse(secp256k1_scalar *r, secp256k1_scalar *s,  unsigned char *sig, size_t size);
static int secp256k1_ecdsa_sig_serialize(unsigned char *sig, size_t *size,  secp256k1_scalar *r,  secp256k1_scalar *s);
static int secp256k1_ecdsa_sig_verify( secp256k1_ecmult_context *ctx,  secp256k1_scalar* r,  secp256k1_scalar* s,  secp256k1_ge *pubkey,  secp256k1_scalar *message);
static int secp256k1_ecdsa_sig_sign( secp256k1_ecmult_gen_context *ctx, secp256k1_scalar* r, secp256k1_scalar* s,  secp256k1_scalar *seckey,  secp256k1_scalar *message,  secp256k1_scalar *nonce, int *recid);

#endif /* SECP256K1_ECDSA_H */
