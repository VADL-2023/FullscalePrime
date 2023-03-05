#include "State_RAFCO_Mission.h"
#include "Root.h"

State_RAFCO_Mission::State_RAFCO_Mission() : State()
{
}

State_RAFCO_Mission::State_RAFCO_Mission(StateName name, std::map<EventName, StateName> &state_transitions, Root *root) : State(name, state_transitions, root)
{
}

EventName State_RAFCO_Mission::execute()
{
	this->root_->m_log_.write("Starting SDRs");
	this->root_->radio1.startSDR();
	this->root_->radio2.startSDR();
	

	this->root_->m_log_.write("Waiting for packets");

	std::string sdr1_output = "";
	std::string sdr2_output = "";
	bool sdr1_valid = true;
	bool sdr2_valid = true;
	while (sdr1_valid || sdr2_valid)
	{
		if (sdr1_valid && this->root_->radio1.packetAvailable())
		{
			sdr1_output = this->root_->radio1.getPacket();
			this->root_->m_log_.write("Radio 1 Received: " + sdr1_output);
			if (sdr1_output.find("shutdown") != std::string::npos)
			{
				this->root_->m_log_.write("Shutting down SDRs");
				this->root_->radio1.stopSDR();
				this->root_->radio2.stopSDR();
				return RAFCO_COMPLETE;
			}
			else if (sdr1_output.find("ERROR") != std::string::npos)
			{
				sdr1_valid = false;
			}
		}
		if (sdr2_valid && this->root_->radio2.packetAvailable())
		{
			sdr2_output = this->root_->radio2.getPacket();
			this->root_->m_log_.write("Radio 2 Received: " + sdr2_output);
			if (sdr2_output.find("shutdown") != std::string::npos)
			{
				this->root_->m_log_.write("Shutting down SDRs");
				this->root_->radio1.stopSDR();
				this->root_->radio2.stopSDR();
				return RAFCO_COMPLETE;
			}
			else if (sdr2_output.find("ERROR") != std::string::npos)
			{
				sdr2_valid = false;
			}
		}
		if (sdr1_output.find("A1") == 0 || sdr2_output.find("A1") == 0)
		{
			this->root_->m_log_.write("Swivel one way");
			gpioWrite(this->root_->stepper_2_standby_pin_, 1);
			usleep(1000000);
			// std::cout << "Standby: " << gpioRead(standby_pin) << std::endl;
			this->root_->stepper_2_.step(this->root_->num_steps_);
			usleep(1000000);
			gpioWrite(this->root_->stepper_2_standby_pin_, 0);
		} else if (sdr1_output.find("B2") == 0 || sdr2_output.find("B2") == 0)
		{
			this->root_->m_log_.write("Swivel other way");
			gpioWrite(this->root_->stepper_2_standby_pin_, 1);
			usleep(1000000);
			// std::cout << "Standby: " << gpioRead(standby_pin) << std::endl;
			this->root_->stepper_2_.step(-this->root_->num_steps_);
			usleep(1000000);
			gpioWrite(this->root_->stepper_2_standby_pin_, 0);
		}
		sdr1_output = "";
		sdr2_output = "";
	}
	this->root_->m_log_.write("Shutting down SDRs");
	this->root_->radio1.stopSDR();
	this->root_->radio2.stopSDR();
	return RAFCO_COMPLETE;
}

EventName State_RAFCO_Mission::unitExecute()
{
	return this->execute();
}