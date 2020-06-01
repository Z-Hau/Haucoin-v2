// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_KEYSTORE_H
#define BITCOIN_KEYSTORE_H

#include <key.h>
#include <pubkey.h>
#include <script/script.h>
#include <script/standard.h>
#include <sync.h>

#include <boost/signals2/signal.hpp>

/** A virtual base class for key stores */
class CKeyStore
{
protected:
    mutable CCriticalSection cs_KeyStore;

public:
    virtual ~CKeyStore() {}

    //! Add a key to the store.
    virtual bool AddKeyPubKey( CKey &key,  CPubKey &pubkey) =0;
    virtual bool AddKey( CKey &key);

    //! Check whether a key corresponding to a given address is present in the store.
    virtual bool HaveKey( CKeyID &address)  =0;
    virtual bool GetKey( CKeyID &address, CKey& keyOut)  =0;
    virtual std::set<CKeyID> GetKeys()  =0;
    virtual bool GetPubKey( CKeyID &address, CPubKey& vchPubKeyOut)  =0;

    //! Support for BIP 0013 : see https://github.com/bitcoin/bips/blob/master/bip-0013.mediawiki
    virtual bool AddCScript( CScript& redeemScript) =0;
    virtual bool HaveCScript( CScriptID &hash)  =0;
    virtual std::set<CScriptID> GetCScripts()  =0;
    virtual bool GetCScript( CScriptID &hash, CScript& redeemScriptOut)  =0;

    //! Support for Watch-only addresses
    virtual bool AddWatchOnly( CScript &dest) =0;
    virtual bool RemoveWatchOnly( CScript &dest) =0;
    virtual bool HaveWatchOnly( CScript &dest)  =0;
    virtual bool HaveWatchOnly()  =0;
};

typedef std::map<CKeyID, CKey> KeyMap;
typedef std::map<CKeyID, CPubKey> WatchKeyMap;
typedef std::map<CScriptID, CScript > ScriptMap;
typedef std::set<CScript> WatchOnlySet;

/** Basic key store, that keeps keys in an address->secret map */
class CBasicKeyStore : public CKeyStore
{
protected:
    KeyMap mapKeys;
    WatchKeyMap mapWatchKeys;
    ScriptMap mapScripts;
    WatchOnlySet setWatchOnly;

    void ImplicitlyLearnRelatedKeyScripts( CPubKey& pubkey);

public:
    bool AddKeyPubKey( CKey& key,  CPubKey &pubkey) override;
    bool GetPubKey( CKeyID &address, CPubKey& vchPubKeyOut)  override;
    bool HaveKey( CKeyID &address)  override;
    std::set<CKeyID> GetKeys()  override;
    bool GetKey( CKeyID &address, CKey &keyOut)  override;
    bool AddCScript( CScript& redeemScript) override;
    bool HaveCScript( CScriptID &hash)  override;
    std::set<CScriptID> GetCScripts()  override;
    bool GetCScript( CScriptID &hash, CScript& redeemScriptOut)  override;

    bool AddWatchOnly( CScript &dest) override;
    bool RemoveWatchOnly( CScript &dest) override;
    bool HaveWatchOnly( CScript &dest)  override;
    bool HaveWatchOnly()  override;
};

typedef std::vector<unsigned char, secure_allocator<unsigned char> > CKeyingMaterial;
typedef std::map<CKeyID, std::pair<CPubKey, std::vector<unsigned char> > > CryptedKeyMap;

/** Return the CKeyID of the key involved in a script (if there is a unique one). */
CKeyID GetKeyForDestination( CKeyStore& store,  CTxDestination& dest);

#endif // BITCOIN_KEYSTORE_H
