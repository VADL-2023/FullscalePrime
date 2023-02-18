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
	std::cout << "In State_Full_Level and will return end state." << std::endl;
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
		int desired_servo_pos = this->root_->servo_up_pos_ + ((average_error - this->root_->max_up_angle_) * (this->root_->servo_down_pos_ - this->root_->servo_up_pos_)) / (this->root_->max_down_angle_ - this->root_->max_up_angle_);
		std::cout << "Average error: " << average_error << std::endl;
		std::cout << "Desired servo pos: " << desired_servo_pos << std::endl;
		if (desired_servo_pos > this->root_->servo_down_pos_)
		{
			desired_servo_pos = this->root_->servo_down_pos_;
		}
		else if (desired_servo_pos < this->root_->servo_up_pos_)
		{
			desired_servo_pos = this->root_->servo_up_pos_;
		}
		float pulse_width = this->root_->angleToPulseWidth(this->root_->servo_pulse_max_, this->root_->servo_pulse_min_, this->root_->servo_deg_range_, desired_servo_pos);
		gpioServo(this->root_->level_servo_, pulse_width);
		gpioSleep(0, 2, 0);
	}
	return LEVEL_SUCCESS;
}

EventName State_Full_Level::unitExecute()
{
	return this->execute();
}