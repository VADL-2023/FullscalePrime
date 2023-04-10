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
double getCurrentTime()
{
    return double(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
}
/*int main()
{
    cv::VideoCapture cap("/dev/videoCam1");

    if (!cap.isOpened())
    {
        std::cout << "Error opening video stream" << std::endl;
        return -1;
    }
    // Define the codec and create VideoWriter object
    int fourcc = cv::VideoWriter::fourcc('X', 'V', 'I', 'D');
    int fps = 30;
    int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    std::cout << "Test 1" << std::endl;
    cv::VideoWriter writer("output_001.avi", fourcc, fps, cv::Size(frame_width, frame_height));
    std::cout << "Test 2" << std::endl;
    // Open the camera

    int i = 0;
    while (i < 1000)
    {
        // Read a frame from the camera
        cv::Mat frame;
        cap >> frame;

        if (frame.empty())
            break;


        // Write the flipped frame to the output video file
        writer.write(frame);
        i++;
    }

    // Release everything if job is finished
    cap.release();
    writer.release();

    return 0;
}*/

using namespace std;
using namespace cv;

// Create a VideoCapture object and use camera to capture the video
VideoCapture cap1;
VideoCapture cap2;
VideoCapture cap3;

// Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file.
std::vector<VideoWriter> videos1;
std::vector<VideoWriter> videos2;
std::vector<VideoWriter> videos3;

std::vector<std::thread> threads;

int max_flight_time = 3 * 60; // seconds
int vid_clip_time = 3;   // seconds
int fps = 24;
size_t n_photo_bit_size = 6;

std::string cam1_str = "cam1";
std::string cam2_str = "cam2";
std::string cam3_str = "cam3";

int frames_per_vid = fps * vid_clip_time;

bool is_done = false;

void picThread(VideoCapture *cap, int cam_number)
{
    std::vector<cv::Mat> frames;
    std::vector<std::string> date_times;
    while (!is_done)
    {
        cv::Mat frame;

        (*cap).read(frame);
        if (frame.empty())
        {
            break;
        }
        auto end = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        std::string date_time = std::ctime(&end_time);
        date_time.pop_back();
        date_times.push_back(date_time);
        frames.push_back(frame);
    }
    for (int i = 0; i < frames.size(); i++)
    {
        std::string rcb_num_string = std::to_string(i);
        int precision = n_photo_bit_size - std::min(n_photo_bit_size, rcb_num_string.size());
        rcb_num_string.insert(0, precision, '0');
        std::string pic_name_str = "i" + rcb_num_string + ".png";
        cv::Size text_size = cv::getTextSize(date_times[i], cv::FONT_HERSHEY_COMPLEX, 1, 4, 0);
        cv::Point the_org(0, text_size.height);
        cv::Scalar color1(0, 0, 0);
        cv::Scalar color2(255, 255, 255);
        auto the_frame = frames[i];
        cv::putText(the_frame, date_times[i], the_org, cv::FONT_HERSHEY_COMPLEX, 1, color1, 4, cv::LINE_AA);
        cv::putText(the_frame, date_times[i], the_org, cv::FONT_HERSHEY_COMPLEX, 1, color2, 2, cv::LINE_AA);
        cv::imwrite(pic_name_str, the_frame);
    }
}

void theCaptureThread(VideoCapture *cap, std::vector<VideoWriter> *videos, int cam_number)
{
    bool is_first = true;
    int i = 0;
    int index = 0;
    double max_time = 0;
    double min_time = 10000000;
    while (!is_done)
    {
        Mat frame;
        auto start_time = getCurrentTime();
        (*cap) >> frame;
        if (frame.empty())
        {
            break;
        }
        auto end = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        std::string date_time = std::ctime(&end_time);

        cv::Size text_size = cv::getTextSize(date_time, cv::FONT_HERSHEY_COMPLEX, 1, 4, 0);
        cv::Point the_org(0, text_size.height);
        cv::Scalar color1(0, 0, 0);
        cv::Scalar color2(255, 255, 255);
        cv::putText(frame, date_time, the_org, cv::FONT_HERSHEY_COMPLEX, 1, color1, 4, cv::LINE_AA);
        cv::putText(frame, date_time, the_org, cv::FONT_HERSHEY_COMPLEX, 1, color2, 2, cv::LINE_AA);
        index = i / frames_per_vid;
        if (index >= max_flight_time / vid_clip_time)
        {
            index = (max_flight_time / vid_clip_time) - 1;
        }
        (*videos)[index].write(frame);
        auto write_time = getCurrentTime();
        /*if(cam_number == 2) {
          std::cout << "Time: " << write_time - start_time << std::endl;
        }*/
        if (!is_first && write_time - start_time > max_time)
        {
            max_time = write_time - start_time;
        }
        if (!is_first && write_time - start_time < min_time)
        {
            min_time = write_time - start_time;
        }
        is_first = false;
        i++;
    }
    std::cout << "Thread " << cam_number << ": Max time: " << max_time << std::endl;
    std::cout << "Thread " << cam_number << ": Min time: " << min_time << std::endl;
    for (int i = 0; i < max_flight_time / vid_clip_time; i++)
    {
        (*videos)[i].release();
    }
    cap->release();
}

int main()
{

    // Check if camera opened successfully
    if (!cap1.isOpened())
    {
        cap1.open("/dev/videoCam1");
        if (!cap1.isOpened())
        {
            cout << "Error opening video 1 stream" << endl;
            return -1;
        }
    }

    if (!cap2.isOpened())
    {
        cap2.open("/dev/videoCam2");
        if (!cap2.isOpened())
        {
            cout << "Error opening video 2 stream" << endl;
            return -1;
        }
    }

    if (!cap3.isOpened())
    {
        cap3.open("/dev/videoCam3");
        if (!cap3.isOpened())
        {
            cout << "Error opening video 3 stream" << endl;
            return -1;
        }
    }
    std::cout << "CV Version: " << CV_VERSION << std::endl;
    // Default resolutions of the frame are obtained.The default resolutions are system dependent.
    int frame_width = cap1.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = cap1.get(cv::CAP_PROP_FRAME_HEIGHT);
    std::cout << "Frame width: " << frame_width << std::endl;
    std::cout << "Frame height: " << frame_height << std::endl;
    cap1.set(CAP_PROP_FPS, fps);
    cap2.set(CAP_PROP_FPS, fps);
    cap3.set(CAP_PROP_FPS, fps);

    mkdir(cam1_str.c_str(), 0777);
    mkdir(cam2_str.c_str(), 0777);
    mkdir(cam3_str.c_str(), 0777);

    for (int i = 0; i < max_flight_time / vid_clip_time; i++)
    {
        std::string num_string = std::to_string(i);
        int precision = n_photo_bit_size - std::min(n_photo_bit_size, num_string.size());
        num_string.insert(0, precision, '0');
        std::string video1_name = cam1_str + "/i" + num_string + ".avi";
        std::string video2_name = cam2_str + "/i" + num_string + ".avi";
        std::string video3_name = cam3_str + "/i" + num_string + ".avi";

        VideoWriter video1(video1_name, cv::VideoWriter::fourcc('X', 'V', 'I', 'D'), fps, Size(frame_width, frame_height));
        videos1.emplace_back(video1);

        VideoWriter video2(video2_name, cv::VideoWriter::fourcc('X', 'V', 'I', 'D'), fps, Size(frame_width, frame_height));
        videos2.emplace_back(video2);

        VideoWriter video3(video3_name, cv::VideoWriter::fourcc('X', 'V', 'I', 'D'), fps, Size(frame_width, frame_height));
        videos3.emplace_back(video3);
    }

    std::thread t1(theCaptureThread, &cap1, &videos1, 1);
    //std::thread t1(picThread,&cap1,1);
    threads.push_back(move(t1));
    std::thread t2(theCaptureThread, &cap2, &videos2,2);
    threads.push_back(move(t2));
    //std::thread t3(theCaptureThread, &cap3, &videos3,3);
    //threads.push_back(move(t3));
    auto start_time = getCurrentTime();
    auto current_time = getCurrentTime();
    while (current_time - start_time < 5 * 60 * 1000)
    {
        current_time = getCurrentTime();
    }
    is_done = true;
    for (int i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }

    return 0;
}