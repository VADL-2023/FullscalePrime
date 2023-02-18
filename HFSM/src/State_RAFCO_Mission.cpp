#include "State_RAFCO_Mission.h"
#include "Root.h"

State_RAFCO_Mission::State_RAFCO_Mission(): State() {

}

State_RAFCO_Mission::State_RAFCO_Mission(StateName name, std::map<EventName, StateName> &state_transitions, Root* root) : State(name, state_transitions, root) {

}

EventName State_RAFCO_Mission::execute() {
	this->root_->m_log_.write("Starting SDRs");
	this->root_->radio1.startSDR();
	this->root_->radio2.startSDR();

	this->root_->m_log_.write("Waiting for packets");

	std::string sdr1_output = "";
	std::string sdr2_output = "";
	bool sdr1_valid = true;
	bool sdr2_valid = true;
	while (sdr1_valid || sdr2_valid) {
		if (sdr1_valid && this->root_->radio1.packetAvailable()) {
        	sdr1_output = this->root_->radio1.getPacket();
			this->root_->m_log_.write("Radio 1 Received: " + sdr1_output);
			if (sdr1_output.find("shutdown") != std::string::npos || sdr1_output.find("ERROR") != std::string::npos) {
				sdr1_valid = false;
			}
		}
		if (this->root_->radio2.packetAvailable()) {
			sdr2_output = this->root_->radio2.getPacket();
			this->root_->m_log_.write("Radio 2 Received: " + sdr2_output);
			if (sdr2_output.find("shutdown") != std::string::npos || sdr2_output.find("ERROR") != std::string::npos) {
				sdr2_valid = false;
			}
		}
    }
	this->root_->m_log_.write("Shutting down SDRs");
	this->root_->radio1.stopSDR();
	return RAFCO_COMPLETE;
}

EventName State_RAFCO_Mission::unitExecute() {
	return this->execute();
}