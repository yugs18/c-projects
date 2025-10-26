#include "toralize.h"

Req *request(struct sockaddr_in *sock2) {
    Req *req = (Req *)malloc(REQUEST_SIZE);

    req->version = 4; // SOCKS4
    req->command = 1; // CONNECT
    req->port = sock2->sin_port; // Destination port
    req->ip = sock2->sin_addr.s_addr; // Destination IP address
    strncpy(req->userid, USERID, 8); // Set user ID

    return req;
}

int connect(int sockfd2, const struct sockaddr *sock2, socklen_t addrlen) {

    int (*p)(int, const struct sockaddr *, socklen_t);
    
    p = dlsym(RTLD_NEXT, "connect");

    int sockfd; // Socket file descriptor
    struct sockaddr_in sock; // Socket address structure
    char buffer[REQUEST_SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return -1;
    }

    sock.sin_family = AF_INET;
    sock.sin_port = htons(PROXYPORT);
    sock.sin_addr.s_addr = inet_addr(PROXY);



    if (p(sockfd, (struct sockaddr *)&sock, sizeof(sock))) {
        perror("Connection to proxy failed");
        close(sockfd);
        return -1;
    }

    printf("Connected to proxy successfully.\n");
    Req *req = request((struct sockaddr_in *)sock2);
    write(sockfd, req, REQUEST_SIZE);

    memset(buffer, 0, RESPONSE_SIZE);

    if (read(sockfd, buffer, RESPONSE_SIZE) < 1) {
        perror("Failed to read response from proxy");
        free(req);
        close(sockfd);
        return -1;
    }

    Res *res = (Res *)buffer;
    int status = (res->status == 90);

    if (status) {
        // printf("Connection to %s:%d through proxy established successfully.\n", host, port);
    } else {
        // printf("Failed to connect to %s:%d through proxy. Status code: %d\n", host, port, res->status);
    }

    dup2(sockfd, sockfd2);

    free(req);

    return 0;
}
