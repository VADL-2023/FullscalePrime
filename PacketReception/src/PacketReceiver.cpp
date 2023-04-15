#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include </usr/include/signal.h>

#include "../include/PacketReceiver.h"

PacketReceiver::PacketReceiver(int serial_num, std::string frequency, int port, std::string configfile) : serial_num{serial_num}, frequency{frequency}, port_num{port}, configfile{configfile}
{
}

PacketReceiver::~PacketReceiver()
{
    stopSDR();
}

void PacketReceiver::startSDR()
{
    stopSDR();
    std::string input_filename = "sdr" + std::to_string(serial_num) + "_output" + std::to_string(count++) + ".txt";
    std::string command = sdr_script + " -o " + input_filename + " -d " + std::to_string(serial_num) + " -f " + frequency + " -c " + configfile;
    std::cout << "Command: " << command << std::endl;
    bool got_pid = false;
    for (int i = 0; i < 5 && !got_pid; i++)
    {
        try
        {
            rtl_pid = std::stoi(exec(command.c_str()));
            got_pid = true;
        }
        catch (...)
        {
            std::cout << "PID was null. Trying to execute again" << std::endl;
        }
    }
    sleep(1);

    // Connect to TCP port
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        fprintf(stderr, "ERROR opening socket\n");
        return;
    }

    server = gethostbyname("localhost");
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        return;
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port_num);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        fprintf(stderr, "ERROR connecting\n");
        return;
    }

    rtl_active = true;
}

void PacketReceiver::startSDR(Log &m_log)
{
    stopSDR();
    std::string input_filename = "sdr" + std::to_string(serial_num) + "_output" + std::to_string(count++) + ".txt";
    std::string command = sdr_script + " -o " + input_filename + " -d " + std::to_string(serial_num) + " -f " + frequency + " -c " + configfile;
    //std::cout << "Command: " << command << std::endl;
    bool got_pid = false;
    for (int i = 0; i < 5 && !got_pid; i++)
    {
        try
        {
            rtl_pid = std::stoi(exec(command.c_str()));
            got_pid = true;
        }
        catch (...)
        {
            m_log.write("PID was null. Trying to execute again");
        }
    }
    sleep(1);

    // Connect to TCP port
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        fprintf(stderr, "ERROR opening socket\n");
        return;
    }

    server = gethostbyname("localhost");
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        return;
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port_num);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        fprintf(stderr, "ERROR connecting\n");
        return;
    }

    rtl_active = true;
}

void PacketReceiver::stopSDR()
{
    if (rtl_active)
    {
        close(sockfd);
        sleep(1);
        kill(rtl_pid, SIGTERM);
        sleep(1);
    }
    rtl_active = false;
}

int PacketReceiver::packetAvailable()
{
    if (!rtl_active)
        return 0;

    struct pollfd fds;
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 1000000;
    fds.fd = sockfd;
    fds.events = POLLIN;
    int r = ppoll(&fds, 1, &ts, NULL);
    return r == 1;
}

AX25Packet PacketReceiver::getPacket()
{
    AX25Packet p;

    if (rtl_active)
    {
        int buffer_len = 255;
        char buffer[buffer_len];
        memset(buffer, 0, buffer_len);

        int n = read(sockfd, buffer, 255);
        if (n < 0)
        {
            fprintf(stderr, "ERROR reading from socket\n");
            p.source = "ERROR";
            p.dest = "ERROR";
            p.msg = "ERROR";
            return p;
        }

        bool valid = false;
        char message[buffer_len];
        memset(message, 0, buffer_len);
        char c;
        int j = 0;

        // Extract the KISS frame
        for (int i = 0; i < buffer_len; i++)
        {
            c = buffer[i];
            if (c == 0xc0)
            {
                valid = !valid;
                i = i + 1;
            }
            else if (valid)
            {
                message[j++] = c;
            }
        }

        // Extract the data and translate it
        int len_frame = j;

        char dest_addr[7];
        char dest_ssid;
        char source_addr[7];
        char source_ssid;
        char control[2];
        char data[len_frame - 15];

        memset(dest_addr, 0, 7);
        memset(source_addr, 0, 7);
        memset(data, 0, len_frame - 15);

        for (int i = 0; i < len_frame; i++)
        {
            // Extract the destination address
            if (i < 6)
            {
                dest_addr[i] = message[i] >> 1;

                // Extract the destination SSID
            }
            else if (i < 7)
            {
                dest_ssid = (message[i] >> 1) & 0x0F;

                // Extract the source address
            }
            else if (i < 13)
            {
                source_addr[i - 7] = message[i] >> 1;

                // Extract the source SSID
            }
            else if (i < 14)
            {
                source_ssid = (message[i] >> 1) & 0x0F;

                // Extract the control bits
            }
            else if (i < 16)
            {
                control[i - 14] = message[i];

                // Extract the message (called data)
            }
            else if (i)
            {
                data[i - 16] = message[i];
            }
        }

        // for (int i = 0; i < 255; i++) {
        //     printf("%x ", buffer[i]);
        // }

        // std::cout << std::endl;

        // for (int i = 0; i < j; i++) {
        //     printf("%x ", message[i]);
        // }

        // std::cout << std::endl;

        //
        if (data[0] == '`' || data[0] == '‘' || data[0] == '\'' || data[0] == 0x1C || data[0] == 0x1D) {
            p.msg = decodeMicEComment(data, len_frame - 15);
        } else {
            p.msg = data;
        }

        p.source = source_addr;
        p.source_ssid = source_ssid;
        p.dest = dest_addr;
    } 

    return p;
}

std::string PacketReceiver::decodeMicEComment(char* msg, int length) {
    // Verify that it is a MicE packet
    if (length < 9 || !(*msg == '`' || *msg == '‘' || *msg == '\'' || *msg == 0x1C || *msg == 0x1D)) {
        return "";
    }

    // status text offset
    int i = 10;
    
    // get status text
    std::string status_text = "";
    while (i < length && *(msg + i) != '_') {
        char c = *(msg + i);
        // make sure it is ascii
        if (c >= 32 && c <= 127 ) {
            status_text += c;
            i++;
        } else {
            // return if it is not an ascii character
            return "";
        }
    }

    return status_text;
}

// Executes cmd on the command line
// returns the output from the command line
std::string PacketReceiver::exec(std::string cmd)
{
    char buffer[128];
    std::string result = "";

    // Open the command for reading.
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe)
    {
        exit(1);
    }

    // Read the output a line at a time - output it
    while (!feof(pipe))
    {
        // use buffer to read and add to result
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }

    // Close
    pclose(pipe);
    return result;
}