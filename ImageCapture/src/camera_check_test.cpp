#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>
// using namespace cv;
// using namespace std;
bool cameraCheck(std::string camera_stream,cv::VideoCapture& cap)
{
    cv::Mat frame;
    //--- INITIALIZE VIDEOCAPTURE
    // open the default camera using default API
    // cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 100;      // 0 = open default camera
    int apiID = cv::CAP_ANY; // 0 = autodetect default API
    bool isGray = false;
    bool isRotated = false;
    bool isBlurred = false;
    int numPics = 0;
    std::cout << "Trying to open camera stream: " << camera_stream << std::endl;
    // open selected camera using selected API
    cap.open(camera_stream);
    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    // check if we succeeded
    if (!cap.isOpened())
    {
        std::cerr << "ERROR! Unable to open camera\n";
        return false;
    }
    std::cout << "Opened camera " << camera_stream << std::endl;
    // wait for a new frame from camera and store it into 'frame'
    cap.read(frame);
    // check if we succeeded
    if (frame.empty())
    {
        std::cerr << "ERROR! blank frame grabbed\n";
        return false;
    }
    return true;
}

int main(int, char **)
{
    cv::VideoCapture cap1;
    cv::VideoCapture cap2;
    cv::VideoCapture cap3;
    system("sudo bash ../../cam_assignment.bash");
    std::cout << "Cam 1: " << cameraCheck("/dev/videoCam1",cap1) << std::endl;
    std::cout << "Cam 2: " << cameraCheck("/dev/videoCam2",cap2) << std::endl;
    std::cout << "Cam 3: " << cameraCheck("/dev/videoCam3",cap3) << std::endl;

    cap1.open("/dev/videoCam1");
    cap1.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    if (!cap1.isOpened())
    {
        std::cerr << "ERROR! Unable to open camera 1" << std::endl;
    }
    else
    {
        for (int i = 0; i < 3; i++)
        {
            cv::Mat temp_frame;
            cap1.read(temp_frame);
        }
    }

    cap2.open("/dev/videoCam2");
    cap2.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    if (!cap2.isOpened())
    {
        std::cerr << "ERROR! Unable to open camera 2" << std::endl;
    }
    else
    {
        for (int i = 0; i < 3; i++)
        {
            cv::Mat temp_frame;
            cap2.read(temp_frame);
        }
    }

    cap3.open("/dev/videoCam3");
    cap3.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    if (!cap3.isOpened())
    {
        std::cerr << "ERROR! Unable to open camera 3" << std::endl;
    }
    else
    {
        for (int i = 0; i < 3; i++)
        {
            cv::Mat temp_frame;
            cap3.read(temp_frame);
        }
    }

    std::cout << "TAKE TWO" << std::endl;

    system("sudo bash ../../cam_assignment.bash");
    std::cout << "Cam 1: " << cameraCheck("/dev/videoCam1",cap1) << std::endl;
    std::cout << "Cam 2: " << cameraCheck("/dev/videoCam2",cap2) << std::endl;
    std::cout << "Cam 3: " << cameraCheck("/dev/videoCam3",cap3) << std::endl;

    cap1.open("/dev/videoCam1");
    cap1.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    if (!cap1.isOpened())
    {
        std::cerr << "ERROR! Unable to open camera 1" << std::endl;
    }
    else
    {
        for (int i = 0; i < 3; i++)
        {
            cv::Mat temp_frame;
            cap1.read(temp_frame);
        }
    }

    cap2.open("/dev/videoCam2");
    cap2.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    if (!cap2.isOpened())
    {
        std::cerr << "ERROR! Unable to open camera 2" << std::endl;
    }
    else
    {
        for (int i = 0; i < 3; i++)
        {
            cv::Mat temp_frame;
            cap2.read(temp_frame);
        }
    }

    cap3.open("/dev/videoCam3");
    cap3.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    if (!cap3.isOpened())
    {
        std::cerr << "ERROR! Unable to open camera 3" << std::endl;
    }
    else
    {
        for (int i = 0; i < 3; i++)
        {
            cv::Mat temp_frame;
            cap3.read(temp_frame);
        }
    }
    return 0;
}