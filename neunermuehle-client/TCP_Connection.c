#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define PORT "1357"

// void performConnection(int sock) <- Jonas

int createConnection() {
    struct addrinfo *result; // pointer to a linked list of struct addrinfo returned by the getaddrinfo()-function
    struct addrinfo criteria; // search-criteria for getaddrinfo()-function
    memset(&criteria, 0, sizeof(criteria));
    criteria.ai_family = AF_INET; // use IPv4-adresses
    criteria.ai_socktype = SOCK_STREAM; // use TCP-socket

    // translation of hostname into IP-adress:
    if (getaddrinfo(HOSTNAME, PORT, &criteria, &result) != 0) {
        perror("Invalid address.");
        return -1;
    }
    // socket creation:
    int sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sock < 0) {
        perror("Socket creation failed.");
        freeaddrinfo(result);
        return -1;
    }
    // connection establishment:
    if (connect(sock, result->ai_addr, result->ai_addrlen) < 0) {
        perror("Connection establishment failed.");
        close(sock);
        freeaddrinfo(result);
        return -1;
    }

    performConnection(sock);
    close(sock);
    freeaddrinfo(result);
    return 0;
}

int main() {
    if (createConnection() != 0) {
        printf("Failed to establish connection.\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}