#!/usr/bin/env python3
import serial
import time

if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
    ser.reset_input_buffer()

    while True:
        ser.write(bytes("Autobots, transform and roll out!\n",'utf-8'))
        try:
            line = ser.readline().decode('utf-8').rstrip()
            print(line)
        except:
            print("Pain")
        #time.sleep(1)