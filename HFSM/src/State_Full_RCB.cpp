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
	gpioWrite(this->root_->rcb_lift_standby_, 1);
	bool is_time_up = false;
	double start_time = this->root_->getCurrentTime();
	double current_time = start_time;
	this->root_->m_log_.write("Starting RCB");
	bool first_imu_disconnect = true;
	while (!rcb_stable)
	{
		try
		{
			auto response_rpy = this->root_->m_vn_->readYawPitchRoll();
			auto pitch_error = response_rpy[1];
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
			if (first_imu_disconnect)
			{
				this->root_->m_log_.write("RCB IMU Exception: ");
				this->root_->m_log_.write(e.what());
				first_imu_disconnect = false;
			}
		}
		current_time = this->root_->getCurrentTime();
		if (current_time - start_time > this->root_->full_rcb_time_threshold_)
		{
			is_time_up = true;
			rcb_stable = true;
		}
	}
	this->root_->m_log_.write("Initiating Nacelle servo unlock");
	gpioServo(this->root_->nacelle_servo_, this->root_->nacelle_unlock_);
	gpioSleep(0, 2, 0);
	this->root_->m_log_.write("Initiating Nacelle servo lock");
	if (is_time_up)
	{
		this->root_->m_log_.write("RCB Timeout Error");
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