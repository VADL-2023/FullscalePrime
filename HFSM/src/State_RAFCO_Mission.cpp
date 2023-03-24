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
	this->root_->m_log_.write("Starting the SDRs");
	this->root_->radio1_.startSDR();
	this->root_->radio2_.startSDR();
	
	this->root_->m_log_.write("Waiting for packets");

	// These structs hold the output from the radios
	AX25Packet p1;
	AX25Packet p2;

	// These indicate whether or not errors have occurred on either SDR
	bool sdr1_valid = true;
	bool sdr2_valid = true;

	// Save the time that the radios are started
	auto start_time = this->root_->getCurrentTime();

	// Receive packets for a set amount of time
	while (this->root_->getCurrentTime() - start_time < this->root_->length_collect_rafco_ * 1000)
	{
		// If SDR 1 has not had an error and if it has received a packet,
		// then parse that packet
		if (sdr1_valid && this->root_->radio1_.packetAvailable())
		{
			// Read the packet from the SDR and print it out
			p1 = this->root_->radio1_.getPacket();
			this->root_->m_log_.write("Radio 1 Received: " + p1.msg);

			// An error will be returned if the SDR becomes disconnected
			// or if there is an issue with the ports
			if (p1.msg.find("ERROR") != std::string::npos)
			{
				sdr1_valid = false;
			}
		}

		if (sdr2_valid && this->root_->radio2_.packetAvailable())
		{
			// Read the packet from the SDR and print it out
			p2 = this->root_->radio2_.getPacket();
			this->root_->m_log_.write("Radio 2 Received: " + p2.msg);

			// An error will be returned if the SDR becomes disconnected
			// or if there is an issue with the ports
			if (p2.msg.find("ERROR") != std::string::npos)
			{
				sdr2_valid = false;
			}
		}

		// TODO: compare if the received values are the same

		// Execute the individual commands
		if (p1.msg.find("A1") == 0 || p2.msg.find("A1") == 0)
		{
			this->root_->m_log_.write("Swivel one way");
			gpioWrite(this->root_->stepper_2_standby_pin_, 1);
			usleep(1000000);
			// std::cout << "Standby: " << gpioRead(standby_pin) << std::endl;
			this->root_->stepper_2_.step(this->root_->num_steps_);
			usleep(1000000);
			gpioWrite(this->root_->stepper_2_standby_pin_, 0);
		} 
		else if (p1.msg.find("B2") == 0 || p2.msg.find("B2") == 0)
		{
			this->root_->m_log_.write("Swivel other way");
			gpioWrite(this->root_->stepper_2_standby_pin_, 1);
			usleep(1000000);
			// std::cout << "Standby: " << gpioRead(standby_pin) << std::endl;
			this->root_->stepper_2_.step(-this->root_->num_steps_);
			usleep(1000000);
			gpioWrite(this->root_->stepper_2_standby_pin_, 0);
		}
		p1.msg = "";
		p2.msg = "";
	}

	// Shut down the SDRs
	this->root_->m_log_.write("Shutting down SDRs");
	this->root_->radio1_.stopSDR();
	this->root_->radio2_.stopSDR();
	return RAFCO_COMPLETE;
}

EventName State_RAFCO_Mission::unitExecute()
{
	return this->execute();
}