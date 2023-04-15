#include "State_Full_Level.h"
#include "Root.h"

State_Full_Level::State_Full_Level() : State()
{
}

State_Full_Level::State_Full_Level(StateName name, std::map<EventName, StateName> &state_transitions, Root *root) : State(name, state_transitions, root)
{
}

EventName State_Full_Level::execute()
{
	this->root_->m_log_.write("In State_Full_Level and will return end state.");
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
			return LEVEL_FAILURE;
		}
	}
	this->root_->is_imu_connected_ = true;
	bool got_level_reading = false;
	for (int i = 0; i < 5 && !got_level_reading; i++)
	{
		try {
		auto response_rpy = this->root_->m_vn_->readYawPitchRoll();
		double average_error = 0;
		for (int i = 0; i < this->root_->num_level_samples_; i++)
		{
			auto yaw_measurement = response_rpy[2];
			if (yaw_measurement < 0)
			{
				average_error += (this->root_->ideal_level_angle_ + yaw_measurement);
			}
			else
			{
				average_error += (yaw_measurement - this->root_->ideal_level_angle_);
			}
		}
		average_error /= this->root_->num_level_samples_;
		int desired_servo_pos = (((this->root_->max_pulse_width_ - this->root_->min_pulse_width_) / (this->root_->max_angle_ - this->root_->min_angle_)) * (average_error - this->root_->min_angle_)) + this->root_->min_pulse_width_;
		this->root_->m_log_.write("Average error: " + std::to_string(average_error));
		this->root_->m_log_.write("Desired servo pulsewidth: " + std::to_string(desired_servo_pos));
		if (desired_servo_pos > this->root_->max_pulse_width_)
		{
			this->root_->m_log_.write("At max pulse width");
			desired_servo_pos = this->root_->max_pulse_width_;
		}
		else if (desired_servo_pos < this->root_->min_pulse_width_)
		{
			this->root_->m_log_.write("At min pulse width");
			desired_servo_pos = this->root_->min_pulse_width_;
		}
		gpioServo(this->root_->level_servo_, desired_servo_pos);
		gpioSleep(0, 2, 0);
		got_level_reading = true;
		} catch (...) {
			this->root_->m_log_.write("Couldn't read from IMU. About to try again");
		}
	}
	this->root_->lift_done_ = true;
	try {
	this->root_->lift_thread_.join();
	} catch(...) {
		std::cout << "No video thread" << std::endl;
	}
	this->root_->lift_and_level_cap_.release();
	return LEVEL_SUCCESS;
}

EventName State_Full_Level::unitExecute()
{
	return this->execute();
}