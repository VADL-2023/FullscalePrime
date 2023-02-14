// C Library headers
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <sstream>

// Linux headers
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h> // write(), read(), close()

#ifndef SERIALOBJECT_H
#define SERIALOBJECT_H

class SerialObject {

    private:
        
        int serial_port;
        std::string input_;
        int counter_;
        char size_buf_[256];
        char photo_buf_[256];
        int size_index_;
        int photo_index_;
        std::string indicator_;
        int indicator_index_;

    public:

        SerialObject();
        SerialObject(std::string input);
        ~SerialObject();

        void writeSerial(std::string str);

        std::string readSerial();
        void readSerialImage();
        std::string readSerial(int buff_size);

};

#endif
