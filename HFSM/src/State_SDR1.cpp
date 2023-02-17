#include "State_SDR1.h"
#include "Root.h"

State_SDR1::State_SDR1(): State() {

}

State_SDR1::State_SDR1(StateName name, std::map<EventName, StateName> &state_transitions, Root* root) : State(name, state_transitions, root) {

}

EventName State_SDR1::execute() {
	std::cout << "In State_SDR1 and will return end state." << std::endl;
	return TERMINATE;
}

EventName State_SDR1::unitExecute() {
	std::cout << "Starting SDR" << std::endl;
	this->root_->radio1.startSDR();
	std::cout << "Waiting for packets" << std::endl;
	std::string sdr_output = "";
	while (sdr_output.find("shutdown") == std::string::npos && sdr_output.find("ERROR") == std::string::npos) {
        sdr_output = this->root_->radio1.getPacket();
        std::cout << sdr_output << std::endl;
    }
	std::cout << "Halting SDR" << std::endl;
	this->root_->radio1.stopSDR();
	return RECEIVED_PACKETS;
}