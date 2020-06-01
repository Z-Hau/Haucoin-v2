// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CORE_IO_H
#define BITCOIN_CORE_IO_H

#include <amount.h>

#include <string>
#include <vector>

class CBlock;
class CScript;
class CTransaction;
struct CMutableTransaction;
class uint256;
class UniValue;

// core_read.cpp
CScript ParseScript( std::string& s);
std::string ScriptToAsmStr( CScript& script,  bool fAttemptSighashDecode = false);
bool DecodeHexTx(CMutableTransaction& tx,  std::string& hex_tx, bool try_no_witness = false, bool try_witness = true);
bool DecodeHexBlk(CBlock&,  std::string& strHexBlk);
uint256 ParseHashUV( UniValue& v,  std::string& strName);
uint256 ParseHashStr( std::string&,  std::string& strName);
std::vector<unsigned char> ParseHexUV( UniValue& v,  std::string& strName);

// core_write.cpp
UniValue ValueFromAmount( CAmount& amount);
std::string FormatScript( CScript& script);
std::string EncodeHexTx( CTransaction& tx,  int serializeFlags = 0);
void ScriptPubKeyToUniv( CScript& scriptPubKey, UniValue& out, bool fIncludeHex);
void TxToUniv( CTransaction& tx,  uint256& hashBlock, UniValue& entry, bool include_hex = true, int serialize_flags = 0);

#endif // BITCOIN_CORE_IO_H
