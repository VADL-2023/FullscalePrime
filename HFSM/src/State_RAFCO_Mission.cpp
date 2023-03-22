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
	this->root_->radio2.startSDR();
	this->root_->radio1.startSDR();
	

	this->root_->m_log_.write("Waiting for packets");

	// These strings will hold the output from the radios
	std::string sdr1_output = "";
	std::string sdr2_output = "";

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
		if (sdr1_valid && this->root_->radio1.packetAvailable())
		{
			// Read the packet from the SDR and print it out
			sdr1_output = this->root_->radio1.getPacket();
			this->root_->m_log_.write("Radio 1 Received: " + sdr1_output);

			// An error will be returned if the SDR becomes disconnected
			// or if there is an issue with the ports
			if (sdr1_output.find("ERROR") != std::string::npos)
			{
				sdr1_valid = false;
			}
		}

		if (sdr2_valid && this->root_->radio2.packetAvailable())
		{
			// Read the packet from the SDR and print it out
			sdr2_output = this->root_->radio2.getPacket();
			this->root_->m_log_.write("Radio 2 Received: " + sdr2_output);

			// An error will be returned if the SDR becomes disconnected
			// or if there is an issue with the ports
			if (sdr2_output.find("ERROR") != std::string::npos)
			{
				sdr2_valid = false;
			}
		}

		// TODO: compare if the received values are the same

		// Execute the individual commands
		if (sdr1_output.find("A1") == 0 || sdr2_output.find("A1") == 0)
		{
			this->root_->m_log_.write("Swivel one way");
			gpioWrite(this->root_->stepper_2_standby_pin_, 1);
			usleep(1000000);
			// std::cout << "Standby: " << gpioRead(standby_pin) << std::endl;
			this->root_->stepper_2_.step(this->root_->num_steps_);
			usleep(1000000);
			gpioWrite(this->root_->stepper_2_standby_pin_, 0);
		} 
		else if (sdr1_output.find("B2") == 0 || sdr2_output.find("B2") == 0)
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

	// Shut down the SDRs
	this->root_->m_log_.write("Shutting down SDRs");
	this->root_->radio1.stopSDR();
	this->root_->radio2.stopSDR();
	return RAFCO_COMPLETE;
}

EventName State_RAFCO_Mission::unitExecute()
{
	return this->execute();
}