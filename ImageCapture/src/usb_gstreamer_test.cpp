#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/core/version.hpp>

int main()
{
#ifdef HAVE_OPENCV_GSTREAMER
    std::cout << "I GOT IT" << std::endl;
#else
    std::cout << "I DON'T GOT IT" << std::endl;
#endif
    cv::VideoCapture cap("v4l2src device=/dev/video0 ! video/x-raw,format=YUY2,width=640,height=480,framerate=30/1 ! videoconvert ! appsink", cv::CAP_GSTREAMER);
    if (!cap.isOpened())
    {
        std::cerr << "Failed to open camera" << std::endl;
        return -1;
    }
    cv::VideoWriter writer("appsrc ! videoconvert ! video/x-raw,format=BGR ! jpegenc ! avimux ! filesink location=test.avi", cv::CAP_GSTREAMER, 0, 30, cv::Size(640, 480));

    if (!writer.isOpened())
    {
        std::cerr << "Failed to open writer" << std::endl;
        return -1;
    }
    int i = 0;
    while (i < 1000)
    {
        cv::Mat frame;
        cap >> frame;
        writer.write(frame);
    }
    return 0;
}