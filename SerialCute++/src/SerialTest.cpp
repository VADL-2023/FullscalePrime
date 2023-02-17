#include <string>
#include <iostream>
#include <unistd.h>
#include "SerialObject.h"

int main() {
    SerialObject serial;

    // Prints to the Teensy
    serial.writeSerial("Hello, World!\n");

    std::cout << "Waiting for input" << std::endl;

    std::string str;
    do {
        sleep(1);
        //Read from the teensy
        str = serial.readSerial();
    } while(str.empty());

    std::cout << str << std::endl;

    return 0;
}