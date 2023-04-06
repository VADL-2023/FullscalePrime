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
	int capture_size = this->root_->aac_camera_captures_.size();
	if (capture_size > 2)
	{
		capture_size = 2;
	}
	for (int i = 0; i < capture_size; i++)
	{
		int max_photos = 900;
		if (i == 0)
		{
			max_photos = 1500;
		}
		if (this->root_->aac_camera_streams_[i] == "/dev/videoCam1")
		{
			//Only need it for camera 1 for the scenario where just camera 3 is plugged in
			//If no cameras are plugged in, we will just try for 2 and 3
			cv::Mat frame;
				this->root_->aac_camera_captures_[i].read(frame);
			if(frame.empty()) {
				this->root_->m_log_.write("Camera 1 disconnected");
				capture_size++;
			} else {
				std::thread t1(&Root::camThreadLanding, this->root_, &this->root_->aac_camera_captures_[i], 1, max_photos);
				this->root_->threads_.push_back(move(t1));
			}

		}
		else if (this->root_->aac_camera_streams_[i] == "/dev/videoCam3")
		{
			std::thread t2(&Root::camThreadLanding, this->root_, &this->root_->aac_camera_captures_[i], 3, max_photos);
			this->root_->threads_.push_back(move(t2));
		}
		else if (this->root_->aac_camera_streams_[i] == "/dev/videoCam2")
		{
			std::thread t3(&Root::camThreadLanding, this->root_, &this->root_->aac_camera_captures_[i], 2, max_photos);
			this->root_->threads_.push_back(move(t3));
		}
	}
	std::string aac_num_str = "   Number of AAC Videos: " + this->root_->threads_.size();
	this->root_->m_log_.write(aac_num_str);

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