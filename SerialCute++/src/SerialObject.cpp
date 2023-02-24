#include "../include/SerialObject.h"

SerialObject::SerialObject(std::string input) : input_(input), counter_(0), size_index_(0), photo_index_(0), indicator_("ABCD1$OPTIMUS$2PRIME3!Megatron#4Bumblebee5^Starscream&6Ratchet1$OPTIMUS$2PRIME3!Megatron#4Bumblebee5^Starscream&6Ratchet7*Shockwave(8Ironhide9)Soundwave10Jazz1$OPTIMUS$2PRIME3!Megatron#4Bumblebee5^Starscream&6Ratchet1$OPTIMUS$2PRIME3!Megatron#4Bumblebee5^Starscream&6Ratchet7*Shockwave(8Ironhide9)Soundwave10Jazz"), indicator_index_(0)
{
    memset(&size_buf_, '\0', sizeof(size_buf_));
    memset(&photo_buf_, '\0', sizeof(photo_buf_));
    serial_port = open((const char *)input_.c_str(), O_RDWR);
    std::cout << "Input: " << input_.c_str() << std::endl;
    // Check for errors
    if (serial_port < 0)
    {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        return;
    }

    struct termios tty;

    // Read in existing settings and handle any error
    if (tcgetattr(serial_port, &tty) != 0)
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return;
    }

    tty.c_cflag &= ~PARENB;        // Clear parity bit, disabling parity
    tty.c_cflag &= ~CSTOPB;        // Clear stop field, only one stop bit used in communication
    tty.c_cflag &= ~CSIZE;         // clear all the size bits
    tty.c_cflag |= CS8;            // 8 bits per byte
    tty.c_cflag &= ~CRTSCTS;       // Disable RTS/CTS hardware flow control
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;                                                        // Disable echo
    tty.c_lflag &= ~ECHOE;                                                       // Disale erasure
    tty.c_lflag &= ~ECHONL;                                                      // Disable new line echo
    tty.c_lflag &= ~ISIG;                                                        // Disable interpretation of INTR, QUIT, and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);                                      // Turn off s/w flow control
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpreation of output bytes
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newLine to carraige return/line feed

    tty.c_cc[VTIME] = 10; // Wait for up to 1s (10 deciseconds), returning as soon as any data is received
    tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 9600
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0)
    {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return;
    }
}

SerialObject::SerialObject()
{
    // Open serial port
    // If there is an error opening the serial port,
    // make sure that the serial port login is turned
    // off on the Pi
    serial_port = open("/dev/ttyS0", O_RDWR);

    // Check for errors
    if (serial_port < 0)
    {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        return;
    }

    struct termios tty;

    // Read in existing settings and handle any error
    if (tcgetattr(serial_port, &tty) != 0)
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return;
    }

    tty.c_cflag &= ~PARENB;        // Clear parity bit, disabling parity
    tty.c_cflag &= ~CSTOPB;        // Clear stop field, only one stop bit used in communication
    tty.c_cflag &= ~CSIZE;         // clear all the size bits
    tty.c_cflag |= CS8;            // 8 bits per byte
    tty.c_cflag &= ~CRTSCTS;       // Disable RTS/CTS hardware flow control
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;                                                        // Disable echo
    tty.c_lflag &= ~ECHOE;                                                       // Disale erasure
    tty.c_lflag &= ~ECHONL;                                                      // Disable new line echo
    tty.c_lflag &= ~ISIG;                                                        // Disable interpretation of INTR, QUIT, and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);                                      // Turn off s/w flow control
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpreation of output bytes
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newLine to carraige return/line feed

    tty.c_cc[VTIME] = 10; // Wait for up to 1s (10 deciseconds), returning as soon as any data is received
    tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 9600
    cfsetispeed(&tty, B230400);
    cfsetospeed(&tty, B230400);

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0)
    {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return;
    }
}

// Close serial port when class is destroyed
SerialObject::~SerialObject()
{
    close(serial_port);
}

// Write to serial port
void SerialObject::writeSerial(std::string str)
{
    // This write function comes from the unistd library
    write(serial_port, str.c_str(), str.size());
}

