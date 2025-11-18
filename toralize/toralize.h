#ifndef TORALIZE_H        // Include guard start: prevents including this header multiple times
#define TORALIZE_H

#include <stdio.h>        // Standard I/O functions: printf, perror, etc.
#include <stdlib.h>       // General utilities: malloc, free, exit, etc.
#include <string.h>       // String handling functions: strncpy, memset, etc.
#include <unistd.h>       // POSIX API: read, write, close, dup2, etc.
#include <errno.h>        // Error number definitions and strerror

#include <sys/types.h>    // Basic system data types used by sys/socket.h
#include <sys/socket.h>   // Socket API: socket, connect, etc.
#include <netinet/in.h>   // Structures for internet domain addresses (sockaddr_in)
#include <arpa/inet.h>    // Functions for manipulating IP addresses: inet_addr, htons, etc.

#include <dlfcn.h>        // Dynamic linking: dlsym, RTLD_NEXT


// SOCKS proxy configuration
#define PROXY           "127.0.0.1"       // Address of the proxy server (here: localhost)
#define PROXYPORT       9050              // Port that the proxy server listens on (Tor default)
#define USERID          "toraliz"         // User ID to send in the SOCKS4 request
#define REQUEST_SIZE    sizeof(struct proxy_request)   // Size of a SOCKS4 request
#define RESPONSE_SIZE   sizeof(struct proxy_response)  // Size of a SOCKS4 response

// Fixed-width integer type aliases (non-standard but commonly used)
typedef unsigned char  int8;              // 8-bit unsigned integer
typedef unsigned short int16;             // 16-bit unsigned integer
typedef unsigned int   int32;             // 32-bit unsigned integer

// ----------------------
// SOCKS4 protocol structs
// ----------------------

// SOCKS protocol version: 4
// Command code: connect = 1

// Request structure for SOCKS4
struct proxy_request {
    int8  version;        // Protocol version (4 for SOCKS4)
    int8  command;        // Command code (1 = CONNECT)
    int16 port;           // Destination port (network byte order)
    int32 ip;             // Destination IP address (network byte order)
    unsigned char userid[8]; // User ID (fixed size buffer of 8 bytes)
};

typedef struct proxy_request Req;    // Convenient alias for request struct

// Response structure from the proxy server (SOCKS4 reply)
struct proxy_response {
    int8  version;        // Version of the reply (should be 0 for SOCKS4)
    int8  status;         // Status code (90 = request granted, 91+ = errors)
    int16 _;              // Destination port (ignored by client)
    int32 __;             // Destination IP address (ignored by client)
};

typedef struct proxy_response Res;   // Convenient alias for response struct

// ----------------------
// Function declarations
// ----------------------

// Build a SOCKS4 request based on the target address
Req *request(struct sockaddr_in *);

// Interpose / override the standard connect() to tunnel connections through the proxy
int connect(int, const struct sockaddr *, socklen_t);

#endif // TORALIZE_H      // Include guard end
