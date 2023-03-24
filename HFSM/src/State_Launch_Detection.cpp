#include "State_Launch_Detection.h"
#include "Root.h"

State_Launch_Detection::State_Launch_Detection() : State()
{
}

State_Launch_Detection::State_Launch_Detection(StateName name, std::map<EventName, StateName> &state_transitions, Root *root) : State(name, state_transitions, root)
{
}

EventName State_Launch_Detection::execute()
{
	this->root_->m_log_.write("Ready for Assembly and Launch Rail");

	float accelArray[this->root_->num_data_points_checked_4_launch_] = {0};
	float accelAvg = 0;
	int counter = 0;
	system("sudo bash ../../cam_assignment.bash");
	for (int i = 0; i < this->root_->camera_streams_.size(); i++)
	{
		std::string camera_stream = this->root_->camera_streams_[i];
		int cam_result = this->root_->cameraCheck(camera_stream);
		this->root_->aac_camera_streams_.push_back(camera_stream);
	}
	for(int i = 0; i < this->root_->aac_camera_streams_.size(); ++i) {
		std::cout << this->root_->aac_camera_streams_[i] << std::endl;
	}
	
	// open the default camera using default API
	// cap.open(0);
	// OR advance usage: select any API backend
	int apiID = cv::CAP_ANY; // 0 = autodetect default API
	int numPics = 0;
	// open selected camera using selected API
	this->root_->cap1.open(this->root_->aac_camera_streams_[0], apiID);
	// check if we succeeded
	if (!this->root_->cap1.isOpened())
	{
		std::cerr << "ERROR! Unable to open camera\n";
	}
	// launch detected when avg accel exceeds accelRoof*g0 for numDataPointsChecked4Launch
	while (accelAvg < this->root_->accel_roof_ * this->root_->g0_)
	{
		try
		{
			this->root_->response_ = this->root_->m_vn_->readImuMeasurements();
			this->root_->m_log_.write(this->root_->response_);
		}
		catch (const std::exception &e)
		{
			this->root_->m_log_.write("Exception: ");
			this->root_->m_log_.write(e.what());
			this->root_->m_log_.write("IMU disconnected while waiting for launch");
			this->root_->m_log_.writeTime("Ending Program");
			this->root_->terminateConnections(this->root_->m_vn_);
			return TERMINATE;
		}

		accelArray[counter % this->root_->num_data_points_checked_4_launch_] = sqrt(pow(this->root_->response_.accel[0], 2) + pow(this->root_->response_.accel[1], 2) + pow(this->root_->response_.accel[2], 2)); // total accel vector
		accelAvg = this->root_->calcArrayAverage(accelArray, this->root_->num_data_points_checked_4_launch_);
		++counter;
	}

	this->root_->m_log_.write("Average acceleration exceeded " + to_string(this->root_->accel_roof_ * this->root_->g0_) + " m/s^2 over " + std::to_string(this->root_->num_data_points_checked_4_launch_) + " data points");
	this->root_->m_log_.writeDelim("Rocket Has Launched");
	this->root_->m_log_.write("Waiting for motor burn time");

	this->root_->launch_time_ = this->root_->getCurrentTime();

	// Wait for motor burn to complete while still collecting IMU data
	this->root_->activeSleep(this->root_->t_burn_, this->root_->m_vn_, this->root_->response_, this->root_->m_log_, this->root_->start_time_);
	return LAUNCH_DETECTED;
}

EventName State_Launch_Detection::unitExecute()
{
	return this->execute();
}