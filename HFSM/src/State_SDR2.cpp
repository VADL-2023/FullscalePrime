#include "State_SDR2.h"
#include "Root.h"

State_SDR2::State_SDR2(): State() {

}

State_SDR2::State_SDR2(StateName name, std::map<EventName, StateName> &state_transitions, Root* root) : State(name, state_transitions, root) {

}

EventName State_SDR2::execute() {
	std::cout << "In State_SDR2 and will return end state." << std::endl;
	return TERMINATE;
}

EventName State_SDR2::unitExecute() {
	std::cout << "Starting SDR" << std::endl;
	this->root_->radio2.startSDR();
	std::cout << "Waiting for packets" << std::endl;
	std::string sdr_output = "";
	while (sdr_output.find("shutdown") == std::string::npos && sdr_output.find("ERROR") == std::string::npos) {
        sdr_output = this->root_->radio2.getPacket();
        std::cout << sdr_output << std::endl;
    }
	std::cout << "Halting SDR" << std::endl;
	sleep(5);
	this->root_->radio2.stopSDR();
	return RECEIVED_PACKETS;
}