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
	std::cout << "Waiting for packets for 10 seconds" << std::endl;
	sleep(10);
	std::cout << "Halting SDR" << std::endl;
	this->root_->radio2.stopSDR();
	std::string packetsReceived = this->root_->radio2.getPackets();
	std::cout << packetsReceived;
	return RECEIVED_PACKETS;
}