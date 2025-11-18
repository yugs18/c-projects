#include "toralize.h"

// Build a SOCKS4 request for the given target address
Req *request(struct sockaddr_in *sock2) {
    // Allocate memory for the request structure
    Req *req = (Req *)malloc(REQUEST_SIZE);

    req->version = 4;                     // SOCKS4 protocol version
    req->command = 1;                     // CONNECT command
    req->port = sock2->sin_port;          // Destination port (already in network byte order)
    req->ip = sock2->sin_addr.s_addr;     // Destination IP address (network byte order)
    strncpy(req->userid, USERID, 8);      // Copy user ID (up to 8 bytes)

    return req;
}

// Override the standard connect() function to route connections via SOCKS proxy
int connect(int sockfd2, const struct sockaddr *sock2, socklen_t addrlen) {

    // Function pointer to hold the original connect()
    int (*p)(int, const struct sockaddr *, socklen_t);
    
    // Get the "real" connect() using dlsym and RTLD_NEXT
    p = dlsym(RTLD_NEXT, "connect");

    int sockfd;                 // Socket used to talk with the proxy server
    struct sockaddr_in sock;    // Address of the proxy server
    char buffer[REQUEST_SIZE];  // Buffer to read proxy response into

    // Create a new TCP socket to connect to the proxy
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Fill in proxy address (127.0.0.1:9050)
    sock.sin_family = AF_INET;
    sock.sin_port = htons(PROXYPORT);
    sock.sin_addr.s_addr = inet_addr(PROXY);

    // Connect to the SOCKS proxy using the original connect()
    if (p(sockfd, (struct sockaddr *)&sock, sizeof(sock))) {
        perror("Connection to proxy failed");
        close(sockfd);
        return -1;
    }

    printf("Connected to proxy successfully.\n");

    // Build SOCKS4 request for the actual destination
    Req *req = request((struct sockaddr_in *)sock2);

    // Send the request to the proxy
    write(sockfd, req, REQUEST_SIZE);

    // Clear the response buffer
    memset(buffer, 0, RESPONSE_SIZE);

    // Read response from proxy
    if (read(sockfd, buffer, RESPONSE_SIZE) < 1) {
        perror("Failed to read response from proxy");
        free(req);
        close(sockfd);
        return -1;
    }

    // Interpret the response
    Res *res = (Res *)buffer;
    int status = (res->status == 90); // 90 = request granted in SOCKS4

    if (status) {
        // Connection via proxy was successful
        // (You could print more info here if needed)
        // printf("Connection through proxy established successfully.\n");
    } else {
        // Proxy refused the connection
        // printf("Failed to connect through proxy. Status code: %d\n", res->status);
    }

    // Duplicate proxy socket file descriptor onto the original sockfd2
    // so further I/O on sockfd2 actually goes through the proxy connection.
    dup2(sockfd, sockfd2);

    // Free allocated request
    free(req);

    return 0;
}
