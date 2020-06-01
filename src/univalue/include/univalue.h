// Copyright 2014 BitPay Inc.
// Copyright 2015 Bitcoin Core Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __UNIVALUE_H__
#define __UNIVALUE_H__

#include <stdint.h>
#include <string.h>

#include <string>
#include <vector>
#include <map>
#include <cassert>

#include <sstream>        // .get_int64()
#include <utility>        // std::pair

class UniValue {
public:
    enum VType { VNULL, VOBJ, VARR, VSTR, VNUM, VBOOL, };

    UniValue() { typ = VNULL; }
    UniValue(UniValue::VType initialType,  std::string& initialStr = "") {
        typ = initialType;
        val = initialStr;
    }
    UniValue(uint64_t val_) {
        setInt(val_);
    }
    UniValue(int64_t val_) {
        setInt(val_);
    }
    UniValue(bool val_) {
        setBool(val_);
    }
    UniValue(int val_) {
        setInt(val_);
    }
    UniValue(double val_) {
        setFloat(val_);
    }
    UniValue( std::string& val_) {
        setStr(val_);
    }
    UniValue( char *val_) {
        std::string s(val_);
        setStr(s);
    }
    ~UniValue() {}

    void clear();

    bool setNull();
    bool setBool(bool val);
    bool setNumStr( std::string& val);
    bool setInt(uint64_t val);
    bool setInt(int64_t val);
    bool setInt(int val_) { return setInt((int64_t)val_); }
    bool setFloat(double val);
    bool setStr( std::string& val);
    bool setArray();
    bool setObject();

    enum VType getType()  { return typ; }
     std::string& getValStr()  { return val; }
    bool empty()  { return (values.size() == 0); }

    size_t size()  { return values.size(); }

    bool getBool()  { return isTrue(); }
    void getObjMap(std::map<std::string,UniValue>& kv) ;
    bool checkObject( std::map<std::string,UniValue::VType>& memberTypes) ;
     UniValue& operator[]( std::string& key) ;
     UniValue& operator[](size_t index) ;
    bool exists( std::string& key)  { size_t i; return findKey(key, i); }

    bool isNull()  { return (typ == VNULL); }
    bool isTrue()  { return (typ == VBOOL) && (val == "1"); }
    bool isFalse()  { return (typ == VBOOL) && (val != "1"); }
    bool isBool()  { return (typ == VBOOL); }
    bool isStr()  { return (typ == VSTR); }
    bool isNum()  { return (typ == VNUM); }
    bool isArray()  { return (typ == VARR); }
    bool isObject()  { return (typ == VOBJ); }

    bool push_back( UniValue& val);
    bool push_back( std::string& val_) {
        UniValue tmpVal(VSTR, val_);
        return push_back(tmpVal);
    }
    bool push_back( char *val_) {
        std::string s(val_);
        return push_back(s);
    }
    bool push_back(uint64_t val_) {
        UniValue tmpVal(val_);
        return push_back(tmpVal);
    }
    bool push_back(int64_t val_) {
        UniValue tmpVal(val_);
        return push_back(tmpVal);
    }
    bool push_back(int val_) {
        UniValue tmpVal(val_);
        return push_back(tmpVal);
    }
    bool push_back(double val_) {
        UniValue tmpVal(val_);
        return push_back(tmpVal);
    }
    bool push_backV( std::vector<UniValue>& vec);

    void __pushKV( std::string& key,  UniValue& val);
    bool pushKV( std::string& key,  UniValue& val);
    bool pushKV( std::string& key,  std::string& val_) {
        UniValue tmpVal(VSTR, val_);
        return pushKV(key, tmpVal);
    }
    bool pushKV( std::string& key,  char *val_) {
        std::string _val(val_);
        return pushKV(key, _val);
    }
    bool pushKV( std::string& key, int64_t val_) {
        UniValue tmpVal(val_);
        return pushKV(key, tmpVal);
    }
    bool pushKV( std::string& key, uint64_t val_) {
        UniValue tmpVal(val_);
        return pushKV(key, tmpVal);
    }
    bool pushKV( std::string& key, int val_) {
        UniValue tmpVal((int64_t)val_);
        return pushKV(key, tmpVal);
    }
    bool pushKV( std::string& key, double val_) {
        UniValue tmpVal(val_);
        return pushKV(key, tmpVal);
    }
    bool pushKVs( UniValue& obj);

    std::string write(unsigned int prettyIndent = 0,
                      unsigned int indentLevel = 0) ;

