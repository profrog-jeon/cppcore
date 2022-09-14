#include "stdafx.h"
#include "SocketDefine.h"

#include <sys/types.h> 
#include <sys/socket.h> 
#include <net/if.h> 
#include <net/route.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netinet/in.h>

/*
* Types
*/
const int SOCK_STREAM_ = SOCK_STREAM     ;  /* stream socket */
const int SOCK_DGRAM_ = SOCK_DGRAM      ;  /* datagram socket */
const int SOCK_RAW_ = SOCK_RAW        ;  /* raw-protocol interface */
const int SOCK_RDM_ = SOCK_RDM        ;  /* reliably-delivered message */
const int SOCK_SEQPACKET_ = SOCK_SEQPACKET  ;  /* sequenced packet stream */

/*
* Option flags per-socket.
*/
const int SO_DEBUG_ = SO_DEBUG       ; /* turn on debugging info recording */
const int SO_ACCEPTCONN_ = SO_ACCEPTCONN  ; /* socket has had listen() */
const int SO_REUSEADDR_ = SO_REUSEADDR   ; /* allow local address reuse */
const int SO_KEEPALIVE_ = SO_KEEPALIVE   ; /* keep connections alive */
const int SO_DONTROUTE_ = SO_DONTROUTE   ; /* just use interface addresses */
const int SO_BROADCAST_ = SO_BROADCAST   ; /* permit sending of broadcast msgs */
//const int SO_USELOOPBACK_ = SO_USELOOPBACK ; /* bypass hardware when possible */
const int SO_LINGER_ = SO_LINGER      ; /* linger on close if data present */
const int SO_OOBINLINE_ = SO_OOBINLINE   ; /* leave received OOB data in line */
//const int SO_DONTLINGER_ = SO_DONTLINGER  ;

/*
* Additional options.
*/
const int SO_SNDBUF_ = SO_SNDBUF      ;          /* send buffer size */
const int SO_RCVBUF_ = SO_RCVBUF      ;          /* receive buffer size */
const int SO_SNDLOWAT_ = SO_SNDLOWAT    ;          /* send low-water mark */
const int SO_RCVLOWAT_ = SO_RCVLOWAT    ;          /* receive low-water mark */
const int SO_SNDTIMEO_ = SO_SNDTIMEO    ;          /* send timeout */
const int SO_RCVTIMEO_ = SO_RCVTIMEO    ;          /* receive timeout */
const int SO_ERROR_ = SO_ERROR       ;          /* get error status and clear */
const int SO_TYPE_ = SO_TYPE        ;          /* get socket type */
//
///*
//* Options for connect and disconnect data and options.  Used only by
//* non-TCP/IP transports such as DECNet, OSI TP4, etc.
//*/
//const int SO_CONNDATA_ = SO_CONNDATA      ;
//const int SO_CONNOPT_ = SO_CONNOPT	   ;
//const int SO_DISCDATA_ = SO_DISCDATA	   ;
//const int SO_DISCOPT_ = SO_DISCOPT	   ;
//const int SO_CONNDATALEN_ = SO_CONNDATALEN   ;
//const int SO_CONNOPTLEN_ = SO_CONNOPTLEN	   ;
//const int SO_DISCDATALEN_ = SO_DISCDATALEN   ;
//const int SO_DISCOPTLEN_ = SO_DISCOPTLEN	   ;
//
///*
//* Option for opening sockets for synchronous access.
//*/
//const int SO_OPENTYPE_ = SO_OPENTYPE                           ;
//const int SO_SYNCHRONOUS_ALERT_ = SO_SYNCHRONOUS_ALERT			;
//const int SO_SYNCHRONOUS_NONALERT_ = SO_SYNCHRONOUS_NONALERT		;
//
///*
//* Other NT-specific options.
//*/
//const int SO_MAXDG_ = SO_MAXDG                      ;
//const int SO_MAXPATHDG_ = SO_MAXPATHDG					 ;
//const int SO_UPDATE_ACCEPT_CONTEXT_ = SO_UPDATE_ACCEPT_CONTEXT		 ;
//const int SO_CONNECT_TIME_ = SO_CONNECT_TIME				 ;

