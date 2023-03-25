#include "State_Apogee_Detection.h"
#include "Root.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

State_Apogee_Detection::State_Apogee_Detection() : State()
{
}

State_Apogee_Detection::State_Apogee_Detection(StateName name, std::map<EventName, StateName> &state_transitions, Root *root) : State(name, state_transitions, root)
{
}

EventName State_Apogee_Detection::execute()
{

	float max_altitude = 0;
	int samples_since_max_has_changed = 0;

	this->root_->m_log_.write("Looking for Apogee");

	// apogee detected when a new max altitude has not been achieved for this->root_->num_data_points_checked_4_apogee_
	while ((!this->root_->time_delay_enabled_ && samples_since_max_has_changed < this->root_->num_data_points_checked_4_apogee_) && !this->root_->isTimeExceeded(this->root_->launch_time_, this->root_->max_flight_time_))
	{
		try
		{
			this->root_->response_ = this->root_->m_vn_->readImuMeasurements();
			this->root_->m_log_.write(this->root_->response_);
			this->root_->z_current_ = this->root_->pressure2Altitude(this->root_->t0_, this->root_->p0_, this->root_->g0_, this->root_->response_.pressure);

			if (this->root_->z_current_ >= max_altitude)
			{
				max_altitude = this->root_->z_current_;
				samples_since_max_has_changed = 0;
			}
			else
			{
				++samples_since_max_has_changed;
			}
			cv::Mat frame1;
			cv::Mat frame2;
			auto the_thing1 = this->root_->cap1.read(frame1);
			auto the_thing2 = this->root_->cap2.read(frame2);
			if (frame1.empty())
			{
				std::cerr << "ERROR! blank frame1 grabbed\n";
			}
			else
			{
				cv::Mat display_frame1 = frame1;
				// Need to rotate frame1 because of how camera is mounted
				cv::rotate(display_frame1, display_frame1, cv::ROTATE_180);
				std::string folder_name_str = "SecondaryPayloadImages" + this->root_->m_log_.getTimestamp();
				mkdir(folder_name_str.c_str(), 0777);
				std::string cam1_str = folder_name_str + "/cam1";
				mkdir(cam1_str.c_str(),0777);
				std::string aac_num_string = std::to_string(this->root_->aac_pic_num_);
				int precision = this->root_->n_photo_bit_size_ - std::min(this->root_->n_photo_bit_size_,aac_num_string.size());
				aac_num_string.insert(0,precision,'0');
				std::string pic_name_str = cam1_str + "/i" + aac_num_string + ".png";
				cv::imwrite(pic_name_str, display_frame1);
				this->root_->aac_pic_num_++;
			}
			if (frame2.empty())
			{
				std::cerr << "ERROR! blank frame2 grabbed\n";
			}
			else
			{
				cv::Mat display_frame2 = frame2;
				// Need to rotate frame2 because of how camera is mounted
				cv::rotate(display_frame2, display_frame2, cv::ROTATE_180);
				std::string folder_name_str = "SecondaryPayloadImages" + this->root_->m_log_.getTimestamp();
				mkdir(folder_name_str.c_str(), 0777);
				std::string cam2_str = folder_name_str + "/cam2";
				mkdir(cam2_str.c_str(),0777);
				std::string aac_num_string = std::to_string(this->root_->aac_pic_num_);
				int precision = this->root_->n_photo_bit_size_ - std::min(this->root_->n_photo_bit_size_,aac_num_string.size());
				aac_num_string.insert(0,precision,'0');
				std::string pic_name_str = cam2_str + "/i" + aac_num_string + ".png";
				cv::imwrite(pic_name_str, display_frame2);
			}
		}
		catch (const std::exception &e)
		{
			this->root_->m_log_.write("Exception: ");
			this->root_->m_log_.write(e.what());
			this->root_->m_log_.write("IMU disconnected while waiting for apogee");
			this->root_->m_log_.write("Delaying for " + std::to_string(this->root_->max_flight_time_ - ((this->root_->getCurrentTime() - this->root_->launch_time_) / 1000)) + "s");
			this->root_->time_delay_enabled_ = true;
		}
	}

	this->root_->m_log_.writeDelim("Apogee Detected");
	this->root_->m_log_.write("Altitude has not reached a new max for " + to_string(this->root_->num_data_points_checked_4_apogee_) + " samples");
	return APOGEE_DETECTED;
}

EventName State_Apogee_Detection::unitExecute()
{
	return this->execute();
}