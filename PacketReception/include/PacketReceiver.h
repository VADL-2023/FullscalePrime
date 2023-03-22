#include <string>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#ifndef PACKETRECEIVER_H
#define PACKETRECEIVER_H

class PacketReceiver {

    private:

        bool rtl_active;
        int rtl_pid;
        int sdr_num;
        std::string serial_num;
        std::string script_name;
        std::string input_filename = "direwolf_output0.txt";
        int count = 0;

        // TCP port info
        int port_num;
        int sockfd;
        struct sockaddr_in serv_addr;
        struct hostent *server;
        
        std::string exec(std::string cmd);

    public:

        PacketReceiver(int device_num);
        ~PacketReceiver();

        void startSDR();
        void stopSDR();
        
        int packetAvailable();
        std::string getPacket();
};

#endif