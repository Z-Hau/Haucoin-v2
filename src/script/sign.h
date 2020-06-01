// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_SCRIPT_SIGN_H
#define BITCOIN_SCRIPT_SIGN_H

#include <script/interpreter.h>

class CKeyID;
class CKeyStore;
class CScript;
class CTransaction;

struct CMutableTransaction;

/** Virtual base class for signature creators. */
class BaseSignatureCreator {
protected:
     CKeyStore* keystore;

public:
    explicit BaseSignatureCreator( CKeyStore* keystoreIn) : keystore(keystoreIn) {}
     CKeyStore& KeyStore()  { return *keystore; };
    virtual ~BaseSignatureCreator() {}
    virtual  BaseSignatureChecker& Checker()  =0;

    /** Create a singular (non-script) signature. */
    virtual bool CreateSig(std::vector<unsigned char>& vchSig,  CKeyID& keyid,  CScript& scriptCode, SigVersion sigversion)  =0;
};

/** A signature creator for transactions. */
class TransactionSignatureCreator : public BaseSignatureCreator {
     CTransaction* txTo;
    unsigned int nIn;
    int nHashType;
    CAmount amount;
     TransactionSignatureChecker checker;

public:
    TransactionSignatureCreator( CKeyStore* keystoreIn,  CTransaction* txToIn, unsigned int nInIn,  CAmount& amountIn, int nHashTypeIn=SIGHASH_ALL);
     BaseSignatureChecker& Checker()  override { return checker; }
    bool CreateSig(std::vector<unsigned char>& vchSig,  CKeyID& keyid,  CScript& scriptCode, SigVersion sigversion)  override;
};

class MutableTransactionSignatureCreator : public TransactionSignatureCreator {
    CTransaction tx;

public:
    MutableTransactionSignatureCreator( CKeyStore* keystoreIn,  CMutableTransaction* txToIn, unsigned int nInIn,  CAmount& amountIn, int nHashTypeIn) : TransactionSignatureCreator(keystoreIn, &tx, nInIn, amountIn, nHashTypeIn), tx(*txToIn) {}
};

/** A signature creator that just produces 72-byte empty signatures. */
class DummySignatureCreator : public BaseSignatureCreator {
public:
    explicit DummySignatureCreator( CKeyStore* keystoreIn) : BaseSignatureCreator(keystoreIn) {}
     BaseSignatureChecker& Checker()  override;
    bool CreateSig(std::vector<unsigned char>& vchSig,  CKeyID& keyid,  CScript& scriptCode, SigVersion sigversion)  override;
};

struct SignatureData {
    CScript scriptSig;
    CScriptWitness scriptWitness;

    SignatureData() {}
    explicit SignatureData( CScript& script) : scriptSig(script) {}
};

/** Produce a script signature using a generic signature creator. */
bool ProduceSignature( BaseSignatureCreator& creator,  CScript& scriptPubKey, SignatureData& sigdata);

/** Produce a script signature for a transaction. */
bool SignSignature( CKeyStore &keystore,  CScript& fromPubKey, CMutableTransaction& txTo, unsigned int nIn,  CAmount& amount, int nHashType);
bool SignSignature( CKeyStore& keystore,  CTransaction& txFrom, CMutableTransaction& txTo, unsigned int nIn, int nHashType);

/** Combine two script signatures using a generic signature checker, intelligently, possibly with OP_0 placeholders. */
SignatureData CombineSignatures( CScript& scriptPubKey,  BaseSignatureChecker& checker,  SignatureData& scriptSig1,  SignatureData& scriptSig2);

/** Extract signature data from a transaction, and insert it. */
SignatureData DataFromTransaction( CMutableTransaction& tx, unsigned int nIn);
void UpdateTransaction(CMutableTransaction& tx, unsigned int nIn,  SignatureData& data);

/* Check whether we know how to sign for an output like this, assuming we
 * have all private keys. While this function does not need private keys, the passed
 * keystore is used to look up public keys and redeemscripts by hash.
 * Solvability is unrelated to whether we consider this output to be ours. */
bool IsSolvable( CKeyStore& store,  CScript& script);

#endif // BITCOIN_SCRIPT_SIGN_H
