#pragma once

#ifndef IN_CLASSA
#define IN_CLASSA(i)            (((LONG)(i) & 0x80000000) == 0)
#endif

#ifndef IN_CLASSB
#define IN_CLASSB(i)            (((LONG)(i) & 0xc0000000) == 0x80000000)
#endif

#ifndef IN_CLASSC
#define IN_CLASSC(i)            (((LONG)(i) & 0xe0000000) == 0xc0000000)
#endif

#ifndef IN_CLASSD
#define IN_CLASSD(i)            (((long)(i) & 0xf0000000) == 0xe0000000)
#endif

#ifndef IN_MULTICAST
#define IN_MULTICAST(i)         IN_CLASSD(i)
#endif

typedef size_t SOCKET;

extern const int INVALID_SOCKET_;
extern const int SOCKET_ERROR_;

/*
* Types
*/
extern const int SOCK_STREAM_;  /* stream socket */
extern const int SOCK_DGRAM_;  /* datagram socket */
extern const int SOCK_RAW_;  /* raw-protocol interface */
extern const int SOCK_RDM_;  /* reliably-delivered message */
extern const int SOCK_SEQPACKET_;  /* sequenced packet stream */

/*
* Option flags per-socket.
*/
extern const int SO_DEBUG_; /* turn on debugging info recording */
extern const int SO_ACCEPTCONN_; /* socket has had listen() */
extern const int SO_REUSEADDR_; /* allow local address reuse */
extern const int SO_KEEPALIVE_; /* keep connections alive */
extern const int SO_DONTROUTE_; /* just use interface addresses */
extern const int SO_BROADCAST_; /* permit sending of broadcast msgs */
extern const int SO_USELOOPBACK_; /* bypass hardware when possible */
extern const int SO_LINGER_; /* linger on close if data present */
extern const int SO_OOBINLINE_; /* leave received OOB data in line */
extern const int SO_DONTLINGER_;

/*
* Additional options.
*/
extern const int SO_SNDBUF_;          /* send buffer size */
extern const int SO_RCVBUF_;          /* receive buffer size */
extern const int SO_SNDLOWAT_;          /* send low-water mark */
extern const int SO_RCVLOWAT_;          /* receive low-water mark */
extern const int SO_SNDTIMEO_;          /* send timeout */
extern const int SO_RCVTIMEO_;          /* receive timeout */
extern const int SO_ERROR_;          /* get error status and clear */
extern const int SO_TYPE_;          /* get socket type */

/*
* Options for connect and disconnect data and options.  Used only by
* non-TCP/IP transports such as DECNet, OSI TP4, etc.
*/
extern const int SO_CONNDATA_;
extern const int SO_CONNOPT_;
extern const int SO_DISCDATA_;
extern const int SO_DISCOPT_;
extern const int SO_CONNDATALEN_;
extern const int SO_CONNOPTLEN_;
extern const int SO_DISCDATALEN_;
extern const int SO_DISCOPTLEN_;

/*
* Option for opening sockets for synchronous access.
*/
extern const int SO_OPENTYPE_;
extern const int SO_SYNCHRONOUS_ALERT_;
extern const int SO_SYNCHRONOUS_NONALERT_;

/*
* Other NT-specific options.
*/
extern const int SO_MAXDG_;
extern const int SO_MAXPATHDG_;
extern const int SO_UPDATE_ACCEPT_CONTEXT_;
extern const int SO_CONNECT_TIME_;

/*
* TCP options.
*/
extern const int TCP_NODELAY_;
extern const int TCP_BSDURGENT_;

/*
* manifest constants for shutdown()
*/
extern const int SD_RECEIVE_;
extern const int SD_SEND_;
extern const int SD_BOTH_;

/*
* Maximum queue length specifiable by listen.
*/
extern const int SOMAXCONN_;

extern const int MSG_OOB_;              /* process out-of-band data */
extern const int MSG_PEEK_;            /* peek at incoming message */
extern const int MSG_DONTROUTE_;         /* send without using routing tables */
extern const int MSG_WAITALL_;         /* send without using routing tables */
extern const int MSG_MAXIOVLEN_;
extern const int MSG_PARTIAL_;        /* partial send or recv for message xport */

/*
* Protocols
*/
extern const int IPPROTO_IP_;    /* dummy for IP */
extern const int IPPROTO_ICMP_;  /* control message protocol */
extern const int IPPROTO_IGMP_;  /* group management protocol */
extern const int IPPROTO_GGP_;   /* gateway^2 (deprecated) */
extern const int IPPROTO_TCP_;   /* tcp */
extern const int IPPROTO_PUP_;   /* pup */
extern const int IPPROTO_UDP_;   /* user datagram protocol */
extern const int IPPROTO_IDP_;   /* xns idp */
extern const int IPPROTO_ND_;    /* UNOFFICIAL net disk proto */
extern const int IPPROTO_RAW_;   /* raw IP packet */
extern const int IPPROTO_MAX_;

