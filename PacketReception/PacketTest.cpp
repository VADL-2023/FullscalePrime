#include <iostream>
#include "./include/PacketReceiver.h"

int main() {
    PacketReceiver sdr1 = PacketReceiver(1);
    sdr1.startSDR();

    std::string sdr_output = "";

    while (sdr_output.find("shutdown") == std::string::npos) {
        sdr_output = sdr1.getPacket();
        std::cout << sdr_output << std::endl;
    }

    sdr1.stopSDR();

    return 0;
}