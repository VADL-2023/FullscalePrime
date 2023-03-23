#include "State_Camera_Check.h"
#include "Root.h"
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

State_Camera_Check::State_Camera_Check() : State()
{
}

State_Camera_Check::State_Camera_Check(StateName name, std::map<EventName, StateName> &state_transitions, Root *root) : State(name, state_transitions, root)
{
}

EventName State_Camera_Check::execute()
{
	system("sudo bash ../../cam_assignment.bash");
	std::cout << this->root_->m_log_.getTimestamp() << std::endl;
	bool cam1_online = false;
	bool cam2_online = false;
	bool cam3_online = false;
	for (int i = 0; i < this->root_->camera_streams_.size(); i++)
	{
		std::string camera_stream = this->root_->camera_streams_[i];
		int cam_result = cameraCheck(camera_stream);
		if(cam_result == 0) {
			if(i == 0) {
				cam1_online = true;
			}else if(i == 1) {
				cam2_online = true;
			} else if(i == 2) {
				cam3_online = true;
			}
		}
	}
	if(cam2_online) {
		this->root_->primary_camera_stream_ = this->root_->camera_streams_[1];
	} else if(cam3_online) {
		this->root_->primary_camera_stream_ = this->root_->camera_streams_[2];
	} else if(cam1_online) {
		this->root_->primary_camera_stream_ = this->root_->camera_streams_[0];
	} else {
		std::cout << "No cameras work" << std::endl;
		this->root_->primary_camera_stream_ = this->root_->camera_streams_[1];

	}
	std::cout << "Primary camera: " << this->root_->primary_camera_stream_ << std::endl;
	return CAMERA_PICKED;
}

EventName State_Camera_Check::unitExecute()
{
	return this->execute();
}

int State_Camera_Check::cameraCheck(std::string camera_stream)
{
	cv::Mat frame;
	//--- INITIALIZE VIDEOCAPTURE
	cv::VideoCapture cap;
	// open the default camera using default API
	// cap.open(0);
	// OR advance usage: select any API backend
	int deviceID = 100;		 // 0 = open default camera
	int apiID = cv::CAP_ANY; // 0 = autodetect default API
	bool isGray = false;
	bool isRotated = false;
	bool isBlurred = false;
	int numPics = 0;
	// open selected camera using selected API
	cap.open(camera_stream, apiID);
	// check if we succeeded
	if (!cap.isOpened())
	{
		std::cerr << "ERROR! Unable to open camera\n";
		return -1;
	}
	std::cout << "Opened camera " << camera_stream << std::endl;
	// wait for a new frame from camera and store it into 'frame'
	cap.read(frame);
	// check if we succeeded
	if (frame.empty())
	{
		std::cerr << "ERROR! blank frame grabbed\n";
		return -1;
	}
	return 0;
}