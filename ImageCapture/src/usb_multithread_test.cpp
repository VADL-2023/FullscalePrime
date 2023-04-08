#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <chrono>
#include <ctime>
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
 
int main(){
 
  // Create a VideoCapture object and use camera to capture the video
  VideoCapture cap(0); 
 
  // Check if camera opened successfully
  if(!cap.isOpened()){
    cout << "Error opening video stream" << endl;
        return -1;
  }
   
  // Default resolutions of the frame are obtained.The default resolutions are system dependent.
  int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
  int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
   
  // Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file.
  VideoWriter video("outcpp.avi", cv::VideoWriter::fourcc('X','V','I','D'), 30, Size(frame_width,frame_height));
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
  video.release();
 
  return 0;
}