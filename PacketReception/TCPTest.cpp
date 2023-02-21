#include <iostream> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "./include/PacketReceiver.h"

int main() {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int buffer_len = 255;
    char buffer[buffer_len];
    
    portno = 8001;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        fprintf(stderr,"ERROR opening socket");
    }

    server = gethostbyname("localhost");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host");
        return 0;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *)  &serv_addr,sizeof(serv_addr)) < 0) {
        fprintf(stderr, "ERROR connecting");    
    }
    
    n = read(sockfd,buffer,255);
    if (n < 0) {
        fprintf(stderr,"ERROR reading from socket");
    }

    bool valid = false;
    char message[buffer_len];
    memset(message, 0, buffer_len);
    char *payload = nullptr;
    char c;
    int j = 0;
    int payloadStart = 0;
    for (int i = 0; i < buffer_len; i++) {
        c = buffer[i];
        if (c == 0xc0) {
            valid = !valid;
            i = i + 1;
        } else if (valid) {
            message[j++] = c;
            if (c == 0xf0) {
                payloadStart = j;
            }
        }
    }
    
    std::string s = "";
    for (int i = payloadStart; i < j; i++) {
        s += message[i];
    }
    std::cout << s << std::endl;

    return 0;
}