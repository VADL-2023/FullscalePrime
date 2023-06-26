/**
 * @file PacketReceiver.h
 * @brief Library to receive APRS Packet
 * 
 */
#include <string>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "Log.h"

#ifndef PACKETRECEIVER_H
#define PACKETRECEIVER_H

/**
 * @struct AX25Packet
 * @brief Contains all packet information in the standard AX.25 protocol
 * 
 */
struct AX25Packet {
    /**
     * @brief Source of packet represented as string
     * 
     */
    std::string source;

    /**
     * @brief SSID for the source
     * 
     */
    int source_ssid;

    /**
     * @brief Packet destination represented as a string
     * 
     */
    std::string dest;

    /**
     * @brief Message encoded by APRS packet
     * 
     */
    std::string msg;
};

/**
 * @class PacketReceiver
 * @brief PacketReceiver Object for APRS 
 * 
 */
class PacketReceiver {

    private:

        /**
         * @brief RTL-SDR serial number
         * 
         */
        int serial_num; 

        /**
         * @brief Frequency to tune the SDR to
         * 
         */
        std::string frequency; 
        
        /**
         * @brief TCP Port for Direwolf
         * 
         */
        int port_num;       

        /**
         * @brief Name of config file for Direwolf
         * 
         */
        std::string configfile;   

        /**
         * @brief Command Line instruction to run SDR script
         * 
         */
        std::string sdr_script = "sudo ../../runSDR.sh";

        /**
         * @brief Indicates whether the radio is currently running
         * 
         */
        bool rtl_active = false;   

        /**
         * @brief Process ID for RTL_FM the last time it was started
         * 
         */
        int rtl_pid;              

        /**
         * @brief Count the number of times Direwolf has been run
         * 
         */
        int count = 0;             

        /**
         * @brief TCP Port info
         * 
         */
        int sockfd;

        /**
         * @brief TCP Port info server address
         * 
         */
        struct sockaddr_in serv_addr;

        /**
         * @brief TCP Port info server
         * 
         */
        struct hostent *server;
        
        /**
         * @brief Executes cmd on the command line
         * 
         * @param cmd Command line instruction
         * @return string representing output from terminal 
         */
        std::string exec(std::string cmd);

        /**
         * @brief Decodes Mic-E message
         * 
         * @param msg The message
         * @param length The length of msg
         * @return string representing the decoded message 
         */
        std::string decodeMicEComment(char* msg, int length);

    public:

        /**
         * @brief Construct a new Packet Receiver object
         * 
         * @param serial_num Serial number
         * @param frequency Desired frequency
         * @param port TCP Port
         * @param configfile Direwolf config file
         */
        PacketReceiver(int serial_num, std::string frequency, int port, std::string configfile);

        /**
         * @brief Destroy the Packet Receiver object
         * 
         */
        ~PacketReceiver();

        /**
         * @brief Initializes SDR
         * 
         */
        void startSDR();

        /**
         * @brief Initializes SDR so Log can track it
         * 
         * @param m_log Pointer to the Log object
         */
        void startSDR(Log& m_log);

        /**
         * @brief Terminates SDR
         * 
         */
        void stopSDR();

        /**
         * @brief Checks if APRS Packet is available
         * 
         * @return true APRS Packet is available
         * @return false APRS Packet is not available
         */
        bool packetAvailable();

        /**
         * @brief Get the AX25Packet
         * 
         * @return The APRS Packet 
         */
        AX25Packet getPacket();
};

#endif