//
// Although AF_UNSPEC is defined for backwards compatibility, using
// AF_UNSPEC for the "af" parameter when creating a socket is STRONGLY
// DISCOURAGED.  The interpretation of the "protocol" parameter
// depends on the actual address family chosen.  As environments grow
// to include more and more address families that use overlapping
// protocol values there is more and more chance of choosing an
// undesired address family when AF_UNSPEC is used.
//
extern const int AF_UNSPEC_;       // unspecified
extern const int AF_UNIX_;         // local to host (pipes, portals)
extern const int AF_INET_;         // internetwork: UDP, TCP, etc.
extern const int AF_IMPLINK_;      // arpanet imp addresses
extern const int AF_PUP_;          // pup protocols: e.g. BSP
extern const int AF_CHAOS_;        // mit CHAOS protocols
extern const int AF_NS_;           // XEROX NS protocols
extern const int AF_IPX_;          // IPX protocols: IPX, SPX, etc.
extern const int AF_ISO_;          // ISO protocols
extern const int AF_OSI_;          // OSI is ISO
extern const int AF_ECMA_;         // european computer manufacturers
extern const int AF_DATAKIT_;      // datakit protocols
extern const int AF_CCITT_;        // CCITT protocols, X.25 etc
extern const int AF_SNA_;          // IBM SNA
extern const int AF_DECnet_;       // DECnet
extern const int AF_DLI_;          // Direct data link interface
extern const int AF_LAT_;          // LAT
extern const int AF_HYLINK_;       // NSC Hyperchannel
extern const int AF_APPLETALK_;    // AppleTalk
extern const int AF_NETBIOS_;      // NetBios-style addresses
extern const int AF_VOICEVIEW_;    // VoiceView
extern const int AF_FIREFOX_;      // Protocols from Firefox
extern const int AF_UNKNOWN1_;     // Somebody is using this!
extern const int AF_BAN_;          // Banyan
extern const int AF_ATM_;          // Native ATM Services
extern const int AF_INET6_;        // Internetwork Version 6
extern const int AF_CLUSTER_;      // Microsoft Wolfpack
extern const int AF_12844_;        // IEEE 1284.4 WG AF
extern const int AF_IRDA_;         // IrDA
extern const int AF_NETDES_;       // Network Designers OSI & gateway

/*
* Protocol families, same as address families for now.
*/
extern const int PF_UNSPEC_;
extern const int PF_UNIX_;
extern const int PF_INET_;
extern const int PF_IMPLINK_;
extern const int PF_PUP_;
extern const int PF_CHAOS_;
extern const int PF_NS_;
extern const int PF_IPX_;
extern const int PF_ISO_;
extern const int PF_OSI_;
extern const int PF_ECMA_;
extern const int PF_DATAKIT_;
extern const int PF_CCITT_;
extern const int PF_SNA_;
extern const int PF_DECnet_;
extern const int PF_DLI_;
extern const int PF_LAT_;
extern const int PF_HYLINK_;
extern const int PF_APPLETALK_;
extern const int PF_VOICEVIEW_;
extern const int PF_FIREFOX_;
extern const int PF_UNKNOWN1_;
extern const int PF_BAN_;
extern const int PF_MAX_;

/*
* Types
*/
extern const int SOCK_STREAM_;        /* stream socket */
extern const int SOCK_DGRAM_;         /* datagram socket */
extern const int SOCK_RAW_;           /* raw-protocol interface */
extern const int SOCK_RDM_;           /* reliably-delivered message */
extern const int SOCK_SEQPACKET_;     /* sequenced packet stream */

/*
* Definitions of bits in internet address integers.
* On subnets, the decomposition of addresses to host and net parts
* is done according to subnet mask, not the masks here.
*
* N.B. RFC-compliant definitions for host-order elements are named IN_xxx,
* while network-order elements are named IN4_xxx.
*/
extern const int IN_CLASSA_NET_;
extern const int IN_CLASSA_NSHIFT_;
extern const int IN_CLASSA_HOST_;
extern const int IN_CLASSA_MAX_;

extern const int IN_CLASSB_NET_;
extern const int IN_CLASSB_NSHIFT_;
extern const int IN_CLASSB_HOST_;
extern const int IN_CLASSB_MAX_;

extern const int IN_CLASSC_NET_;
extern const int IN_CLASSC_NSHIFT_;
extern const int IN_CLASSC_HOST_;
extern const int IN_CLASSD_NET_;   /* These ones aren't really */
extern const int IN_CLASSD_NSHIFT_;/* net and host fields, but */
extern const int IN_CLASSD_HOST_;  /* routing needn't know.    */
extern const int IN_MULTICAST_;

extern const int INADDR_ANY_;
extern const int INADDR_LOOPBACK_;
extern const int INADDR_BROADCAST_;
extern const int INADDR_NONE_;

/*
* Level number for (get/set)sockopt() to apply to socket itself.
*/
extern const int SOL_SOCKET_;          /* options for socket level */
