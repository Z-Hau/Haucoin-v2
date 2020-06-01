// Copyright (c) 2015-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
//
// C++ wrapper around ctaes, a constant-time AES implementation

#ifndef BITCOIN_CRYPTO_AES_H
#define BITCOIN_CRYPTO_AES_H

extern "C" {
#include <crypto/ctaes/ctaes.h>
}

static  int AES_BLOCKSIZE = 16;
static  int AES128_KEYSIZE = 16;
static  int AES256_KEYSIZE = 32;

/** An encryption class for AES-128. */
class AES128Encrypt
{
private:
    AES128_ctx ctx;

public:
    explicit AES128Encrypt( unsigned char key[16]);
    ~AES128Encrypt();
    void Encrypt(unsigned char ciphertext[16],  unsigned char plaintext[16]) ;
};

/** A decryption class for AES-128. */
class AES128Decrypt
{
private:
    AES128_ctx ctx;

public:
    explicit AES128Decrypt( unsigned char key[16]);
    ~AES128Decrypt();
    void Decrypt(unsigned char plaintext[16],  unsigned char ciphertext[16]) ;
};

/** An encryption class for AES-256. */
class AES256Encrypt
{
private:
    AES256_ctx ctx;

public:
    explicit AES256Encrypt( unsigned char key[32]);
    ~AES256Encrypt();
    void Encrypt(unsigned char ciphertext[16],  unsigned char plaintext[16]) ;
};

/** A decryption class for AES-256. */
class AES256Decrypt
{
private:
    AES256_ctx ctx;

public:
    explicit AES256Decrypt( unsigned char key[32]);
    ~AES256Decrypt();
    void Decrypt(unsigned char plaintext[16],  unsigned char ciphertext[16]) ;
};

class AES256CBCEncrypt
{
public:
    AES256CBCEncrypt( unsigned char key[AES256_KEYSIZE],  unsigned char ivIn[AES_BLOCKSIZE], bool padIn);
    ~AES256CBCEncrypt();
    int Encrypt( unsigned char* data, int size, unsigned char* out) ;

private:
     AES256Encrypt enc;
     bool pad;
    unsigned char iv[AES_BLOCKSIZE];
};

class AES256CBCDecrypt
{
public:
    AES256CBCDecrypt( unsigned char key[AES256_KEYSIZE],  unsigned char ivIn[AES_BLOCKSIZE], bool padIn);
    ~AES256CBCDecrypt();
    int Decrypt( unsigned char* data, int size, unsigned char* out) ;

private:
     AES256Decrypt dec;
     bool pad;
    unsigned char iv[AES_BLOCKSIZE];
};

class AES128CBCEncrypt
{
public:
    AES128CBCEncrypt( unsigned char key[AES128_KEYSIZE],  unsigned char ivIn[AES_BLOCKSIZE], bool padIn);
    ~AES128CBCEncrypt();
    int Encrypt( unsigned char* data, int size, unsigned char* out) ;

private:
     AES128Encrypt enc;
     bool pad;
    unsigned char iv[AES_BLOCKSIZE];
};

class AES128CBCDecrypt
{
public:
    AES128CBCDecrypt( unsigned char key[AES128_KEYSIZE],  unsigned char ivIn[AES_BLOCKSIZE], bool padIn);
    ~AES128CBCDecrypt();
    int Decrypt( unsigned char* data, int size, unsigned char* out) ;

private:
     AES128Decrypt dec;
     bool pad;
    unsigned char iv[AES_BLOCKSIZE];
};

#endif // BITCOIN_CRYPTO_AES_H
