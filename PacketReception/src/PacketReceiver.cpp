#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include </usr/include/signal.h>

#include "../include/PacketReceiver.h"

PacketReceiver::PacketReceiver(int device_num) : rtl_active{false}, sdr_num(device_num) {
    if (device_num == 2) {
        script_name = "../../runSDR2.sh ";
        port_num = 9001;
    } else {
        // default values
        // could make this a struct instead??
        script_name = "../../runSDR1.sh ";
        port_num = 8001;
    }
}

PacketReceiver::~PacketReceiver(){
    stopSDR();
}

void PacketReceiver::startSDR() {
    stopSDR();
    input_filename = "sdr" + std::to_string(sdr_num) +  "_output" + std::to_string(count++) + ".txt";
    std::string command = script_name + " " + input_filename;
    rtl_pid = std::stoi(exec(command.c_str()));
    rtl_active = true;
    sleep(1);

    // Connect to TCP port
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        fprintf(stderr,"ERROR opening socket\n");
        return;
    }

    server = gethostbyname("localhost");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        return;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port_num);

    if (connect(sockfd,(struct sockaddr *)  &serv_addr,sizeof(serv_addr)) < 0) {
        fprintf(stderr, "ERROR connecting.\n");
        return;    
    }
}

void PacketReceiver::stopSDR() {
    if (rtl_active) {
        close(sockfd);
        sleep(1);
        kill(rtl_pid, SIGTERM);
        sleep(2);
    }
    rtl_active = false;
}

int PacketReceiver::packetAvailable() {
    struct pollfd fds;
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_sec = 1000000;
    fds.fd = sockfd;
    fds.events = POLLIN;
    int r = ppoll (&fds, 1, &ts, NULL);
    return r;
}

std::string PacketReceiver::getPacket() {
    std::string s = "";

    if (rtl_active) {
        int buffer_len = 255;
        char buffer[buffer_len];
        memset(buffer, 0, buffer_len);

        int n = read(sockfd,buffer,255);
        if (n < 0) {
            fprintf(stderr,"ERROR reading from socket\n");
            std::cout << std::to_string(port_num) << std::endl;
            return "ERROR";
        }

        bool valid = false;
        char message[buffer_len];
        memset(message, 0, buffer_len);
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
        
        // for (int i = 0; i < 255; i++) {
        //     printf("%x ", buffer[i]);
        // }

        // std::cout << std::endl;

        // for (int i = payloadStart; i < j; i++) {
        //     printf("%x ", message[i]);
        // }

        // std::cout << std::endl;

        for (int i = payloadStart; i < j; i++) {
            s += message[i];
        }
    } 
    return s;
}

std::string PacketReceiver::exec(std::string cmd) {
    char buffer[128];
    std::string result = "";

    // Open the command for reading.
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        exit(1);
    }

    // Read the output a line at a time - output it
    while (!feof(pipe)) {
      // use buffer to read and add to result
      if (fgets(buffer, 128, pipe) != NULL)
         result += buffer;
   }

    // Close
    pclose(pipe);
    return result;
}