#include <string>

#ifndef PACKETRECEIVER_H
#define PACKETRECEIVER_H

class PacketReceiver {

    private:

        int device_index;
        int default_gain;
        int default_squelch;

        bool rtl_active;
        int rtl_pid;

        std::string input_filename = "direwolf_output0.txt";
        int count = 0;
        
        std::string exec(std::string cmd);

    public:

        PacketReceiver();
        PacketReceiver(int index);
        ~PacketReceiver();

        void startSDR();
        void startSDR_Gain(int gain);
        void startSDR_Squelch(int squelch);
        void startSDR_Squelch(int squelch, std::string prefix);
        void startSDR(int gain, int squelch);
        void startSDR(int gain, int squelch, std::string prefix);
        void stopSDR();
        
        void setDeviceIndex(int index);
        int getDeviceIndex();

        std::string getPackets();
        int countPackets();
};

#endif