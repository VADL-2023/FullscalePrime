#include <iostream>
#include <unistd.h>
#include "./include/PacketReceiver.h"

int main() {
    PacketReceiver sdr1 = PacketReceiver(2);
    sdr1.startSDR();

    std::string sdr_output = "";

    while (sdr_output.find("shutdown") == std::string::npos && sdr_output.find("ERROR") == std::string::npos) {
        if (sdr1.packetAvailable() != -1) {
            std::cout << "Available" << std::endl;
            sdr_output = sdr1.getPacket();
            std::cout << sdr_output << std::endl;
        }
        usleep(1000);
    }

    sdr1.stopSDR();

    return 0;
}