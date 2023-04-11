#include "State_Landing_Detection.h"
#include "Root.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

State_Landing_Detection::State_Landing_Detection() : State()
{
}

State_Landing_Detection::State_Landing_Detection(StateName name, std::map<EventName, StateName> &state_transitions, Root *root) : State(name, state_transitions, root)
{
}

EventName State_Landing_Detection::execute()
{
	float min_altitude = 1000000;
	int samples_since_min_has_changed = 0;

	// IMU FREQUENCY TEST
	double start_measurement_time = 0;
	double end_measurement_time = 0;
	double sample_num = 0;
	bool got_start_time = false;

	bool landing_detected = false;

	this->root_->m_log_.write("Waiting for landing detection");

	// landing detected when a new min altitude has not been achieved for this->root_->num_data_points_checked_4_landing_
	// and the current altitude is within the specified threshold
	// Landing detection also occurs if the current time exceeds the maximum allowable flight time
	double my_start_time = this->root_->getCurrentTime();
	double my_end_time = my_start_time;
	int measure_count = 0;
	//while(this->root_->getCurrentTime() - my_start_time < 180000)
	while (!this->root_->time_delay_enabled_ && !((samples_since_min_has_changed >= this->root_->num_data_points_checked_4_landing_) && (abs(this->root_->z_current_) < this->root_->z_threshold_for_landing_)) && !this->root_->isTimeExceeded(this->root_->launch_time_, this->root_->max_flight_time_))
	{
		try
		{
			this->root_->response_ = this->root_->m_vn_->readImuMeasurements();
			measure_count++;
			sample_num++;
			if (!got_start_time)
			{
				start_measurement_time = this->root_->getCurrentTime() - this->root_->start_time_;
				got_start_time = true;
			}
			this->root_->m_log_.write(this->root_->response_);
			this->root_->z_current_ = this->root_->pressure2Altitude(this->root_->t0_, this->root_->p0_, this->root_->g0_, this->root_->response_.pressure);

			if (this->root_->z_current_ < min_altitude)
			{
				min_altitude = this->root_->z_current_;
				samples_since_min_has_changed = 0;
			}
			else
			{
				++samples_since_min_has_changed;
			}

			this->root_->landing_time_ = this->root_->getCurrentTime();
		}
		catch (const std::exception &e)
		{
			this->root_->m_log_.write("Exception: ");
			this->root_->m_log_.write(e.what());
			this->root_->m_log_.write("IMU disconnected while waiting for landing");
			this->root_->m_log_.write("Delaying for " + std::to_string(this->root_->max_flight_time_ - ((this->root_->getCurrentTime() - this->root_->launch_time_) / 1000)) + "s");
			this->root_->time_delay_enabled_ = true;
		}
	}

	if (this->root_->time_delay_enabled_)
	{
		this->root_->m_log_.write("Delaying for " + std::to_string(this->root_->max_flight_time_ - ((this->root_->getCurrentTime() - this->root_->launch_time_) / 1000)) + "s");
		while (!this->root_->isTimeExceeded(this->root_->launch_time_, this->root_->max_flight_time_))
		{
			usleep(100);
		}
	}

	this->root_->m_log_.writeDelim("Landing Detected");
	my_end_time = this->root_->getCurrentTime();
	this->root_->landing_detected_ = true;
	for(int i = 0;i < this->root_->threads_.size();i++) {
        this->root_->threads_[i].join();
    }
	std::cout << "Made it" << std::endl;
	if ((samples_since_min_has_changed >= this->root_->num_data_points_checked_4_landing_) && (abs(this->root_->z_current_) < this->root_->z_threshold_for_landing_))
	{
		this->root_->m_log_.write("Altitude has not reached a new min for " + to_string(this->root_->num_data_points_checked_4_landing_) + " samples");
		this->root_->m_log_.write("Altitude was within threshold of " + std::to_string(this->root_->z_threshold_for_landing_) + " meters");
	}
	else
	{
		this->root_->m_log_.write("Time exceeded max flight limit of " + std::to_string(this->root_->max_flight_time_) + " s");
	}
	end_measurement_time = this->root_->getCurrentTime() - this->root_->start_time_;
	
	/*std::cout << "Start Time: " << start_measurement_time << std::endl;
	std::cout << "End time: " << end_measurement_time << std::endl;
	std::cout << "Num samples: " << sample_num << std::endl;
	std::cout << "Frequency: " << sample_num / ((end_measurement_time/1000) - (start_measurement_time/1000)) << std::endl;*/
	// Terminate IMU
	if (this->root_->terminateConnections(this->root_->m_vn_))
	{
		this->root_->m_log_.writeDelim("IMU: Disconnected");
	}
	delete this->root_->m_vn_;
	this->root_->is_imu_connected_ = false;

	this->root_->cap1.release();
	this->root_->cap2.release();
	this->root_->cap3.release();
	return LANDING_DETECTED;
}

EventName State_Landing_Detection::unitExecute()
{
	return this->execute();
}