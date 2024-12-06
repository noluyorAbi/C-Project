#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
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

// Mocked global variables
static int mock_socket_result = -1;
static int mock_connect_result = -1;
static int mock_performConnection_result = 0;

// Mocked `socket` function
int __wrap_socket(int domain, int type, int protocol) {
  return mock_socket_result;
}

// Mocked `connect` function
int __wrap_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
  return mock_connect_result;
}

// Mocked `performConnection` function
int __wrap_performConnection(int sock) {
  return mock_performConnection_result;
}

// Mocked `getaddrinfo` function
int __wrap_getaddrinfo(const char *node, const char *service,
                       const struct addrinfo *hints, struct addrinfo **res) {
  if (strcmp(node, HOSTNAME) != 0 || strcmp(service, PORT) != 0) {
    return EAI_FAIL; // Simulate hostname resolution failure
  }

  // Allocate a mock address info
  struct addrinfo *mock_result = malloc(sizeof(struct addrinfo));
  mock_result->ai_family = AF_INET;
  mock_result->ai_socktype = SOCK_STREAM;
  mock_result->ai_protocol = 0;
  mock_result->ai_addrlen = sizeof(struct sockaddr_in);
  mock_result->ai_addr = malloc(sizeof(struct sockaddr_in));
  ((struct sockaddr_in *)mock_result->ai_addr)->sin_family = AF_INET;
  ((struct sockaddr_in *)mock_result->ai_addr)->sin_port = htons(atoi(PORT));
  inet_pton(AF_INET, "127.0.0.1",
            &((struct sockaddr_in *)mock_result->ai_addr)->sin_addr);
  mock_result->ai_next = NULL;

  *res = mock_result;
  return 0; // Simulate success
}

// Mocked `freeaddrinfo` function
void __wrap_freeaddrinfo(struct addrinfo *res) {
  free(res->ai_addr);
  free(res);
}

// Test case: Successful connection
static void test_successful_connection(void **state) {
  mock_socket_result = 3; // Valid socket descriptor
  mock_connect_result = 0;
  mock_performConnection_result = 0;

  int result = createConnection();
  assert_int_equal(result, EXIT_SUCCESS);
}

// Test case: Hostname resolution failure
static void test_hostname_resolution_failure(void **state) {
  will_return(__wrap_getaddrinfo, EAI_FAIL);
  int result = createConnection();
  assert_int_equal(result, EXIT_FAILURE);
}

// Test case: Socket creation failure
static void test_socket_creation_failure(void **state) {
  mock_socket_result = -1;
  int result = createConnection();
  assert_int_equal(result, EXIT_FAILURE);
}

// Test case: Connection failure
static void test_connection_failure(void **state) {
  mock_socket_result = 3; // Valid socket descriptor
  mock_connect_result = -1; // Simulate failure
  int result = createConnection();
  assert_int_equal(result, EXIT_FAILURE);
}

// Test case: performConnection failure
static void test_performConnection_failure(void **state) {
  mock_socket_result = 3; // Valid socket descriptor
  mock_connect_result = 0; // Connection successful
  mock_performConnection_result = -1; // Simulate failure

  int result = createConnection();
  assert_int_equal(result, EXIT_FAILURE);
}

// Test case: Memory cleanup
static void test_memory_cleanup(void **state) {
  struct addrinfo *mock_res;
  __wrap_getaddrinfo(HOSTNAME, PORT, NULL, &mock_res);

  mock_socket_result = -1;
  int result = createConnection();
  assert_int_equal(result, EXIT_FAILURE);
}

// Main function to run tests
int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_successful_connection),
      cmocka_unit_test(test_hostname_resolution_failure),
      cmocka_unit_test(test_socket_creation_failure),
      cmocka_unit_test(test_connection_failure),
      cmocka_unit_test(test_performConnection_failure),
      cmocka_unit_test(test_memory_cleanup),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}