    bool read( char *raw, size_t len);
    bool read( char *raw) { return read(raw, strlen(raw)); }
    bool read( std::string& rawStr) {
        return read(rawStr.data(), rawStr.size());
    }

private:
    UniValue::VType typ;
    std::string val;                       // numbers are stored as C++ strings
    std::vector<std::string> keys;
    std::vector<UniValue> values;

    bool findKey( std::string& key, size_t& retIdx) ;
    void writeArray(unsigned int prettyIndent, unsigned int indentLevel, std::string& s) ;
    void writeObject(unsigned int prettyIndent, unsigned int indentLevel, std::string& s) ;

public:
    // Strict type-specific getters, these throw std::runtime_error if the
    // value is of unexpected type
     std::vector<std::string>& getKeys() ;
     std::vector<UniValue>& getValues() ;
    bool get_bool() ;
     std::string& get_str() ;
    int get_int() ;
    int64_t get_int64() ;
    double get_real() ;
     UniValue& get_obj() ;
     UniValue& get_array() ;

    enum VType type()  { return getType(); }
    bool push_back(std::pair<std::string,UniValue> pear) {
        return pushKV(pear.first, pear.second);
    }
    friend  UniValue& find_value(  UniValue& obj,  std::string& name);
};

//
// The following were added for compatibility with json_spirit.
// Most duplicate other methods, and should be removed.
//
static inline std::pair<std::string,UniValue> Pair( char *cKey,  char *cVal)
{
    std::string key(cKey);
    UniValue uVal(cVal);
    return std::make_pair(key, uVal);
}

static inline std::pair<std::string,UniValue> Pair( char *cKey, std::string strVal)
{
    std::string key(cKey);
    UniValue uVal(strVal);
    return std::make_pair(key, uVal);
}

static inline std::pair<std::string,UniValue> Pair( char *cKey, uint64_t u64Val)
{
    std::string key(cKey);
    UniValue uVal(u64Val);
    return std::make_pair(key, uVal);
}

static inline std::pair<std::string,UniValue> Pair( char *cKey, int64_t i64Val)
{
    std::string key(cKey);
    UniValue uVal(i64Val);
    return std::make_pair(key, uVal);
}

static inline std::pair<std::string,UniValue> Pair( char *cKey, bool iVal)
{
    std::string key(cKey);
    UniValue uVal(iVal);
    return std::make_pair(key, uVal);
}

static inline std::pair<std::string,UniValue> Pair( char *cKey, int iVal)
{
    std::string key(cKey);
    UniValue uVal(iVal);
    return std::make_pair(key, uVal);
}

static inline std::pair<std::string,UniValue> Pair( char *cKey, double dVal)
{
    std::string key(cKey);
    UniValue uVal(dVal);
    return std::make_pair(key, uVal);
}

static inline std::pair<std::string,UniValue> Pair( char *cKey,  UniValue& uVal)
{
    std::string key(cKey);
    return std::make_pair(key, uVal);
}

static inline std::pair<std::string,UniValue> Pair(std::string key,  UniValue& uVal)
{
    return std::make_pair(key, uVal);
}

enum jtokentype {
    JTOK_ERR        = -1,
    JTOK_NONE       = 0,                           // eof
    JTOK_OBJ_OPEN,
    JTOK_OBJ_CLOSE,
    JTOK_ARR_OPEN,
    JTOK_ARR_CLOSE,
    JTOK_COLON,
    JTOK_COMMA,
    JTOK_KW_NULL,
    JTOK_KW_TRUE,
    JTOK_KW_FALSE,
    JTOK_NUMBER,
    JTOK_STRING,
};

extern enum jtokentype getJsonToken(std::string& tokenVal,
                                    unsigned int& consumed,  char *raw,  char *end);
extern  char *uvTypeName(UniValue::VType t);

static inline bool jsonTokenIsValue(enum jtokentype jtt)
{
    switch (jtt) {
    case JTOK_KW_NULL:
    case JTOK_KW_TRUE:
    case JTOK_KW_FALSE:
    case JTOK_NUMBER:
    case JTOK_STRING:
        return true;

    default:
        return false;
    }

    // not reached
}

static inline bool json_isspace(int ch)
{
    switch (ch) {
    case 0x20:
    case 0x09:
    case 0x0a:
    case 0x0d:
        return true;

    default:
        return false;
    }

    // not reached
}

extern  UniValue NullUniValue;

 UniValue& find_value(  UniValue& obj,  std::string& name);

#endif // __UNIVALUE_H__
