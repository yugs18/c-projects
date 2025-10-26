#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <dlfcn.h>


#define PROXY           "127.0.0.1"
#define PROXYPORT       9050
#define USERID         "toraliz"
#define REQUEST_SIZE    sizeof(struct proxy_request)
#define RESPONSE_SIZE   sizeof(struct proxy_response)

typedef unsigned char int8;
typedef unsigned short int16;
typedef unsigned int int32;

// Socks protocol version - 4
// Command code - connect = 1
// Request structure for SOCKS4
struct proxy_request {
    int8 version;        // Protocol version
    int8 command;        // Command code
    int16 port;         // Destination port
    int32 ip;           // Destination IP address
    unsigned char userid[8]; // User ID (variable length)
};

typedef struct proxy_request Req;

// Response structure from the proxy server
struct proxy_response {
    int8 version;       // Version of the reply (should be 0)
    int8 status;        // Response status (90 = request granted)
    int16 _;        // Destination port (should be ignored)
    int32 __;          // Destination IP address (should be ignored)
};

typedef struct proxy_response Res;

// Function declarations
Req *request(struct sockaddr_in *);
int connect(int, const struct sockaddr *, socklen_t);