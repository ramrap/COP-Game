#include "client_udp.hpp"
#include "constans.hpp"
#include <iostream>

void prepare_client(int *sock, struct sockaddr_in *client_addr)
{
    if ((*sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("client socket failed");
    }

    if (bind(*sock, (struct sockaddr *)client_addr, sizeof(struct sockaddr)) < 0)
    {
        perror("bind client error");
    }
}

void send_to_server(int sock, struct sockaddr_in serv_addr, int16_t id, int16_t winner)
{
    int16_t tab[2];
    tab[0] = id;
    tab[1] = winner;
    if (id == -3)
    {
        // std::cout<<"SENDING WINNER DATA ";
    }

    socklen_t serv_addr_size = sizeof(struct sockaddr);
    if (sendto(sock, tab, sizeof(int16_t) * 2, 0,
               (struct sockaddr *)&serv_addr, serv_addr_size) < 0)
    {
        perror("sendto error");
    }
}

int client_listen(int sock, int16_t *tab)
{
    int length = recvfrom(sock, tab, sizeof(int16_t) * BUF_MAX, 0, NULL, 0);
    return length;
}
