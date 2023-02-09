#include <iostream>
#include <iostream>
#include <fstream>
#include "./include/PacketReceiver.h"

void getAntennaData(PacketReceiver radio, std::string csv_filename, int packets_sent, int min_squelch, int max_squelch, int inc_squelch) {
    std::cout << "---------- Running Test ----------" << std::endl;

    // Get the type of antenna from the user
    std::cout << "Antenna: ";
    std::string antenna;
    std::cin >> antenna;
    
    // Get the distance from the user
    std::cout << "Distance: ";
    std::string distance;
    std::cin >> distance;

    // Get receiver polarization
    std::cout << "Polarization: ";
    std::string polarization;
    std::cin >> polarization;
    
    std::string filename_prefix = antenna + "_" + distance + "_";

    // Open a csv to write the output data to
    std::ofstream outputData(csv_filename + ".csv", std::ios::app);
    outputData << antenna << "," << distance << "," << polarization << ",";

    bool waiting = true;
    std::string usrInput;

    for (int sq = min_squelch; sq <= max_squelch; sq += inc_squelch) {
        // Start the SDR with the given squelch value
        std::cout << "Starting rtl_fm with squelch value " + std::to_string(sq) << std::endl;
        radio.startSDR_Squelch(sq, filename_prefix);

        // Wait for the user to stop the SDR
        waiting = true;
        while (waiting) {   
            std::cout << "Ready for next squelch value? (y|n): ";
            std::cin >> usrInput;
            if (usrInput == "y" || usrInput == "Y") {
                waiting = false;
            }
        }
        radio.stopSDR();

        // Determine the number of packets that were received and write to a csv
        int packetsReceived = radio.countPackets();
        std::cout << "Packets received at squelch " + std::to_string(sq) + ": " + std::to_string(packetsReceived) << std::endl;
        if (sq < max_squelch)
            outputData << std::to_string(packetsReceived / (double) packets_sent) << ",";
        else
            outputData << std::to_string(packetsReceived / (double) packets_sent);
    }

    outputData << "\n";
    outputData.close();
}

int main() {
    // Get csv name
    std::cout << "Name of CSV: ";
    std::string csv_filename;
    std::cin >> csv_filename;

    // Get min squelch
    std::cout << "Minimum Squelch Value: ";
    int min_squelch;
    std::cin >> min_squelch;

    // Get the distance from the user
    std::cout << "Maximum Squelch Value: ";
    int max_squelch;
    std::cin >> max_squelch;

    // Get the distance from the user
    std::cout << "Squelch Increment: ";
    int inc_squelch;
    std::cin >> inc_squelch;

    // Get the distance from the user
    std::cout << "How many packets will you send?: ";
    int packets_sent;
    std::cin >> packets_sent;

    // Add labels to csv
    std::ofstream outputData(csv_filename + ".csv", std::ios::app);
    outputData << "Antenna,Distance,Polarization,";
    for (int sq = min_squelch; sq < max_squelch; sq += inc_squelch) {
        outputData << std::to_string(sq) << ",";
    }
    outputData << std::to_string(max_squelch) << "\n";
    outputData.close();

    PacketReceiver radio;
    bool testAntenna = true;
    std::string usr_input;
    do {
        getAntennaData(radio, csv_filename, packets_sent, min_squelch, max_squelch, inc_squelch);
        std::cout << "Do you want to test another antenna (y|n): ";
        std::cin >> usr_input;
        if (usr_input != "y" && usr_input != "Y") {
            testAntenna = false;
        }
    } while (testAntenna);
}