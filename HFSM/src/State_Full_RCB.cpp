#include "State_Full_RCB.h"
#include "Root.h"

State_Full_RCB::State_Full_RCB() : State()
{
}

State_Full_RCB::State_Full_RCB(StateName name, std::map<EventName, StateName> &state_transitions, Root *root) : State(name, state_transitions, root)
{
}

EventName State_Full_RCB::execute()
{
	this->root_->m_log_.write("In State Full RCB");
	if (!this->root_->is_imu_connected_)
	{
		try
		{
			// IMU connection and configuration
			this->root_->m_vn_ = new VnSensor();
			this->root_->m_vn_->connect(IMU_PORT, IMU_BAUD_RATE);
			this->root_->m_log_.write("IMU Connected");
		}
		catch (const std::exception &e)
		{
			this->root_->m_log_.write("Exception: ");
			this->root_->m_log_.write(e.what());
			this->root_->m_log_.write("IMU failed to connect... restart program");
			this->root_->m_log_.write("Initiating Nacelle servo unlock");
			gpioServo(this->root_->nacelle_servo_, this->root_->nacelle_unlock_);
			gpioSleep(0, 2, 0);
			this->root_->m_log_.write("Initiating Nacelle servo lock");
			return RCB_FAILURE;
		}
	}
	this->root_->is_imu_connected_ = true;
	bool rcb_stable = false;
	bool doing_backup = false;
	if (this->root_->primary_camera_stream_ != "/dev/videoCam2")
	{
		doing_backup = true;
	}
	gpioWrite(this->root_->rcb_lift_standby_, 1);
	bool is_time_up = false;
	double start_time = this->root_->getCurrentTime();
	double current_time = start_time;
	this->root_->m_log_.write("Starting RCB");
	bool first_imu_disconnect = true;
	cv::VideoCapture cap;
	// open the default camera using default API
	// cap.open(0);
	// OR advance usage: select any API backend
	int apiID = cv::CAP_ANY; // 0 = autodetect default API
	int numPics = 0;
	std::thread t1_test;
	if (this->root_->primary_camera_stream_ != "")
	{
		// open selected camera using selected API
		cap.open(this->root_->primary_camera_stream_, apiID);
		// check if we succeeded
		if (!cap.isOpened())
		{
			this->root_->m_log_.write("ERROR! Unable to open camera " + this->root_->primary_camera_stream_);
		}
		std::thread t1(&Root::camThreadRCB, this->root_, &cap);
		t1_test = std::move(t1);
	}
	if (this->root_->primary_camera_stream_ == "/dev/videoCam1")
	{
		this->root_->m_log_.write("Rotating to camera 1");
	}
	else if (this->root_->primary_camera_stream_ == "/dev/videoCam1")
	{
		this->root_->m_log_.write("Rotating to camera 3");
	}
	else
	{
		this->root_->m_log_.write("Rotating to camera 2");
	}
	while (!rcb_stable)
	{
		try
		{
			auto response_rpy = this->root_->m_vn_->readYawPitchRoll();
			auto pitch_error = response_rpy[1];
			if (this->root_->primary_camera_stream_ == "/dev/videoCam1")
			{
				pitch_error -= 90;
			}
			else if (this->root_->primary_camera_stream_ == "/dev/videoCam3")
			{
				pitch_error += 90;
			}

			this->root_->m_log_.write("RCB Angle Error: " + std::to_string(pitch_error));
			if (pitch_error <= this->root_->rcb_angle_threshold_ && pitch_error >= -this->root_->rcb_angle_threshold_)
			{
				rcb_stable = true;
				gpioWrite(this->root_->rcb_lift_standby_, 0);
				gpioWrite(this->root_->rcb_p_, 0);
				gpioWrite(this->root_->rcb_n_, 0);
				gpioPWM(this->root_->rcb_enable_, 0);
			}
			else if (pitch_error > this->root_->rcb_angle_threshold_)
			{
				gpioWrite(this->root_->rcb_lift_standby_, 1);
				gpioWrite(this->root_->rcb_p_, 1);
				gpioWrite(this->root_->rcb_n_, 0);
				gpioPWM(this->root_->rcb_enable_, this->root_->pwm_motor_max_);
			}
			else if (pitch_error < -this->root_->rcb_angle_threshold_)
			{
				gpioWrite(this->root_->rcb_lift_standby_, 1);
				gpioWrite(this->root_->rcb_p_, 0);
				gpioWrite(this->root_->rcb_n_, 1);
				gpioPWM(this->root_->rcb_enable_, this->root_->pwm_motor_max_);
			}
		}
		catch (const std::exception &e)
		{
			this->root_->m_log_.write("RCB IMU Disconnected");
			gpioWrite(this->root_->rcb_lift_standby_, 0);
			gpioWrite(this->root_->rcb_p_, 0);
			gpioWrite(this->root_->rcb_n_, 0);
			gpioPWM(this->root_->rcb_enable_, 0);
			this->root_->rcb_done_ = true;
			t1_test.join();

			this->root_->m_log_.write("Initiating Nacelle servo unlock");
			gpioServo(this->root_->nacelle_servo_, this->root_->nacelle_unlock_);
			gpioSleep(0, 2, 0);
			this->root_->m_log_.write("Initiating Nacelle servo lock");
			cap.release();
			return RCB_FAILURE;
		}
		current_time = this->root_->getCurrentTime();
		if (current_time - start_time > this->root_->full_rcb_time_threshold_)
		{
			is_time_up = true;
			rcb_stable = true;
		}
	}
	this->root_->rcb_done_ = true;
	t1_test.join();

	this->root_->m_log_.write("Initiating Nacelle servo unlock");
	gpioServo(this->root_->nacelle_servo_, this->root_->nacelle_unlock_);
	gpioSleep(0, 2, 0);
	this->root_->m_log_.write("Initiating Nacelle servo lock");
	cap.release();
	if (is_time_up)
	{
		this->root_->m_log_.write("RCB Timeout Error");
		return RCB_FAILURE;
	}
	else if (doing_backup)
	{
		this->root_->m_log_.write("RCB Backup Success");
		return RCB_FAILURE;
	}
	else
	{
		this->root_->m_log_.write("RCB Success");
		return RCB_SUCCESS;
	}
}

EventName State_Full_RCB::unitExecute()
{
	return this->execute();
}