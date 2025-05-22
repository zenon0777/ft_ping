#ifndef FT_PING_H
#define FT_PING_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <sys/time.h>
#include <strings.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define ICMP_ECHOREPLY  0
#define ICMP_DEST_UNREACH 3
#define ICMP_ECHO 8  
#define ICMP_TIME_EXCEEDED 11        
#define PING_PKT_S 64

struct icmp_packet {
    struct icmphdr header;
    char            msg[PING_PKT_S - sizeof(struct icmphdr)];
};

#endif