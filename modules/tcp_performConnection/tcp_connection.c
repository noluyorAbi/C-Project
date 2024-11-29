#include "performConnection.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define PORT "1357"

int createConnection() {
  int sock = -1; // File descriptor for socket, -1 indicates uninitialized
  struct addrinfo
    *result; // Pointer to the list of address results (returned by getaddrinfo)
  struct addrinfo *rp;         // Iterator for address list
  struct addrinfo hints = {0}; // Hints for getaddrinfo configuration
  // Set connection criteria for getaddrinfo
  hints.ai_family = AF_UNSPEC;     // IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; // TCP

  // Translate hostname and port into a list of address structures
  // (IP-addresses)
  int status = getaddrinfo(HOSTNAME, PORT, &hints, &result);
  if (status != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return EXIT_FAILURE;
  }

  // Iterate through the address list and attempt to connect to one of them
  for (rp = result; rp != NULL; rp = rp->ai_next) {
    // Try to create a socket for the current address
    sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (sock == -1) {
      continue;
    } // Socket creation failed, try the next address

    if (connect(sock, rp->ai_addr, rp->ai_addrlen) == 0) {
      break; // Successful connection
    }

    // If connection failed, close the socket and try the next address
    close(sock);
    sock = -1;
  }

  freeaddrinfo(result); // Free the memory allocated by getaddrinfo

  if (sock == -1) {
    fprintf(stderr, "Could not connect to %s: %s\n", HOSTNAME, PORT);
    return EXIT_FAILURE;
  }

  if (performConnection(sock) != 0) {
    fprintf(stderr, "performConnection failed.\n");
    close(sock);
    return EXIT_FAILURE;
  }

  close(sock);
  return EXIT_SUCCESS;
}

int main() {
  if (createConnection() != 0) {
    fprintf(stderr, "Failed to establish connection.\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}