// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_NETADDRESS_H
#define BITCOIN_NETADDRESS_H

#if defined(HAVE_CONFIG_H)
#include <config/bitcoin-config.h>
#endif

#include <compat.h>
#include <serialize.h>

#include <stdint.h>
#include <string>
#include <vector>

enum Network
{
    NET_UNROUTABLE = 0,
    NET_IPV4,
    NET_IPV6,
    NET_TOR,
    NET_INTERNAL,

    NET_MAX,
};

/** IP address (IPv6, or IPv4 using mapped IPv6 range (::FFFF:0:0/96)) */
class CNetAddr
{
    protected:
        unsigned char ip[16]; // in network byte order
        uint32_t scopeId; // for scoped/link-local ipv6 addresses

    public:
        CNetAddr();
        explicit CNetAddr( struct in_addr& ipv4Addr);
        void Init();
        void SetIP( CNetAddr& ip);

        /**
         * Set raw IPv4 or IPv6 address (in network byte order)
         * @note Only NET_IPV4 and NET_IPV6 are allowed for network.
         */
        void SetRaw(Network network,  uint8_t *data);

        /**
          * Transform an arbitrary string into a non-routable ipv6 address.
          * Useful for mapping resolved addresses back to their source.
         */
        bool SetInternal( std::string& name);

        bool SetSpecial( std::string &strName); // for Tor addresses
        bool IsIPv4() ;    // IPv4 mapped address (::FFFF:0:0/96, 0.0.0.0/0)
        bool IsIPv6() ;    // IPv6 address (not mapped IPv4, not Tor)
        bool IsRFC1918() ; // IPv4 private networks (10.0.0.0/8, 192.168.0.0/16, 172.16.0.0/12)
        bool IsRFC2544() ; // IPv4 inter-network communications (192.18.0.0/15)
        bool IsRFC6598() ; // IPv4 ISP-level NAT (100.64.0.0/10)
        bool IsRFC5737() ; // IPv4 documentation addresses (192.0.2.0/24, 198.51.100.0/24, 203.0.113.0/24)
        bool IsRFC3849() ; // IPv6 documentation address (2001:0DB8::/32)
        bool IsRFC3927() ; // IPv4 autoconfig (169.254.0.0/16)
        bool IsRFC3964() ; // IPv6 6to4 tunnelling (2002::/16)
        bool IsRFC4193() ; // IPv6 unique local (FC00::/7)
        bool IsRFC4380() ; // IPv6 Teredo tunnelling (2001::/32)
        bool IsRFC4843() ; // IPv6 ORCHID (2001:10::/28)
        bool IsRFC4862() ; // IPv6 autoconfig (FE80::/64)
        bool IsRFC6052() ; // IPv6 well-known prefix (64:FF9B::/96)
        bool IsRFC6145() ; // IPv6 IPv4-translated address (::FFFF:0:0:0/96)
        bool IsTor() ;
        bool IsLocal() ;
        bool IsRoutable() ;
        bool IsInternal() ;
        bool IsValid() ;
        enum Network GetNetwork() ;
        std::string ToString() ;
        std::string ToStringIP() ;
        unsigned int GetByte(int n) ;
        uint64_t GetHash() ;
        bool GetInAddr(struct in_addr* pipv4Addr) ;
        std::vector<unsigned char> GetGroup() ;
        int GetReachabilityFrom( CNetAddr *paddrPartner = nullptr) ;

        explicit CNetAddr( struct in6_addr& pipv6Addr,  uint32_t scope = 0);
        bool GetIn6Addr(struct in6_addr* pipv6Addr) ;

        friend bool operator==( CNetAddr& a,  CNetAddr& b);
        friend bool operator!=( CNetAddr& a,  CNetAddr& b);
        friend bool operator<( CNetAddr& a,  CNetAddr& b);

        ADD_SERIALIZE_METHODS;

        template <typename Stream, typename Operation>
        inline void SerializationOp(Stream& s, Operation ser_action) {
            READWRITE(FLATDATA(ip));
        }

        friend class CSubNet;
};

class CSubNet
{
    protected:
        /// Network (base) address
        CNetAddr network;
        /// Netmask, in network byte order
        uint8_t netmask[16];
        /// Is this value valid? (only used to signal parse errors)
        bool valid;

    public:
        CSubNet();
        CSubNet( CNetAddr &addr, int32_t mask);
        CSubNet( CNetAddr &addr,  CNetAddr &mask);

        //constructor for single ip subnet (<ipv4>/32 or <ipv6>/128)
        explicit CSubNet( CNetAddr &addr);

        bool Match( CNetAddr &addr) ;

        std::string ToString() ;
        bool IsValid() ;

        friend bool operator==( CSubNet& a,  CSubNet& b);
        friend bool operator!=( CSubNet& a,  CSubNet& b);
        friend bool operator<( CSubNet& a,  CSubNet& b);

        ADD_SERIALIZE_METHODS;

        template <typename Stream, typename Operation>
        inline void SerializationOp(Stream& s, Operation ser_action) {
            READWRITE(network);
            READWRITE(FLATDATA(netmask));
            READWRITE(FLATDATA(valid));
        }
};

/** A combination of a network address (CNetAddr) and a (TCP) port */
class CService : public CNetAddr
{
    protected:
        unsigned short port; // host order

    public:
        CService();
        CService( CNetAddr& ip, unsigned short port);
        CService( struct in_addr& ipv4Addr, unsigned short port);
        explicit CService( struct sockaddr_in& addr);
        void Init();
        unsigned short GetPort() ;
        bool GetSockAddr(struct sockaddr* paddr, socklen_t *addrlen) ;
        bool SetSockAddr( struct sockaddr* paddr);
        friend bool operator==( CService& a,  CService& b);
        friend bool operator!=( CService& a,  CService& b);
        friend bool operator<( CService& a,  CService& b);
        std::vector<unsigned char> GetKey() ;
        std::string ToString() ;
        std::string ToStringPort() ;
        std::string ToStringIPPort() ;

        CService( struct in6_addr& ipv6Addr, unsigned short port);
        explicit CService( struct sockaddr_in6& addr);

        ADD_SERIALIZE_METHODS;

        template <typename Stream, typename Operation>
        inline void SerializationOp(Stream& s, Operation ser_action) {
            READWRITE(FLATDATA(ip));
            unsigned short portN = htons(port);
            READWRITE(FLATDATA(portN));
            if (ser_action.ForRead())
                 port = ntohs(portN);
        }
};

#endif // BITCOIN_NETADDRESS_H
