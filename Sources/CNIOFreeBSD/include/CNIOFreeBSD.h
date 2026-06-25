//===----------------------------------------------------------------------===//
//
// This source file is part of the SwiftNIO open source project
//
// Copyright (c) 2017-2025 Apple Inc. and the SwiftNIO project authors
// Licensed under Apache License v2.0
//
// See LICENSE.txt for license information
// See CONTRIBUTORS.txt for the list of SwiftNIO project authors
//
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#ifndef C_NIO_FREEBSD_H
#define C_NIO_FREEBSD_H

#if defined(__FreeBSD__)
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/un.h>
#include <sys/utsname.h>
#include <dirent.h>
#include <fts.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <pthread.h>
#include <pthread_np.h>
#include <stdbool.h>

// See SR-6772: Swift cannot directly see mmsghdr/sendmmsg/recvmmsg, so we
// provide shims with a reproduced struct that matches the ABI layout.

typedef struct {
    struct msghdr msg_hdr;
    unsigned int msg_len;
} CNIOFreeBSD_mmsghdr;

typedef struct {
    struct in6_addr ipi6_addr;
    unsigned int ipi6_ifindex;
} CNIOFreeBSD_in6_pktinfo;

int CNIOFreeBSD_sendmmsg(int sockfd, CNIOFreeBSD_mmsghdr *msgvec, unsigned int vlen, int flags);
int CNIOFreeBSD_recvmmsg(int sockfd, CNIOFreeBSD_mmsghdr *msgvec, unsigned int vlen, int flags, struct timespec *timeout);

int CNIOFreeBSD_pthread_setname_np(pthread_t thread, const char *name);
int CNIOFreeBSD_pthread_getname_np(pthread_t thread, char *name, size_t len);

// Non-standard socket stuff.
int CNIOFreeBSD_accept4(int sockfd, struct sockaddr *addr, socklen_t *addrlen, int flags);

// cmsghdr handling
struct cmsghdr *CNIOFreeBSD_CMSG_FIRSTHDR(const struct msghdr *);
struct cmsghdr *CNIOFreeBSD_CMSG_NXTHDR(struct msghdr *, struct cmsghdr *);
const void *CNIOFreeBSD_CMSG_DATA(const struct cmsghdr *);
void *CNIOFreeBSD_CMSG_DATA_MUTABLE(struct cmsghdr *);
size_t CNIOFreeBSD_CMSG_LEN(size_t);
size_t CNIOFreeBSD_CMSG_SPACE(size_t);

extern const int CNIOFreeBSD_SO_TIMESTAMP;
extern const int CNIOFreeBSD_SO_RCVTIMEO;

int CNIOFreeBSD_system_info(struct utsname *uname_data);

const char *CNIOFreeBSD_dirent_dname(struct dirent *ent);

extern const unsigned long CNIOFreeBSD_UTIME_OMIT;
extern const unsigned long CNIOFreeBSD_UTIME_NOW;

extern const long CNIOFreeBSD_UDP_MAX_SEGMENTS;

FTS *CNIOFreeBSD_fts_open(char * const *path_argv, int options, int (*compar)(const FTSENT **, const FTSENT **));

// ECN / IP_PKTINFO constants not in standard FreeBSD headers at the same names
#define IPTOS_ECN_NOT_ECT 0x00
#define IPTOS_ECN_MASK    0x03
#define IPTOS_ECN_ECT0    0x02
#define IPTOS_ECN_ECT1    0x01
#define IPTOS_ECN_CE      0x03
// IP_PKTINFO is IPv4-only and not standard on FreeBSD; stub to -1 (unused path)
#define IP_PKTINFO (-1)
// FreeBSD standard values for IPV6_RECVPKTINFO / IPV6_PKTINFO
#define IPV6_RECVPKTINFO 36
#define IPV6_PKTINFO 46

#endif
#endif
