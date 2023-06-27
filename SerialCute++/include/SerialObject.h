/**
 * @file SerialObject.h
 * @brief Serial Object Header File
 * 
 */
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

/**
 * @class SerialObject
 * @brief Serial Communication object 
 * 
 */
class SerialObject {

    private:
        
        /**
         * @brief Serial Port
         * 
         */
        int serial_port;

        /**
         * @brief String input
         * 
         */
        std::string input_;

        /**
         * @brief Character counter
         * 
         */
        int counter_;

        /**
         * @brief Message buffer
         * 
         */
        char size_buf_[256];

        /**
         * @brief Message buffer for photos
         * 
         */
        uint8_t photo_buf_[3000000];

        /**
         * @brief Current index in size buffer
         * 
         */
        int size_index_;

        /**
         * @brief Current index in photo buffer
         * 
         */
        int photo_index_;

        /**
         * @brief Indicator string
         * 
         */
        std::string indicator_;

        /**
         * @brief Index in the indicator string
         * 
         */
        int indicator_index_;

    public:

        /**
         * @brief Construct a new Serial Object object
         * 
         */
        SerialObject();

        /**
         * @brief Construct a new Serial Object object
         * 
         * @param input String representing Serial input
         */
        SerialObject(std::string input);

        /**
         * @brief Destroy the Serial Object object
         * 
         */
        ~SerialObject();

        /**
         * @brief Writes message
         * 
         * @param str The message
         */
        void writeSerial(std::string str);

        /**
         * @brief Reads message
         * 
         * @return The message
         */
        std::string readSerial();

        /**
         * @brief Attempt to read image serially
         * 
         */
        void readSerialImageOrganic();

        /**
         * @brief Another attempt to read image serially (see Serial Prime alive commit to find the real one) 
         * 
         * @param pic_name Filename of image
         * @return true Successfully read image
         * @return false Error in reading image
         */
        bool readSerialImage(const char* pic_name);

        /**
         * @brief Reads serial message up to a certain size
         * 
         * @param buff_size Maximum size of the message
         * @return The serial message 
         */
        std::string readSerial(int buff_size);

};

#endif
