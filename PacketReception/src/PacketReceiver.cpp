#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include </usr/include/signal.h>

#include "../include/PacketReceiver.h"

PacketReceiver::PacketReceiver() : device_index{0}, default_gain{42}, default_squelch{0}, rtl_active{false} {
    // No need to do anything here
}

PacketReceiver::PacketReceiver(int index) : device_index{index}, default_gain{42}, default_squelch{0}, rtl_active{false} {
    // No need to do anything here
}

PacketReceiver::~PacketReceiver(){
    stopSDR();
}

void PacketReceiver::startSDR() {
    startSDR(default_gain, default_squelch, "");
}

void PacketReceiver::startSDR_Gain(int gain) {
    startSDR(gain, default_squelch, "");
}

void PacketReceiver::startSDR_Squelch(int squelch) {
    startSDR(default_gain, squelch, "");
}

void PacketReceiver::startSDR_Squelch(int squelch, std::string prefix) {
    startSDR(default_gain, squelch, prefix);
}

void PacketReceiver::startSDR(int gain, int squelch) {
    startSDR(gain, squelch, "");
}

void PacketReceiver::startSDR(int gain, int squelch, std::string prefix) {
    stopSDR();
    input_filename = prefix + "direwolf_output" + std::to_string(count++) + ".txt";
    std::string command = "../../runSDR.sh " + std::to_string(device_index) + " " + std::to_string(gain) + " " + std::to_string(squelch) + " " + input_filename;
    rtl_pid = std::stoi(exec(command.c_str()));
    rtl_active = true;
}

void PacketReceiver::stopSDR() {
    if (rtl_active) {
        kill(rtl_pid, SIGTERM);
        sleep(2);
    }
    rtl_active = false;
}

void PacketReceiver::setDeviceIndex(int index) {
    device_index = index;
}

int PacketReceiver::getDeviceIndex() {
    return device_index;
}

std::string PacketReceiver::getPackets() {
    std::string receivedPackets;

    std::ifstream ifile(input_filename);
    
    std::string call_sign = "KQ4DPB";
    std::string packet_start_text = call_sign + ">BEACON:";
    std::string inputString;
    int packet_count = 0;

    while(std::getline(ifile, inputString)) {
        int packet_start_index = inputString.find(packet_start_text);
        if (packet_start_index != -1) {
            // Extract the packet
            packet_start_index += packet_start_text.length();
            int packet_end_index = inputString.find("<");
            std::string packet_data = inputString.substr(packet_start_index, packet_end_index - packet_start_index);
            receivedPackets += "Packet " + std::to_string(++packet_count) + ": " + packet_data + "\n";
        }
    }

    return receivedPackets;
}

int PacketReceiver::countPackets() {
    int packetCount = 0;

    std::ifstream ifile(input_filename);
    
    std::string call_sign = "KQ4DPB";
    std::string packet_start_text = call_sign + ">BEACON:";
    std::string inputString;

    while(std::getline(ifile, inputString)) {
        int packet_start_index = inputString.find(packet_start_text);
        if (packet_start_index != -1) {
            packetCount++;
        }
    }

    return packetCount;
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