#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>
//using namespace cv;
//using namespace std;
int main(int, char **)
{
    cv::Mat frame;
    //--- INITIALIZE VIDEOCAPTURE
    cv::VideoCapture cap;
    // open the default camera using default API
    // cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 100;        // 0 = open default camera
    int apiID = cv::CAP_ANY; // 0 = autodetect default API
    bool isGray = false;
    bool isRotated = false;
    bool isBlurred = false;
    int numPics = 0;
    // open selected camera using selected API
    cap.open("/dev/videoRumble", apiID);
    // check if we succeeded
    if (!cap.isOpened())
    {
        std::cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    //--- GRAB AND WRITE LOOP
    std::cout << "Start grabbing" << std::endl
         << "Press any key to terminate" << std::endl;
    for (;;)
    {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);
        // check if we succeeded
        if (frame.empty())
        {
            std::cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        // show live and wait for a key with timeout long enough to show images
        cv::Mat displayFrame;
        cv::cvtColor(frame, displayFrame, cv::COLOR_BGR2RGB);
        auto key = cv::waitKey(1);
        if (key == 27)
        {
            break;
        }
        else if (key == 103)
        {
            isGray = !isGray;
        }
        else if (key == 98)
        {
            isBlurred = !isBlurred;
        }
        else if (key == 114)
        {
            isRotated = !isRotated;
        }

        if (isGray)
        {
            cv::cvtColor(frame, displayFrame, cv::COLOR_RGB2GRAY);
        }
        if (isRotated)
        {
            cv::rotate(displayFrame, displayFrame, cv::ROTATE_180);
        }
        if (isBlurred)
        {
            cv::GaussianBlur(displayFrame, displayFrame, cv::Size(15, 15), 1000);
        }
        cv::namedWindow("Live", cv::WINDOW_NORMAL);
        cv::resizeWindow("Live", 1920, 1080);
        cv::imshow("Live", displayFrame);
        if (key == 99)
        {
            std::string fileName = "/home/vadl/Desktop/pic" + std::to_string(numPics) + ".jpg";
            cv::imwrite(fileName, displayFrame);
            numPics++;
        }
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}