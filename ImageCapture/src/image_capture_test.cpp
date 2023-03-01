#include <iostream>
#include "SerialObject.h"
#include <map>
#include <sstream>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
int main()
{
    SerialObject testObject("/dev/ttyUSB0");
    std::string start_indicator = "$START$";
    std::string photo_indicator = "$PHOTO$";
    std::string end_indicator = "$END$";
    std::string photo_size = "";
    std::string photo = "";
    int i = 1;
    bool got_packet = false;
    while (!got_packet)
    {
        // std::cout << "I: " << i << std::endl;
        testObject.writeSerial("Autobots! Transform and roll out");
        std::string thing = testObject.readSerial();
        std::cout << thing << std::endl;
        if (thing.find("Autobots! Transform and roll out") != std::string::npos)
        {
            std::cout << "found!" << '\n';
            got_packet = true;
        }

        i++;
        // std::cout << "Serial Read: " << testObject.readSerial() << std::endl;
    }
    std::string command;
    std::string rafco_command = "C3 D4 C3 D4 E5 C3";
    std::stringstream rafco_stream(rafco_command);
    bool is_gray = false;
    bool is_blur = false;
    bool is_rotate = false;
    int pic_num = 1;
    while (rafco_stream >> command)
    { // Extract word from the stream.
        if (command == "C3")
        {
            std::cout << command << std::endl;
            bool got_packet = false;
            while (!got_packet)
            {
                // std::cout << "I: " << i << std::endl;
                testObject.writeSerial("C3");
                std::string thing = testObject.readSerial();
                std::cout << thing << std::endl;
                if (thing.find("C3") != std::string::npos)
                {
                    std::cout << "found!" << '\n';
                    got_packet = true;
                }

                i++;
                // std::cout << "Serial Read: " << testObject.readSerial() << std::endl;
            }
            bool status = false;
            std::string pic_name_str = "/home/vadl/Desktop/i" + std::to_string(pic_num) + ".JPG";
            while (!status)
            {

                status = testObject.readSerialImage(pic_name_str.c_str());
            }
            cv::Mat image = cv::imread(pic_name_str);
            if (is_gray)
            {
                cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
            }
            if (is_rotate)
            {
                cv::rotate(image, image, cv::ROTATE_180);
            }
            if (is_blur)
            {
                cv::GaussianBlur(image, image, cv::Size(15, 15), 1000);
            }
            cv::imwrite(pic_name_str, image);
            pic_num++;
        }
        else if (command == "D4")
        {
            is_gray = !is_gray;
        }
        else if (command == "E5")
        {
            is_rotate = !is_rotate;
        }
    }
    std::string thing = testObject.readSerial();
	std::cout << thing << std::endl;
    std::cout << "Reached the end" << std::endl;
    /*testObject.readSerialImage();
    i++;*/
    /*std::string
    size_t found_start = result.find(start_indicator);
    size_t found_photo = result.find(photo_indicator);
    size_t found_end = result.find(end_indicator);
    std::map<size_t, std::string> position_map;
    position_map.clear();
    bool has_start;
    bool has_photo;
    bool has_end;
    if (found_start != std::string::npos)
    {
        has_start = true;
        position_map.insert(std::pair<size_t, std::string>(found_start, start_indicator));
    }
    if (found_photo != std::string::npos)
    {
        has_photo = true;
        position_map.insert(std::pair<size_t, std::string>(found_photo, photo_indicator));
    }
    if (found_end != std::string::npos)
    {
        has_end = true;
        position_map.insert(std::pair<size_t, std::string>(found_end, end_indicator));
    }
    std::cout << "Result: " << result << std::endl;
    for (const auto &[key, value] : position_map)
    {
        if(value == "$START$") {
            photo_size = result.substr(key);
        } else if(value == "$PHOTO$") {
            if(has_start) {
                photo_size = photo_size.substr(start_indicator.length(),key - start_indicator.length());
            } else {
                photo_size += result.substr(start_indicator.length(),key);
                photo_size = photo_size.substr(start_indicator.length(),photo_size.length() - start_indicator.length());
            }
            photo = result.substr(key);
            std::cout << "Photo size: " << photo_size << std::endl;
            std::cout << "Photo length: " << photo_size.length() << std::endl;
        } else if(value == "$END$") {
            if(has_photo) {
                photo = photo.substr(photo_indicator.length(),key - photo_indicator.length());
            } else {
                photo += result.substr(photo_indicator.length(),key);
                photo = photo.substr(photo_indicator.length(),photo.length() - photo_indicator.length());
            }
            auto final_photo = photo.substr(0,photo.length() - end_indicator.length());
            std::cout << "THE PHOTO: " << photo.substr(0,photo.length() - end_indicator.length()) << std::endl;
            const char *file_hello = "/home/vadl/Desktop/pog5.png";
            FILE *f = fopen(file_hello,"w");
            if(f == NULL) {
                std::cout << "File opening pain" << std::endl;
            }
            char* received_photo = (char*) final_photo.c_str();
            std::stringstream size_stream(photo_size);
            size_t received_size;
            size_stream >> received_size;
            fwrite(received_photo,received_size,1,f);
            fclose(f);
        }
    }*/
    //}
    return 0;
}