void SerialObject::readSerialImageOrganic()
{
    char read_buf[256];

    // Clear buffer
    memset(&read_buf, '\0', sizeof(read_buf));

    // This read function comes from the unistd library
    int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

    // Check for any errors
    if (num_bytes < 0)
    {
        printf("Error reading: %s", strerror(errno));
    }
    for (int i = 0; i < num_bytes; i++)
    {
        std::cout << read_buf[i];
    }
    std::cout << "\n";
}
void SerialObject::readSerialImage()
{
    char read_buf[256];

    // Clear buffer
    memset(&read_buf, '\0', sizeof(read_buf));

    // This read function comes from the unistd library
    int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

    // Check for any errors
    if (num_bytes < 0)
    {
        printf("Error reading: %s", strerror(errno));
    }
    for (int i = 0; i < 256; i++)
    {
        if (indicator_index_ > indicator_.length() - 1)
        {
            counter_++;
            // std::cout << "Counter: " << counter_ << std::endl;
            indicator_index_ = 0;
        }
        else if (read_buf[i] == indicator_[indicator_index_])
        {
            indicator_index_++;
        }
        if (counter_ == 1)
        {
            // std::cout << "Image size: ";
            if (read_buf[i] != '\0')
            {
                // std::cout << "Size index: " << size_index_ << std::endl;
                size_buf_[size_index_] = read_buf[i];
                size_index_++;
                // std::cout << "Read buf: " << read_buf[i] << std::endl;
                // std::cout << "Size index: " << size_index_ << std::endl;
            }
            /*for(int i = 0; i < size_index_; i++) {
                std::cout << size_buf_[i];
            }*/
            // std::cout << "\n";
        }
        else if (counter_ == 2)
        {
            photo_buf_[photo_index_] = read_buf[i];
            photo_index_++;
        }
        else if (counter_ == 3)
        {
            size_index_ -= indicator_.length();
            for (int i = 0; i < size_index_; i++)
            {
                std::cout << size_buf_[i];
            }
            std::cout << "\n";

            photo_index_ -= indicator_.length();
            std::cout << "Photo Index: " << photo_index_ << std::endl;
            for (int i = 0; i < photo_index_; i++)
            {
                std::cout << photo_buf_[i];
            }
            std::cout << "\n";
            std::cout << "END" << std::endl;
            std::cout << "\n"
                      << std::endl;
            const char *file_hello = "/home/vadl/Desktop/hope2.JPG";
            FILE *f = fopen(file_hello, "w");
            if (f == NULL)
            {
                std::cout << "File opening pain" << std::endl;
            }
            char *mutable_size = new char[strlen(size_buf_) + 1];
            strcpy(mutable_size, size_buf_);
            std::stringstream size_stream(mutable_size);
            size_t the_size;
            size_stream >> the_size;
            std::cout << "SIZE: " << the_size << std::endl;
            // fwrite(inputImage.data, inputImage.step[0]*inputImage.rows, 1, f);
            fwrite(photo_buf_, the_size, 1, f);
            fclose(f);
            std::cout << "Buf 1000: " << photo_buf_[1000] << std::endl;
            /*for(int i = 0; i < photo_index_; i++) {
                std::cout << photo_buf_[i];
            }
            std::cout << "\n";*/
            memset(&size_buf_, '\0', sizeof(size_buf_));
            size_index_ = 0;
            memset(&photo_buf_, '\0', sizeof(photo_buf_));
            photo_index_ = 0;
            counter_ = 0;
            /*for(int i = 0; i < size_index_; i++) {
                std::cout << size_buf_[i];
            }
            std::cout << "\n";*/
        }
        /*else if (counter_ == 2)
        {
            photo_buf_[photo_index_] = read_buf[i];
            photo_index_++;
        }
        else if (counter_ == 3)
        {
            // std::cout << "Photo size size: " << size_index_ << std::endl;
            // std::cout << "Photo size contents: ";
            for (int i = 0; i < size_index_; i++)
            {
                std::cout << size_buf_[i];
            }
            std::cout << std::endl;
            std::stringstream photo_size_stream(size_buf_);
            int official_photo_size;
            photo_size_stream >> official_photo_size;
            std::cout << "Photo size: " << official_photo_size << std::endl;
            std::cout << "Photo contents: ";
            char real_photo_buf[photo_index_ - 6];
            memset(&real_photo_buf, '\0', sizeof(real_photo_buf));
            for (int i = 0; i < photo_index_ - 6; i++)
            {
                real_photo_buf[i] = photo_buf_[i];
                //std::cout << photo_buf_[i];
            }
            //std::cout << std::endl;

            for (int i = 0; i < photo_index_ - 6; i++)
            {
                std::cout << real_photo_buf[i];
            }
            std::cout << std::endl;

            const char *file_hello = "/home/vadl/Desktop/eric_pog.jpeg";
            FILE *f = fopen(file_hello, "w");
            if (f == NULL)
            {
                std::cout << "File opening pain" << std::endl;
            }
            fwrite(real_photo_buf, official_photo_size, 1, f);
            fclose(f);
            memset(&size_buf_, '\0', sizeof(size_buf_));
            memset(&photo_buf_, '\0', sizeof(photo_buf_));
            memset(&real_photo_buf, '\0', sizeof(real_photo_buf));
            size_index_ = 0;
            photo_index_ = 0;
            counter_ = 0;
        }*/
    }
}

std::string SerialObject::readSerial()
{
    char read_buf[256];

    // Clear buffer
    memset(&read_buf, '\0', sizeof(read_buf));

    // This read function comes from the unistd library
    int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

    // Check for any errors
    if (num_bytes < 0)
    {
        printf("Error reading: %s", strerror(errno));
    }

    // Return string version of data that was read
    return std::string(read_buf);
}

std::string SerialObject::readSerial(int buff_size)
{
    char read_buf[buff_size];

    // Clear buffer
    memset(&read_buf, '\0', sizeof(read_buf));

    // This read function comes from the unistd library
    int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

    // Check for any errors
    if (num_bytes < 0)
    {
        printf("Error reading: %s", strerror(errno));
    }

    // Return string version of data that was read
    return std::string(read_buf);
}