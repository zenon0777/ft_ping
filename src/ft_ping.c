#include "../ft_ping.h"

unsigned int csum(struct icmp_packet *packet, size_t h_size, size_t p_size)
{
    
}


void build_icmp_packet(struct addrinfo *rp, int seq)
{

    struct icmp_packet packet;
    struct timeval tv;

    if (gettimeofday(&tv, NULL) != 0)
    {
        perror("Error getting current time");
        return;
    }

    bzero(&packet, sizeof(packet));
    packet.header.type = ICMP_ECHO;
    packet.header.code = 0;
    packet.header.checksum = 0;
    packet.header.un.echo.sequence = seq;
    packet.header.un.echo.id = getpid();

    memcpy(packet.msg, &tv, sizeof(tv));
    memset(packet.msg + sizeof(tv),
           0xA5,
           sizeof(packet.msg) - sizeof(tv));

    packet.header.checksum = csum(&packet, sizeof(packet.header), sizeof(packet.msg));
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s target\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char *target = argv[1];
    struct addrinfo hints = {0}, *res, *rp;
    int sock;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;

    if (getaddrinfo(target, NULL, &hints, &res) != 0)
    {
        fprintf(stderr, "ping: cannot resolve %s : Unknown host\n", target);
        exit(EXIT_FAILURE);
    }

    for (rp = res; rp != NULL; rp = rp->ai_next)
    {
        sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sock < 0)
        {
            continue;
        }
        if (connect(sock, rp->ai_addr, rp->ai_addrlen) == 0)
        {
            break;
        }
        close(sock);
    }

    if (rp == NULL)
    {
        fprintf(stderr, "Failed to resolve or connect to %s\n", target);
        freeaddrinfo(res);
        exit(EXIT_FAILURE);
    }

    /* For reporting, show which IP we’ll ping: */
    char ipstr[INET_ADDRSTRLEN];
    struct sockaddr_in *sin = (struct sockaddr_in *)rp->ai_addr;
    inet_ntop(AF_INET, &sin->sin_addr, ipstr, sizeof(ipstr));
    printf("PING %s (%s): %d bytes of data.\n", target, ipstr, 56);

    send_recv_loop(rp);

    freeaddrinfo(res);
    close(sock);
    return 0;
}
