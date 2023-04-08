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

// Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file.
std::vector<VideoWriter> videos1;

std::vector<std::thread> threads;

int max_flight_time = 10 * 60; // seconds
int vid_clip_time = 3;         // seconds
int fps = 30;
size_t n_photo_bit_size = 6;

std::string cam1_str = "cam1";

int frames_per_vid = fps * vid_clip_time;

bool is_done = false;

void theCaptureThread(VideoCapture* cap, std::vector<VideoWriter>* videos)
{
  int i = 0;
  int index = 0;
  while (!is_done)
  {
    Mat frame;
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
    i++;
  }
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

  // Default resolutions of the frame are obtained.The default resolutions are system dependent.
  int frame_width = cap1.get(cv::CAP_PROP_FRAME_WIDTH);
  int frame_height = cap1.get(cv::CAP_PROP_FRAME_HEIGHT);

  mkdir(cam1_str.c_str(), 0777);

  for (int i = 0; i < max_flight_time / vid_clip_time; i++)
  {
    std::string num_string = std::to_string(i);
    int precision = n_photo_bit_size - std::min(n_photo_bit_size, num_string.size());
    num_string.insert(0, precision, '0');
    std::string video1_name = cam1_str + "/i" + num_string + ".avi";
    VideoWriter video1(video1_name, cv::VideoWriter::fourcc('X', 'V', 'I', 'D'), fps, Size(frame_width, frame_height));
    videos1.emplace_back(video1);
  }

  std::thread t1(theCaptureThread, &cap1,&videos1);
  threads.push_back(move(t1));
  auto start_time = getCurrentTime();
  auto current_time = getCurrentTime();
  while(current_time - start_time < 10000) {
    current_time = getCurrentTime();
  }
  is_done = true;
  for(int i = 0; i < threads.size();i++) {
    threads[i].join();
  }
  /*while (i < 20000)
  {
    Mat frame;
    cap1 >> frame;
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
    double current_time = getCurrentTime();
    double the_time = (current_time - start_time) / 1000.0;
    index = i / frames_per_vid;
    if (index >= max_flight_time / vid_clip_time)
    {
      index = (max_flight_time / vid_clip_time) - 1;
    }
    videos1[index].write(frame);
    std::cout << "I: " << i << std::endl;
    i++;
  }
  for (int i = 0; i < max_flight_time / vid_clip_time; i++)
  {
    videos1[i].release();
  }
  cap1.release();*/
  std::cout << "Done" << std::endl;
  // VideoWriter video("optimus.avi", cv::VideoWriter::fourcc('X','V','I','D'), 30, Size(frame_width,frame_height));
  /*auto end = getCurrentTime();
  std::cout << "Video writer time: " << end - start << std::endl;
 int i = 0;
  while(i < 1000){

    Mat frame;

    // Capture frame-by-frame
    cap >> frame;

    // If the frame is empty, break immediately
    if (frame.empty())
      break;

    auto end = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        std::string date_time = std::ctime(&end_time);

        cv::Size text_size = cv::getTextSize(date_time, cv::FONT_HERSHEY_COMPLEX, 1, 4, 0);
        cv::Point the_org(0, text_size.height);
        cv::Scalar color1(0, 0, 0);
        cv::Scalar color2(255, 255, 255);
        cv::putText(frame, date_time, the_org, cv::FONT_HERSHEY_COMPLEX, 1, color1, 4, cv::LINE_AA);
        cv::putText(frame, date_time, the_org, cv::FONT_HERSHEY_COMPLEX, 1, color2, 2, cv::LINE_AA);
    // Write the frame into the file 'outcpp.avi'
    video.write(frame);

    std::cout << "I: " << i << std::endl;
    i++;
  }

  // When everything done, release the video capture and write object
  cap.release();
  video.release();*/

  return 0;
}