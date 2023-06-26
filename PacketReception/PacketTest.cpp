/**
 * @file PacketTest.cpp
 * @brief Simply tests if we can receive APRS Packet
 * 
 */
#include <iostream>
#include <unistd.h>
#include "./include/PacketReceiver.h"

/**
 * @brief Main method to test APRS Packet Reception
 * 
 * @return int representing exit status 
 */
int main() {
    std::string configfile = "/home/vadl/FullscalePrime/sdr1.conf";
    PacketReceiver sdr1 = PacketReceiver(1, "144.97M", 8001, configfile);
    sdr1.startSDR();

    AX25Packet p;

    while (p.source.find("ERROR") == std::string::npos) {
        if (sdr1.packetAvailable()) {
            p = sdr1.getPacket();
            std::cout << "Source: " << p.source << std::endl;
            std::cout << "Destination: " << p.dest << std::endl;
            std::cout << "Message: " << p.msg << std::endl;
        }
        usleep(1000);
    }

    sdr1.stopSDR();

    return 0;
}