/**
 * @file SerialWrite.cpp
 * @brief Alternate Serial test (maybe used for images, probably should be deprecated)
 * 
 */
// C Library headers
#include <stdio.h>
#include <cstring>
#include <string>

// Linux headers
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h> // write(), read(), close()

/**
 * @brief Main method trying to test Serial Object
 * 
 * @return int representing exit status
 */
int main()
{
    // Open serial port
    int serial_port = open("/dev/ttyS0", O_RDWR);

    // Check for errors
    if (serial_port < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
    }

    struct termios tty;

    // Read in existing settings and handle any error
    if (tcgetattr(serial_port, &tty) != 0)
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return 1;
    }

    tty.c_cflag &= ~PARENB;     // Clear parity bit, disabling parity
    tty.c_cflag &= ~CSTOPB;     // Clear stop field, only one stop bit used in communication
    tty.c_cflag &= ~CSIZE;      //clear all the size bits
    tty.c_cflag |= CS8;         // 8 bits per byte
    tty.c_cflag &= ~CRTSCTS;    // Disable RTS/CTS hardware flow control
    tty.c_cflag |= CREAD | CLOCAL;  // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;       // Disable echo
    tty.c_lflag &= ~ECHOE;      // Disale erasure
    tty.c_lflag &= ~ECHONL;     // Disable new line echo
    tty.c_lflag &= ~ISIG;       // Disable interpretation of INTR, QUIT, and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow control
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);    //Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST;      // Prevent special interpreation of output bytes
    tty.c_oflag &= ~ONLCR;      // Prevent conversion of newLine to carraige return/line feed

    tty.c_cc[VTIME] = 10;       // Wait for up to 1s (10 deciseconds), returning as soon as any data is received
    tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 9600
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return 1;
    }

    // Writing
    unsigned char msg[] = {'H', 'e', 'l', 'l', 'o', '\r'};
    write(serial_port, msg, sizeof(msg));

    // Reading
    char read_buf[256];
    memset(&read_buf, '\0', sizeof(read_buf));
    int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

    if (num_bytes < 0)
    {
        printf("Error reading: %s", strerror(errno));
    }

    printf("Read %i bytes. Received message %s", num_bytes, read_buf);

    close(serial_port);
    return 0;
}