/*
* TCP options.
*/
const int TCP_NODELAY_ = TCP_NODELAY;
//const int TCP_BSDURGENT_ = TCP_BSDURGENT;

/*
 * manifest constants for shutdown()
 */
const int SD_RECEIVE_ = SHUT_RD;
const int SD_SEND_ = SHUT_WR;
const int SD_BOTH_ = SHUT_RDWR;

/*
* Maximum queue length specifiable by listen.
*/
const int SOMAXCONN_ = 5;

const int MSG_OOB_ = MSG_OOB;              /* process out-of-band data */
const int MSG_PEEK_ = MSG_PEEK;            /* peek at incoming message */
const int MSG_DONTROUTE_ = MSG_DONTROUTE;         /* send without using routing tables */
const int MSG_WAITALL_ = MSG_WAITALL;		/* do not complete until packet is completely filled */
//const int MSG_MAXIOVLEN_ = MSG_MAXIOVLEN;
//const int MSG_PARTIAL_ = MSG_PARTIAL;        /* partial send or recv for message xport */

/*
* Protocols
*/
const int IPPROTO_IP_ = IPPROTO_IP;    /* dummy for IP */
const int IPPROTO_ICMP_ = IPPROTO_ICMP;  /* control message protocol */
const int IPPROTO_IGMP_ = IPPROTO_IGMP;  /* group management protocol */
//const int IPPROTO_GGP_ = IPPROTO_GGP;   /* gateway^2 (deprecated) */
const int IPPROTO_TCP_ = IPPROTO_TCP;   /* tcp */
const int IPPROTO_PUP_ = IPPROTO_PUP;   /* pup */
const int IPPROTO_UDP_ = IPPROTO_UDP;   /* user datagram protocol */
const int IPPROTO_IDP_ = IPPROTO_IDP;   /* xns idp */
//const int IPPROTO_ND_ = IPPROTO_ND;    /* UNOFFICIAL net disk proto */
const int IPPROTO_RAW_ = IPPROTO_RAW;   /* raw IP packet */
const int IPPROTO_MAX_ = IPPROTO_MAX;

//
// Although AF_UNSPEC is defined for backwards compatibility, using
// AF_UNSPEC for the "af" parameter when creating a socket is STRONGLY
// DISCOURAGED.  The interpretation of the "protocol" parameter
// depends on the actual address family chosen.  As environments grow
// to include more and more address families that use overlapping
// protocol values there is more and more chance of choosing an
// undesired address family when AF_UNSPEC is used.
//
const int AF_UNSPEC_ = AF_UNSPEC;       // unspecified
const int AF_UNIX_ = AF_UNIX;         // local to host (pipes, portals)
const int AF_INET_ = AF_INET;         // internetwork: UDP, TCP, etc.
//const int AF_IMPLINK_ = AF_IMPLINK;      // arpanet imp addresses
//const int AF_PUP_ = AF_PUP;          // pup protocols: e.g. BSP
//const int AF_CHAOS_ = AF_CHAOS;        // mit CHAOS protocols
//const int AF_NS_ = AF_NS;           // XEROX NS protocols
//const int AF_IPX_ = AF_IPX;          // IPX protocols: IPX, SPX, etc.
//const int AF_ISO_ = AF_ISO;          // ISO protocols
//const int AF_OSI_ = AF_OSI;          // OSI is ISO
//const int AF_ECMA_ = AF_ECMA;         // european computer manufacturers
//const int AF_DATAKIT_ = AF_DATAKIT;      // datakit protocols
//const int AF_CCITT_ = AF_CCITT;        // CCITT protocols, X.25 etc
const int AF_SNA_ = AF_SNA;          // IBM SNA
const int AF_DECnet_ = AF_DECnet;       // DECnet
//const int AF_DLI_ = AF_DLI;          // Direct data link interface
//const int AF_LAT_ = AF_LAT;          // LAT
//const int AF_HYLINK_ = AF_HYLINK;       // NSC Hyperchannel
const int AF_APPLETALK_ = AF_APPLETALK;    // AppleTalk
//const int AF_NETBIOS_ = AF_NETBIOS;      // NetBios-style addresses
//const int AF_VOICEVIEW_ = AF_VOICEVIEW;    // VoiceView
//const int AF_FIREFOX_ = AF_FIREFOX;      // Protocols from Firefox
//const int AF_UNKNOWN1_ = AF_UNKNOWN1;     // Somebody is using this!
//const int AF_BAN_ = AF_BAN;          // Banyan
//const int AF_ATM_ = AF_ATM;          // Native ATM Services
const int AF_INET6_ = AF_INET6;        // Internetwork Version 6
//const int AF_CLUSTER_ = AF_CLUSTER;      // Microsoft Wolfpack
//const int AF_12844_ = AF_12844;        // IEEE 1284.4 WG AF
//const int AF_IRDA_ = AF_IRDA;         // IrDA
//const int AF_NETDES_ = AF_NETDES;       // Network Designers OSI & gateway

