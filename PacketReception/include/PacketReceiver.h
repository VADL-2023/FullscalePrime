#include <string>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "Log.h"

#ifndef PACKETRECEIVER_H
#define PACKETRECEIVER_H

struct AX25Packet {
    std::string source;
    std::string dest;
    std::string msg;
};

class PacketReceiver {

    private:

        int serial_num;     // RTL-SDR serial number
        std::string frequency;           // frequency to tune the SDR to
        int port_num;               // TCP port that direwolf
        std::string configfile;     // name of config file for direwolf

        std::string sdr_script = "sudo ../../runSDR.sh";
        bool rtl_active = false;    // indicates whether the radio is currently running
        int rtl_pid;                // process id for rtl_fm the last time it was started

        int count = 0;              // count of the number of times direwolf has been run

        // TCP port info
        int sockfd;
        struct sockaddr_in serv_addr;
        struct hostent *server;
        
        // Executes cmd on the command line
        // returns the output from the command line
        std::string exec(std::string cmd);

        // Decode Mic-E message
        std::string decodeMicEComment(char* msg, int length);

    public:

        PacketReceiver(int serial_num, std::string frequency, int port, std::string configfile);
        ~PacketReceiver();

        void startSDR();

        void startSDR(Log& m_log);
        void stopSDR();
        
        int packetAvailable();
        AX25Packet getPacket();
};

#endif