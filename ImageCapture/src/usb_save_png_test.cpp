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
    system("sudo bash ../../cam_assignment.bash");
    cv::Mat frame1;
    cv::Mat frame2;
    cv::Mat frame3;
    //--- INITIALIZE VIDEOCAPTURE
    cv::VideoCapture cap1;
    cv::VideoCapture cap2;
    cv::VideoCapture cap3;
    // open the default camera using default API
    // cap.open(0);
    // OR advance usage: select any API backend
    int apiID = cv::CAP_ANY; // 0 = autodetect default API
    bool isGray = false;
    bool isRotated = false;
    bool isBlurred = false;
    int numPics = 0;
    // open selected camera using selected API
    cap1.open("/dev/videoCam1");
    cap1.set(cv::CAP_PROP_FOURCC,cv::VideoWriter::fourcc('M','J','P','G'));
    cap2.open("/dev/videoCam2");
    cap2.set(cv::CAP_PROP_FOURCC,cv::VideoWriter::fourcc('M','J','P','G'));
    cap3.open("/dev/videoCam3");
    cap3.set(cv::CAP_PROP_FOURCC,cv::VideoWriter::fourcc('M','J','P','G'));
    // check if we succeeded
    if (!cap1.isOpened())
    {
        std::cerr << "ERROR! Unable to open camera 1\n";
        return -1;
    }
    if (!cap2.isOpened())
    {
        std::cerr << "ERROR! Unable to open camera 2\n";
        return -1;
    }
    if (!cap3.isOpened())
    {
        std::cerr << "ERROR! Unable to open camera 3\n";
        return -1;
    }
    //--- GRAB AND WRITE LOOP
    std::cout << "Start grabbing" << std::endl
         << "Press any key to terminate" << std::endl;
    for (int i = 0;i < 10;i++)
    {
        // wait for a new frame from camera and store it into 'frame'
        cap1.read(frame1);
        cap2.read(frame2);
        cap3.read(frame3);
        // check if we succeeded
        if (frame1.empty())
        {
            std::cerr << "ERROR! blank frame 1 grabbed\n";
            break;
        }
        if (frame2.empty())
        {
            std::cerr << "ERROR! blank frame 2 grabbed\n";
            break;
        }
        if (frame3.empty()) {
            std::cerr << "ERROR! blank frame 3 grabbed \n";
            break;
        }
        cv::imwrite("test.png",frame1);
        cv::imwrite("test2.png",frame2);
        cv::imwrite("test3.png",frame3);
        
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}