/*
 * Protocol families, same as address families for now.
*/
const int PF_UNSPEC_ = PF_UNSPEC;
const int PF_UNIX_ = PF_UNIX;
const int PF_INET_ = PF_INET;
//const int PF_IMPLINK_ = PF_IMPLINK;
//const int PF_PUP_ = PF_PUP;
//const int PF_CHAOS_ = PF_CHAOS;
//const int PF_NS_ = PF_NS;
//const int PF_IPX_ = PF_IPX;
//const int PF_ISO_ = PF_ISO;
//const int PF_OSI_ = PF_OSI;
//const int PF_ECMA_ = PF_ECMA;
//const int PF_DATAKIT_ = PF_DATAKIT;
//const int PF_CCITT_ = PF_CCITT;
const int PF_SNA_ = PF_SNA;
const int PF_DECnet_ = PF_DECnet;
//const int PF_DLI_ = PF_DLI;
//const int PF_LAT_ = PF_LAT;
//const int PF_HYLINK_ = PF_HYLINK;
const int PF_APPLETALK_ = PF_APPLETALK;
//const int PF_VOICEVIEW_ = PF_VOICEVIEW;
//const int PF_FIREFOX_ = PF_FIREFOX;
//const int PF_UNKNOWN1_ = PF_UNKNOWN1;
//const int PF_BAN_ = PF_BAN;
//const int PF_MAX_ = PF_MAX;

/*
* Definitions of bits in internet address integers.
* On subnets, the decomposition of addresses to host and net parts
* is done according to subnet mask, not the masks here.
*
* N.B. RFC-compliant definitions for host-order elements are named IN_xxx,
* while network-order elements are named IN4_xxx.
*/
const int IN_CLASSA_NET_ = IN_CLASSA_NET;
const int IN_CLASSA_NSHIFT_ = IN_CLASSA_NSHIFT;
const int IN_CLASSA_HOST_ = IN_CLASSA_HOST;
const int IN_CLASSA_MAX_ = IN_CLASSA_MAX;

const int IN_CLASSB_NET_ = IN_CLASSB_NET;
const int IN_CLASSB_NSHIFT_ = IN_CLASSB_NSHIFT;
const int IN_CLASSB_HOST_ = IN_CLASSB_HOST;
const int IN_CLASSB_MAX_ = IN_CLASSB_MAX;

const int IN_CLASSC_NET_ = IN_CLASSC_NET;
const int IN_CLASSC_NSHIFT_ = IN_CLASSC_NSHIFT;
const int IN_CLASSC_HOST_ = IN_CLASSC_HOST;
//
//const int IN_CLASSD_NET_ = IN_CLASSD_NET;   /* These ones aren't really */
//const int IN_CLASSD_NSHIFT_ = IN_CLASSD_NSHIFT;/* net and host fields, but */
//const int IN_CLASSD_HOST_ = IN_CLASSD_HOST;  /* routing needn't know.    */

const int INADDR_ANY_ = INADDR_ANY;
const int INADDR_LOOPBACK_ = INADDR_LOOPBACK;
const int INADDR_BROADCAST_ = INADDR_BROADCAST;
const int INADDR_NONE_ = INADDR_NONE;

/*
* Level number for (get/set)sockopt() to apply to socket itself.
*/
const int SOL_SOCKET_ = SOL_SOCKET;          /* options for socket level */
