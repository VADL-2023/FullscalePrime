#include "State_Full_Lift.h"
#include "Root.h"
#include <unistd.h> // sleep function

State_Full_Lift::State_Full_Lift() : State()
{
}

State_Full_Lift::State_Full_Lift(StateName name, std::map<EventName, StateName> &state_transitions, Root *root) : State(name, state_transitions, root)
{
}

EventName State_Full_Lift::execute()
{
	this->root_->m_log_.write("In State full lift");
	this->root_->m_log_.write("Initiate lift servo unlock");
	gpioServo(this->root_->lift_servo_, this->root_->lift_unlock_);
	gpioSleep(0, 2, 0);
	this->root_->m_log_.write("Initiate lift servo lock");
	bool is_done = false;
	bool is_time_up = false;
	double start_time = this->root_->getCurrentTime();
	double current_time = start_time;
	this->root_->m_log_.write("Starting winch");
	gpioWrite(this->root_->rcb_lift_standby_, 1);

	// open the default camera using default API
	// this->root_->lift_and_level_cap_.open(0);
	// OR advance usage: select any API backend
	int numPics = 0;
	// open selected camera using selected API

	if (this->root_->primary_camera_stream_ != "")
	{
		// open selected camera using selected API
		if (!this->root_->lift_and_level_cap_.isOpened())
		{
			this->root_->lift_and_level_cap_.open(this->root_->primary_camera_stream_);
		}
		// check if we succeeded
		if (!this->root_->lift_and_level_cap_.isOpened())
		{
			this->root_->m_log_.write("ERROR! Unable to open camera " + this->root_->primary_camera_stream_);
		}
		if (this->root_->date_timestamp_ == "")
		{
			auto end = std::chrono::system_clock::now();

			std::time_t end_time = std::chrono::system_clock::to_time_t(end);
			this->root_->date_timestamp_ = std::ctime(&end_time);
		}
		std::string base_folder = this->root_->date_timestamp_;
		std::replace(base_folder.begin(), base_folder.end(), ' ', '_');
		if (!base_folder.empty())
		{
			base_folder.pop_back();
		}
		else
		{
			std::cout << "Invalid date/time format" << std::endl;
		}

		mkdir(base_folder.c_str(), 0777);
		std::string folder_name_str = base_folder + "/Lift" + this->root_->m_log_.getTimestamp();
		std::cout << "Lift Video Folder: " << folder_name_str << std::endl;
		mkdir(folder_name_str.c_str(), 0777);
		std::string lift_and_level_video_name = folder_name_str + "/lift.avi";
		this->root_->lift_and_level_video_.open(lift_and_level_video_name, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), this->root_->fps_, cv::Size(this->root_->frame_width_, this->root_->frame_height_));
		std::thread t1(&Root::realCamThreadLift, this->root_, &this->root_->lift_and_level_cap_, &this->root_->lift_and_level_video_);
		this->root_->lift_thread_ = std::move(t1);
	}
	while (!is_done)
	{
		is_done = !gpioRead(this->root_->lift_final_limit_switch_);
		current_time = this->root_->getCurrentTime();
		if (current_time - start_time > this->root_->lift_time_threshold_)
		{
			is_time_up = true;
			is_done = true;
		}
		if (current_time - start_time < this->root_->lift_min_threshold_)
		{
			is_done = false;
		}
		gpioWrite(this->root_->lift_p_, 1);
		gpioWrite(this->root_->lift_n_, 0);
		gpioPWM(this->root_->lift_enable_, this->root_->pwm_motor_max_);
	}
	// Turn off lift
	gpioWrite(this->root_->lift_p_, 0);
	gpioWrite(this->root_->lift_n_, 0);
	gpioPWM(this->root_->lift_enable_, 0);
	usleep(10000); // TODO: Does this need to be gpioSleep()?
	double lift_time_ms = current_time - start_time;
	this->root_->m_log_.write("Time for lift " + std::to_string(lift_time_ms));

	double backwards_time_start = this->root_->getCurrentTime();
	double backwards_time_current = this->root_->getCurrentTime();
	// Run lift backwards
	if (current_time - start_time > this->root_->lift_min_threshold_)
	{
		while (backwards_time_current - backwards_time_start < this->root_->lift_backwards_time_threshold_)
		{
			gpioWrite(this->root_->lift_p_, 0);
			gpioWrite(this->root_->lift_n_, 1);
			gpioPWM(this->root_->lift_enable_, this->root_->pwm_motor_max_);
			backwards_time_current = this->root_->getCurrentTime();
		}
	}

	// Turn off lift
	gpioWrite(this->root_->rcb_lift_standby_, 0);
	gpioWrite(this->root_->lift_p_, 0);
	gpioWrite(this->root_->lift_n_, 0);
	gpioPWM(this->root_->lift_enable_, 0);

	if (is_time_up)
	{
		this->root_->m_log_.write("Time Ran Out for Lift");
		this->root_->lift_done_ = true;
		this->root_->lift_thread_.join();
		this->root_->lift_and_level_cap_.release();
		this->root_->m_log_.tempSaveProgLog();
		return LIFT_FAILURE;
	}
	else
	{
		this->root_->m_log_.write("Lift success");
		this->root_->m_log_.tempSaveProgLog();
		return LIFT_SUCCESS;
	}
}

EventName State_Full_Lift::unitExecute()
{
	return this->execute();
}