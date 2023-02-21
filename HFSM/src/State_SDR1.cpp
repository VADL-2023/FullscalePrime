#include "State_SDR1.h"
#include "Root.h"

State_SDR1::State_SDR1() : State()
{
}

State_SDR1::State_SDR1(StateName name, std::map<EventName, StateName> &state_transitions, Root *root) : State(name, state_transitions, root)
{
}

EventName State_SDR1::execute()
{
	std::cout << "In State_SDR1 and will return end state." << std::endl;
	return unitExecute();
}

EventName State_SDR1::unitExecute()
{
	this->root_->m_log_.write("Starting SDR");
	this->root_->radio1.startSDR();
	
	this->root_->m_log_.write("Waiting for packets");
	std::string sdr_output = "";
	while (sdr_output.find("shutdown") == std::string::npos && sdr_output.find("ERROR") == std::string::npos)
	{
		sdr_output = this->root_->radio1.getPacket();
		this->root_->m_log_.write(sdr_output);
		if (sdr_output.find("A1") == 0)
		{
			this->root_->m_log_.write("Swivel one way");
			gpioWrite(this->root_->stepper_2_standby_pin_, 1);
			usleep(1000000);
			// std::cout << "Standby: " << gpioRead(standby_pin) << std::endl;
			this->root_->stepper_2_.step(this->root_->num_steps_);
			gpioWrite(this->root_->stepper_2_standby_pin_, 0);
		}
		else if (sdr_output.find("B2") == 0)
		{
			this->root_->m_log_.write("Swivel other way");
			gpioWrite(this->root_->stepper_2_standby_pin_, 1);
			usleep(1000000);
			// std::cout << "Standby: " << gpioRead(standby_pin) << std::endl;
			this->root_->stepper_2_.step(-this->root_->num_steps_);
			gpioWrite(this->root_->stepper_2_standby_pin_, 0);
		}
	}
	this->root_->m_log_.write("Halting SDR");
	this->root_->radio1.stopSDR();
	return RECEIVED_PACKETS;
}