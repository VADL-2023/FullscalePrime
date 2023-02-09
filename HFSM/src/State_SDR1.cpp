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
	std::cout << "Waiting for packets for 10 seconds" << std::endl;
	sleep(10);
	std::cout << "Halting SDR" << std::endl;
	this->root_->radio1.stopSDR();
	std::string packetsReceived = this->root_->radio1.getPackets();
	std::cout << packetsReceived;
	return RECEIVED_PACKETS;
}