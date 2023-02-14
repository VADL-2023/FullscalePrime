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
	std::cout << "In State_Full_RCB and will return end state." << std::endl;
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
			return RCB_SUCCESS;
		}
		bool rcb_stable = false;
		gpioWrite(this->root_->rcb_lift_standby_, 1);
		while (!rcb_stable)
		{
			auto response_rpy = this->root_->m_vn_->readYawPitchRoll();
			auto pitch_error = response_rpy[1];
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
			} else if(pitch_error < -this->root_->rcb_angle_threshold_) {
				gpioWrite(this->root_->rcb_lift_standby_, 1);
				gpioWrite(this->root_->rcb_p_, 0);
				gpioWrite(this->root_->rcb_n_, 1);
				gpioPWM(this->root_->rcb_enable_, this->root_->pwm_motor_max_);
			}
		}
	}
	return RCB_SUCCESS;
}

EventName State_Full_RCB::unitExecute()
{
	return this->execute();
}